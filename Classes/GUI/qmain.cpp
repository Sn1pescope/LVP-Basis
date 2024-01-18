#include "qmain.h"
#include "Classes/GUI/Dialogs/qworkunitdialog.h"
#include "ui_qmain.h"
#include <Classes/GUI/Tools/qcropplantingplanningtool.h>
#include <Classes/GUI/Dialogs/qcreateeditmeasure.h>
#include <Classes/GUI/Dialogs/qcompletemeasures.h>

QMain::QMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::QMain)
{
    ui->setupUi(this);
    connect(&CDataManager::instance(), &CDataManager::currentFarmChanged, this, &QMain::updateMainScreenActive);
    connect(&CDataManager::instance(), &CDataManager::currentFarmChanged, this, &QMain::createFieldList);
    connect(ui->listFields, &QTreeWidget::itemDoubleClicked, this, &QMain::editField);
    connect(ui->listFields, &QTreeWidget::currentItemChanged, this, &QMain::updateCurrentField);
    connect(this, &QMain::shutdown, &CMeasure::instance(), &CMeasure::shutdown);
    lbls[0]=ui->lbl_HarvestYear1;lbls[1]=ui->lbl_HarvestYear2;lbls[2]=ui->lbl_HarvestYear3;lbls[3]=ui->lbl_HarvestYear4;lbls[4]=ui->lbl_HarvestYear5;
    lbls[5]=ui->lbl_Crop1;lbls[6]=ui->lbl_Crop2;lbls[7]=ui->lbl_Crop3;lbls[8]=ui->lbl_Crop4;lbls[9]=ui->lbl_Crop5;
    lbls[10]=ui->lbl_InterCrop1;lbls[11]=ui->lbl_InterCrop2;lbls[12]=ui->lbl_InterCrop3;lbls[13]=ui->lbl_InterCrop4;lbls[14]=ui->lbl_InterCrop5;
    lbls[15]=ui->lbl_Name;lbls[16]=ui->lbl_Size;lbls[17]=ui->lbl_RegNumber;
    this->hide();
    QTimer::singleShot(0, this, &QMain::eventLoopStarted);
}

void QMain::eventLoopStarted(){
    // Start intitializing program
    QWelcome w(this);
    w.exec();
    /*if(!w.exec()){
        QWarningDialog warn(this, tr("Failed to initialize program! Exit..."), true, false);
        warn.exec();
        exit(1);
    }*/
    for(int i = 0; i < 5; i++){
        lbls[i]->setText(CCommunicator::generateHarvestYearString(i));
    }
    ui->lbl_NextHarvestYear->setText(tr("Next Harvest Year:"));

    //Show main window
    updateMainScreenActive();
    this->setWindowState(Qt::WindowMaximized);
    ui->mainScreen->tabBar()->setVisible(false);
    this->show();
    QWarningDialog(this, tr("Information"), tr("Current activated farm: %1\n"
                                               "You can change the current farm with 'Settings' -> 'Farm'.").arg(CDataManager::getCurrentFarmName()), true, false).exec();

    //Ask for completition of tasks
    if(CDataManager::getCurrentFarmName() != ""){
        //Get tasks in past with state planned or in progress
        std::vector<QSharedPointer<CMeasure>> meas = CCommunicator::getMeasures("", CMeasure::STATE_PLANNED, -1, QDate::currentDate(), true);
        std::vector<QSharedPointer<CMeasure>> meastwo = CCommunicator::getMeasures("", CMeasure::STATE_IN_PROGRESS, -1, QDate::currentDate(), true);
        meas.insert(meas.end(), meastwo.begin(), meastwo.end());
        QCompleteMeasures(this, meas).exec();
    }
}

//------------------------------------ Tools -------------------------------------------------

void QMain::cropPlanning(){
    //Start crop planning tool
    QCropPlantingPlanningTool(this).exec();
}


//-------------------------------- Field Management ------------------------------------------

void QMain::createFieldList(){
    ui->listFields->clear();
    CFarm* curFarm = CDataManager::getCurrentFarm();
    if(curFarm != 0x0){
        std::vector<QString> fieldNames = curFarm->getAllFieldNames();
        std::vector<CWorkUnit> workUnits = curFarm->getAllWorkUnits();
        //Add each workUnit
        foreach (CWorkUnit wu, workUnits) {
            ui->listFields->addTopLevelItem(new QTreeWidgetItem(QStringList(wu.getName())));
        }
        //Add each field
        foreach (QString field, fieldNames){
            QString wu = curFarm->getWorkUnitOfField(field);
            if(wu != nullptr){
                //Add to wu
                ui->listFields->findItems(wu, Qt::MatchExactly).at(0)->addChild(new QTreeWidgetItem(QStringList(field)));
            }else{
                //Add as topLevel
                ui->listFields->addTopLevelItem(new QTreeWidgetItem(QStringList(field)));
            }
        }
    }
}

void QMain::deleteField(){
    //Delete selected field
    if(ui->listFields->currentItem() != nullptr && ui->listFields->currentItem()->childCount() == 0){
        QString name = ui->listFields->currentItem()->text(0);
        QWarningDialog warn(this, tr("Delete Field"), tr("Do you really want to delete field '%1'?").arg(name), true, true);
        if(warn.exec()){
            bool data = QWarningDialog(this, tr("Delete field"), tr("Do you want to save the data of field '%1' in an external file before deleting the field?").arg(name), true, false, true).exec();
            if(data){
                QString path = CCommunicator::exportFieldData(name);
                QWarningDialog(this, tr("Information"), tr("Data was saved in following directory:\n'%1'").arg(path), true).exec();
            }
            //Delete field
            CCommunicator::deleteFieldOfCurrentFarm(name);
            //Update listWidget
            createFieldList();
        }
    }
}

void QMain::editField(){
    //Edit selected field through dialog
    if(ui->listFields->currentItem() != nullptr && ui->listFields->currentItem()->childCount() == 0){
        QString name = ui->listFields->currentItem()->text(0);
        QCreateField edit(this, name);
        edit.exec();
    }
}

void QMain::newField(){
    //Create new field through dialog
    QCreateField dia(this, true);
    dia.exec();
    createFieldList();
}

void QMain::splitField(){
    //Check if its not a wu
    if(ui->listFields->currentItem() != nullptr && ui->listFields->currentItem()->childCount() == 0){
        QSplitMergeField(this, *CDataManager::getCurrentFarm()->getField(ui->listFields->currentItem()->text(0))).exec();
        //Update GUI
        save();
        createFieldList();
    }
}

void QMain::mergeFields(){
    //Check if two fields are selected which are both topLevel or both of the same wu
    if(ui->listFields->selectedItems().size() == 2 && ui->listFields->selectedItems().at(0)->parent() == ui->listFields->selectedItems().at(1)->parent()){
        //Make one field out of both
        QSplitMergeField(this, *CDataManager::getCurrentFarm()->getField(ui->listFields->selectedItems().at(0)->text(0)),
                         *CDataManager::getCurrentFarm()->getField(ui->listFields->selectedItems().at(1)->text(0))).exec();
        //Update GUI
        save();
        createFieldList();
    }
}

void QMain::setWorkUnit(){
    //Check if its not a work unit
    if(ui->listFields->currentItem() != nullptr && ui->listFields->currentItem()->childCount() == 0){
        QWorkUnitDialog(this, ui->listFields->currentItem()->text(0)).exec();
        save();
        createFieldList();
    }
}

QMain::~QMain()
{
    delete ui;
}


//--------------------------------- Field Data ----------------------------------------------

void QMain::updateCurrentField(){
    if(ui->listFields->currentItem() == nullptr || ui->listFields->currentItem()->childCount() > 0){
        for (int i = 5; i < 15; i++) {
            if(i < 10){
                //Crops
                lbls[i]->setText("");
            }else{
                //InterCrops
                lbls[i]->setText("");
            }
        }
        lbls[15]->setText("");
        lbls[16]->setText("");
        lbls[17]->setText("");
        ui->lbl_PlannedCrop->setText("");
        ui->lbl_PlannedInterCrop->setText("");
    }else{
        CField* field = CDataManager::getCurrentFarm()->getField(ui->listFields->currentItem()->text(0));
        for (int i = 5; i < 15; i++) {
            if(i < 10){
                //Crops
                if(field->getLastCrop(i - 5) != nullptr){
                    lbls[i]->setText(tr("Crop: ") + field->getLastCrop(i - 5)->getName());
                }else{
                    lbls[i]->setText("");
                }
            }else{
                //InterCrops
                if(field->getLastInterCrop(i - 10) != nullptr){
                    lbls[i]->setText(tr("Inter Crop: ") + field->getLastInterCrop(i - 10)->getName());
                }else{
                    lbls[i]->setText("");
                }
            }
        }
        lbls[15]->setText(field->getName());
        lbls[16]->setText(QString::number(field->getSize()));
        lbls[17]->setText(field->getRegNumber());
        if(field->getNextCrop() != nullptr){
            ui->lbl_PlannedCrop->setText(tr("Crop: ") + field->getNextCrop()->getName());
        }else{
            ui->lbl_PlannedCrop->setText("");
        }
        if(field->getNextInterCrop() != nullptr){
            ui->lbl_PlannedInterCrop->setText(tr("Inter Crop: ") + field->getNextInterCrop()->getName());
        }else{
            ui->lbl_PlannedInterCrop->setText("");
        }
    }
}


//--------------------------------- Slots ---------------------------------------------------

void QMain::updateMainScreenActive(){
    if(CDataManager::getCurrentFarmName() != ""){
        ui->mainScreen->setEnabled(true);
    }else{
        ui->mainScreen->setEnabled(false);
    }
}

void QMain::openSettings(){
    //Open Settings Dialog
    QSettingsDialog settings(this);
    settings.exec();
}

void QMain::save(){
    CCommunicator::saveData();
}

void QMain::homeTab(){
    ui->mainScreen->setCurrentIndex(0);
}

void QMain::tasksTab(){
    ui->mainScreen->setCurrentIndex(1);
    updateTasksCbEntrys();
}

//-------------------------------- Tasks -----------------------------------------------------

void QMain::newMeasure(){
    //Create new measure
    QCreateEditMeasure(this, true).exec();
    updateCalendar(ui->tabWidgetTasks->currentIndex());
}

void QMain::editMeasure(){
    //Edit selected measure
    int row;
    QString f;
    QString key;
    QSharedPointer<CMeasure> meas;
    if(ui->tabWidgetTasks->currentIndex() == 0 && !ui->listMeasures->selectedItems().empty()){
        row = ui->listMeasures->selectedItems().at(0)->row();

        f = ui->listMeasures->item(row, 2)->text();
        key = ui->listMeasures->item(row, 4)->text();

        meas = CDataManager::getCurrentFarm()->getField(f)->getMeasure(key);
        QCreateEditMeasure(this, meas).exec();
        ui->listMeasures->item(row, 0)->setText(CMeasure::STATES.value(meas->getState()));
        ui->listMeasures->item(row, 1)->setText(meas->getDate().toString("dd.MM.yyyy"));
        ui->listMeasures->item(row, 2)->setText(meas->getField());
        ui->listMeasures->item(row, 3)->setText(CMeasure::TYPES.value(meas->getType()));
        ui->listMeasures->item(row, 4)->setText(meas->getKey());
    }else if(ui->tabWidgetTasks->currentIndex() == 1 && !ui->listCalendar->selectedItems().empty()){
        row = ui->listCalendar->selectedItems().at(0)->row();

        f = ui->listCalendar->item(row, 2)->text();
        key = ui->listCalendar->item(row, 4)->text();

        meas = CDataManager::getCurrentFarm()->getField(f)->getMeasure(key);
        QCreateEditMeasure(this, meas).exec();
        ui->listCalendar->item(row, 0)->setText(CMeasure::STATES.value(meas->getState()));
        ui->listCalendar->item(row, 1)->setText(meas->getDate().toString("dd.MM.yyyy"));
        ui->listCalendar->item(row, 2)->setText(meas->getField());
        ui->listCalendar->item(row, 3)->setText(CMeasure::TYPES.value(meas->getType()));
        ui->listCalendar->item(row, 4)->setText(meas->getKey());
    }else{
        return;
    }

}

void QMain::deleteMeasure(){
    //Delete selected measure
    if(!ui->listMeasures->selectedItems().empty()){
        int row = ui->listMeasures->selectedItems().at(0)->row();

        CField* field = CDataManager::getCurrentFarm()->getField(ui->listMeasures->item(row, 2)->text());
        QString key = ui->listMeasures->item(row, 4)->text();
        field->removeMeasure(key);
        updateCalendar(ui->tabWidgetTasks->currentIndex());
    }
}

void QMain::createMeasureList(){
    //Create measure list
    QString field = ui->cbFilterField->currentText();
    int state = ui->cbFilterStates->currentIndex() - 1;
    int type = ui->cbFilterType->currentIndex() - 1;
    QDate date = ui->deFilterDate->date();
    bool before = !ui->cbFilterDate->currentIndex();
    std::vector<QSharedPointer<CMeasure>> measures = CCommunicator::getMeasures(field, state, type, date, before);
    ui->listMeasures->setRowCount(measures.size());

    for(int i = 0; i < (int)measures.size(); i++) {
        QSharedPointer<CMeasure> measure = measures.at(i);
        ui->listMeasures->setItem(i, 0, new QTableWidgetItem(CMeasure::STATES.value(measure->getState())));
        ui->listMeasures->setItem(i, 1, new QTableWidgetItem(measure->getDate().toString("dd.MM.yyyy")));
        ui->listMeasures->setItem(i, 2, new QTableWidgetItem(measure->getField()));
        ui->listMeasures->setItem(i, 3, new QTableWidgetItem(CMeasure::TYPES.value(measure->getType())));
        ui->listMeasures->setItem(i, 4, new QTableWidgetItem(measure->getKey()));
    }

    sortMeasureList();
}

void QMain::sortMeasureList(){
    sortMeasureList(ui->cbSortMeasureList->currentIndex());
}

void QMain::sortMeasureList(int index){
    Qt::SortOrder order;
    if(ui->radioButton->isChecked()){
        order = Qt::AscendingOrder;
    }else{
        order = Qt::DescendingOrder;
    }
    ui->listMeasures->sortItems(index, order);
}

void QMain::updateTasksCbEntrys(){
    //Tab is called -> Update cb entrys, set standard filters, createList
    ui->listMeasures->hideColumn(4);
    ui->listCalendar->hideColumn(4);
    //Sort cb
    ui->cbSortMeasureList->clear();
    for(int i = 0; i < ui->listMeasures->columnCount(); i++){
        ui->cbSortMeasureList->insertItem(i, ui->listMeasures->horizontalHeaderItem(i)->text());
    }
    ui->cbSortMeasureList->setCurrentIndex(1); //Standard = Date

    //Filter States
    ui->cbFilterStates->clear();
    ui->cbFilterStates->addItem("");
    QMap<int, QString> states = CMeasure::STATES;
    QList<int> keys = states.keys();
    foreach(int key, keys){
        ui->cbFilterStates->insertItem(key+1, states.value(key));
    }
    ui->cbFilterStates->setCurrentIndex(0);

    //Filter Date
    ui->cbFilterDate->setCurrentIndex(1); //Set to after
    ui->deFilterDate->setDate(QDate::currentDate());

    //Filter field
    ui->cbFilterField->clear();
    ui->cbFilterField->addItem("");
    std::vector<QString> fields = CDataManager::getCurrentFarm()->getAllFieldNames();
    foreach(QString name, fields){
        ui->cbFilterField->addItem(name);
    }
    if(ui->listFields->currentItem() != nullptr && ui->listFields->currentItem()->childCount() == 0){
        QString name = ui->listFields->currentItem()->text(0);
        ui->cbFilterField->setCurrentText(name);
    }

    //Filter Type
    ui->cbFilterType->clear();
    ui->cbFilterType->addItem("");
    QMap<int, QString> types = CMeasure::TYPES;
    QList<int> typeKeys = types.keys();
    foreach(int key, typeKeys){
        ui->cbFilterType->insertItem(key+1, types.value(key));
    }
    ui->cbFilterType->setCurrentIndex(0);
    updateCalendar(ui->tabWidgetTasks->currentIndex());
}

void QMain::updateCalendar(int currentIndex){
    if(currentIndex == 1){
        //Update calendar
        ui->calendarWidget->updateMeasures(CCommunicator::getMeasures());
        updateCalendarList();
    }else{
        //Update list
        createMeasureList();
    }
}

void QMain::updateCalendarList(){
    QDate date = ui->calendarWidget->selectedDate();
    std::vector<QSharedPointer<CMeasure>> measures = CCommunicator::getMeasures("", -1, -1, date, true, true);
    ui->listCalendar->setRowCount(measures.size());

    for(int i = 0; i < (int)measures.size(); i++) {
        QSharedPointer<CMeasure> measure = measures.at(i);
        ui->listCalendar->setItem(i, 0, new QTableWidgetItem(CMeasure::STATES.value(measure->getState())));
        ui->listCalendar->setItem(i, 1, new QTableWidgetItem(measure->getDate().toString("dd.MM.yyyy")));
        ui->listCalendar->setItem(i, 2, new QTableWidgetItem(measure->getField()));
        ui->listCalendar->setItem(i, 3, new QTableWidgetItem(CMeasure::TYPES.value(measure->getType())));
        ui->listCalendar->setItem(i, 4, new QTableWidgetItem(measure->getKey()));
    }
}

void QMain::setDone(){
    if(!ui->listMeasures->selectedItems().empty()){
        int row = ui->listMeasures->selectedItems().at(0)->row();

        QString f = ui->listMeasures->item(row, 2)->text();
        QString key = ui->listMeasures->item(row, 4)->text();

        QSharedPointer<CMeasure> meas = CDataManager::getCurrentFarm()->getField(f)->getMeasure(key);

        meas->setState(CMeasure::STATE_DONE);

        ui->listMeasures->item(row, 0)->setText(CMeasure::STATES.value(meas->getState()));
    }
}

//--------------------------------- Events ---------------------------------------------------

void QMain::closeEvent(QCloseEvent* ev){
    //Ask if really want to exit program
    QWarningDialog dialog(this, tr("Do you really want to exit?"), tr("Do you really want to exit?"), true, true);
    if(dialog.exec()){
        //Save and exit
        save();
        emit shutdown();
        ev->accept();
    }else{
        //Ignore signal
        ev->ignore();
    }
}


//---------------------------------- Languages -----------------------------------------------

// Slot called by welcome input menu
void QMain::slotWelcomeBoxChanged(int index){
    if(-1 != index) {
        // load the language
        QTranslator* translator = CLanguageManager::loadLanguage(this->findChild<QWelcome*>("QWelcome")->findChild<QComboBox*>("languageBox")->itemData(index).toString());
        if(!(translator == nullptr)){
            qApp->installTranslator(translator);
        }
        // setWindowIcon(action->icon());
    }
}

//Slot called by settings input menu
void QMain::slotLangBoxChanged(int index){
    if(-1 != index) {
        // load the language
        QTranslator* translator = CLanguageManager::loadLanguage(this->findChild<QSettingsDialog*>("QSettingsDialog")->findChild<QComboBox*>("languageBox")->itemData(index).toString());
        if(!(translator == nullptr)){
            qApp->installTranslator(translator);
        }
        // setWindowIcon(action->icon());
    }
}

// Run time language switching
void QMain::changeEvent(QEvent* event) {
 if(0 != event) {
  switch(event->type()) {
   // this event is send if a translator is loaded
   case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;

   // this event is send, if the system, language changes
   case QEvent::LocaleChange:
   {
    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    CLanguageManager::loadLanguage(locale);
   }
   break;
  }
 }
 QMainWindow::changeEvent(event);
}

