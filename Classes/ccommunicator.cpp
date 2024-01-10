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
    CDataManager::setHarvestYearEnd(CFileManager::getHarvestYearEnd());
    CDataManager::setLoadYearsBefore(CFileManager::getLoadYearsBefore());

    CDataManager::loadFarmNames();

    CLanguageManager::init();

    return;
}

void CCommunicator::saveData(){
    CFileManager::checkDirs();
    //TODO: Save all Data to workDir
    CFileManager::writeFarmData();

    //Save all configs
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

QString CCommunicator::exportFieldData(QString name){
    return CFileManager::exportFieldData(*CDataManager::getCurrentFarm()->getField(name), QDir(QDir::homePath() + "/LVP-Data"));
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

void CCommunicator::setLoadYearsBefore(int to){
    CDataManager::setLoadYearsBefore(to);
}

QDate CCommunicator::getCurrentHarvestYearBegin(){
    QDate current = QDate::currentDate();
    bool first = current.month() > CDataManager::getHarvestYearEnd();
    if(first){
        //1.MONTH.YEAR
        return QDate(current.year(), CDataManager::getHarvestYearEnd(), 1);
    }else{
        //1.MONTH.YEAR-1
        return QDate(current.year() - 1, CDataManager::getHarvestYearEnd(), 1);
    }
}
