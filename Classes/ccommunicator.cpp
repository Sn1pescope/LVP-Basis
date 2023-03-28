#include "ccommunicator.h"

CCommunicator::CCommunicator()
{

}


//------------------- Loading and saving -------------------------

void CCommunicator::initialize(){
    //TODO: Load all settings and configs
    CDataManager::clearFarms();
    CFileManager::initialize();

    //Configs
    CDataManager::loadCrops();
    CDataManager::loadFarmNames();
    CDataManager::setHarvestYearEnd(CFileManager::getHarvestYearEnd());

    CLanguageManager::init();

    return;
}

void CCommunicator::saveData(){
    CFileManager::checkDirs();
    //TODO: Save all Data to workDir
    CFileManager::writeFarmData();

    //TODO: Save all configs
    CFileManager::writeConfFile();
    CFileManager::writeCropFile();

    //Save last farm
    CFileManager::writeTmpFile();
}


//------------------ Farms --------------------------------

void CCommunicator::createNewFarm(QString name){
    CDataManager::addFarm(name);
}

bool CCommunicator::loadFarm(int at){
    if(CDataManager::loadFarm(at)){
        return true;
    }else{
        return false;
    }
}

bool CCommunicator::loadFarm(QString at){
    if(CDataManager::loadFarm(at)){
        return true;
    }else{
        return false;
    }
}

bool CCommunicator::tryAutoload(){
    //Load last farm
    if(CDataManager::tryAutoload()){
        return true;
    }else{
        return false;
    }
}

void CCommunicator::closeFarm(){
    //Save
    saveData();
    // Unload farm
    CDataManager::unloadCurFarm();
}

void CCommunicator::deleteFarm(QString name){
    CDataManager::deleteFarm(name);
    saveData();
}

void CCommunicator::reloadFarm(){
    CDataManager::reloadFarm();
}

bool CCommunicator::openFarm(QDir dir){
    QString farmName = CFileManager::openFarmData(dir);
    if(farmName != ""){
        CDataManager::addFarm(farmName);
        loadFarm(farmName);
        return true;
    }
    return false;
}


//-------------------- Fields -------------------------------

void CCommunicator::deleteField(QString name){
    CDataManager::getCurrentFarm()->deleteField(name);
}

void CCommunicator::deleteFieldOfCurrentFarm(QString name){
    deleteField(name);
    saveData();
}


//-------------------- General settings ------------------------

void CCommunicator::deleteCrop(QString name){
    CDataManager::deleteCrop(name);
    saveData();
}

QString CCommunicator::generateHarvestYearString(int yearsBack){
    QDate current = QDate::currentDate();
    int year = current.year() - yearsBack;
    bool first = current.month() > CDataManager::getHarvestYearEnd();
    QString harvestYear = tr("Harvest year: ");
    if(first){
        //Format year/year+1
        harvestYear.append(QString::number(year) + "/" + QString::number(year + 1));
    }else{
        // Format year-1/year
        harvestYear.append(QString::number(year - 1) + "/" + QString::number(year));
    }
    return harvestYear;
}

void CCommunicator::setHarvestYearEnd(int month){
    CDataManager::setHarvestYearEnd(month);
}
