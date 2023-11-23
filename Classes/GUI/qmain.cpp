#include "qmain.h"
#include "Classes/GUI/Dialogs/qworkunitdialog.h"
#include "ui_qmain.h"

QMain::QMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::QMain)
{
    ui->setupUi(this);
    connect(&CDataManager::instance(), &CDataManager::currentFarmChanged, this, &QMain::updateMainScreenActive);
    connect(&CDataManager::instance(), &CDataManager::currentFarmChanged, this, &QMain::createFieldList);
    connect(ui->listFields, &QTreeWidget::itemDoubleClicked, this, &QMain::editField);
    connect(ui->listFields, &QTreeWidget::currentItemChanged, this, &QMain::updateCurrentField);
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

    //Show main window
    updateMainScreenActive();
    this->setWindowState(Qt::WindowMaximized);
    this->show();
    QWarningDialog(this, tr("Information"), tr("Current activated farm: %1\n"
                                               "You can change the current farm with 'Settings' -> 'Farm'.").arg(CDataManager::getCurrentFarmName()), true, false).exec();
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
    }else{
        CField* field = CDataManager::getCurrentFarm()->getField(ui->listFields->currentItem()->text(0));
        for (int i = 5; i < 15; i++) {
            if(i < 10){
                //Crops
                if(field->getLastCrop(i - 5) != nullptr){
                    lbls[i]->setText(tr("Crop: ") + field->getLastCrop(i - 5)->getName());
                }
            }else{
                //InterCrops
                if(field->getLastInterCrop(i - 10) != nullptr){
                    lbls[i]->setText(tr("Inter Crop: ") + field->getLastInterCrop(i - 10)->getName());
                }
            }
        }
        lbls[15]->setText(field->getName());
        lbls[16]->setText(QString::number(field->getSize()));
        lbls[17]->setText(field->getRegNumber());
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


//--------------------------------- Events ---------------------------------------------------

void QMain::closeEvent(QCloseEvent* ev){
    //Ask if really want to exit program
    QWarningDialog dialog(this, tr("Do you really want to exit?"), tr("Do you really want to exit?"), true, true);
    if(dialog.exec()){
        //Save and exit
        save();
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

