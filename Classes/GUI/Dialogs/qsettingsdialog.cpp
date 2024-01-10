#include "qsettingsdialog.h"
#include "ui_qsettingsdialog.h"

QSettingsDialog::QSettingsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QSettingsDialog)
{
    ui->setupUi(this);

    connect(&CDataManager::instance(), &CDataManager::currentFarmChanged, this, &QSettingsDialog::updateEditTab);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &QSettingsDialog::editCrop);

    //Init language menu
    createLanguageMenu();

    //Init lists
    createCropList();
    createFarmList();

    ui->cbHarvestYearEnd->setCurrentIndex(CDataManager::getHarvestYearEnd() - 1);
    ui->spinBox->setValue(CDataManager::getLoadYearsBefore());

    //Autofill edit fields
    ui->editName->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_SPACERS, ui->editName));
    ui->editName->setText(CDataManager::getCurrentFarmName());
}

//-------------------------------------- Tab farms ----------------------------------------------

void QSettingsDialog::createFarmList(){
    ui->listWidgetFarms->clear();
    QString currName = CDataManager::getCurrentFarmName();
    std::vector<QString> farmNames = CDataManager::getAllFarmNames();
    foreach (QString name, farmNames) {
        if(currName == name){
            name.append(tr(" (active farm)"));
        }
        new QListWidgetItem(name, ui->listWidgetFarms);
    }
}

void QSettingsDialog::delFarm(){
    //Delete selected farm
    if(ui->listWidgetFarms->currentItem() != nullptr){
        //Check if it isnt open
        QString name = ui->listWidgetFarms->currentItem()->text();
        if(name.contains("(")){
            name.truncate(name.indexOf("(") - 1);
        }
        if(name != CDataManager::getCurrentFarmName()){
            QWarningDialog warn(this, tr("Delete Farm"), tr("Do you really want to delete farm '%1'?").arg(name), true, true);
            if(warn.exec()){
                //Delete farm
                CCommunicator::deleteFarm(name);
                //Update listWidget
                delete ui->listWidgetFarms->takeItem(ui->listWidgetFarms->row(ui->listWidgetFarms->currentItem()));
            }
        }else{
            QWarningDialog w(this, tr("Error!"), tr("Selected farm is active! Please close farm before deleting it!"), true, false);
            w.exec();
        }
    }
}

void QSettingsDialog::newFarm(){
    //Create new farm through dialog
    QCreateFarm dia(this);
    dia.exec();
    createFarmList();
}

void QSettingsDialog::closeFarm(){
    CCommunicator::closeFarm();
    createFarmList();
}

void QSettingsDialog::loadFarm(){
    if(ui->listWidgetFarms->currentItem() != nullptr){
        QString name = ui->listWidgetFarms->currentItem()->text();
        if(name.contains("(")){
            name.truncate(name.indexOf("(") - 1);
        }
        if(name != CDataManager::getCurrentFarmName()){
            //Close open Farm
            if(CDataManager::getCurrentFarmName() != ""){
                CCommunicator::closeFarm();
            }
            //Load farm
            if(!CCommunicator::loadFarm(name)){
                QWarningDialog w(this, tr("Error"), tr("Unable to load farm! Try again!"), true, false);
                w.exec();
            }else{
                createFarmList();
            }
        }
    }
}

void QSettingsDialog::openFarm(){
    //Open file chooser dialog
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select farm directory"), "/home", QFileDialog::ShowDirsOnly);
    //Check if it contains .lvp file
    QDir farmDir(dir);
    if(farmDir.entryList().contains(farmDir.dirName() + ".lvp")){
        //Read it, add it to list, load it
        if(CCommunicator::openFarm(farmDir)){
            createFarmList();
            return;
        }
    }
    //Error invalid folder
    QWarningDialog(this, 0, tr("Invalid farm folder! Please try again!"), true, false).exec();

}

void QSettingsDialog::updateEditTab(){
    if(CDataManager::getCurrentFarmName() != ""){
        ui->editCurFarm->setEnabled(true);
        ui->editName->setText(CDataManager::getCurrentFarmName());
    }else{
        ui->editCurFarm->setEnabled(false);
        ui->editName->setText("");
    }
}

//@Override
void QSettingsDialog::done(int res){
    if(res == QDialog::Accepted){

        //------------ Farms tab ---------------------------------
        //Check if theres farm to edit
        if(CDataManager::getCurrentFarmName() != ""){
            QString name = ui->editName->text();
            if(!name.isEmpty()){
                if(name != CDataManager::getCurrentFarmName()){
                    //Update Farm
                    CFarm* toUpdate = CDataManager::getCurrentFarm();
                    toUpdate->setFarmName(name);
                    CCommunicator::reloadFarm();
                }
            }else{
                ui->statusLabel->setText(tr("All input fields need to be filled!"));
                return;
            }
        }

        //------------ General tab ----------------------------------
        CCommunicator::setHarvestYearEnd(ui->cbHarvestYearEnd->currentIndex() + 1);
        CCommunicator::setLoadYearsBefore(ui->spinBox->value());

    }
    QDialog::done(res);
}


//-------------------------------------- Tab crops ----------------------------------------------

void QSettingsDialog::createCropList(){
    std::vector<CCrop> crops = CDataManager::getAllCrops();
    foreach (CCrop crop, crops) {
        new QListWidgetItem(crop.getName(), ui->listWidget);
    }
}

void QSettingsDialog::delCrop(){
    //Delete selected crop
    if(ui->listWidget->currentItem() != nullptr){
        QString name = ui->listWidget->currentItem()->text();
        for(CField x : CDataManager::getCurrentFarm()->getAllFields()){
            if(x.usesCrop(name)){
                QWarningDialog(this, tr("Error!"), tr("Crop '%1' is used at field '%2'!\n"
                                                      "You could edit this crop instead of deleting it!").arg(name, x.getName())).exec();
                return;
            }
        }
        QWarningDialog warn(this, tr("Delete Crop"), tr("Do you really want to delete crop '%1'?\n"
                                                        "It will be deleted for all farms!").arg(name), true, true);
        if(warn.exec()){
            //Delete crop
            CCommunicator::deleteCrop(name);
            //Update listWidget
            delete ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
        }
    }
}

void QSettingsDialog::editCrop(){
    //Edit selected crop through dialog
    if(ui->listWidget->currentItem() != nullptr){
        QString name = ui->listWidget->currentItem()->text();
        QEditCrop edit(this, name);
        edit.exec();
    }
}

void QSettingsDialog::newCrop(){
    //Create new crop through dialog
    QEditCrop createDia(this, true);
    createDia.exec();
}

//-------------------------- Languages ------------------------------------

//Create menu entries depending on existing translation files
void QSettingsDialog::createLanguageMenu(){

    connect(ui->languageBox, SIGNAL (currentIndexChanged(int)), this->parent(), SLOT (slotLangBoxChanged(int)));

    ui->languageBox->clear();

    QList<QString> entries = CLanguageManager::getMenuEntries();
    for (int i = 0; i < entries.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = entries[i]; // e.g. "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        // QIcon ico(QString("%1/%2.png").arg(CDataManager::getLangPath()).arg(locale));

        ui->languageBox->addItem(lang, locale);

        // set default translators and language active
        if (CLanguageManager::getDefaultLocale() == locale) {
            ui->languageBox->setCurrentIndex(i);
        }
    }
}

// Run time translation
void QSettingsDialog::changeEvent(QEvent * e){
    if(e->type() == QEvent::LanguageChange) {
        // this event is send if a translator is loaded
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(e);
}

QSettingsDialog::~QSettingsDialog()
{
    delete ui;
}

