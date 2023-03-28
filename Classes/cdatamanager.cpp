#include "cdatamanager.h"

std::vector <CFarm>CDataManager::farms;
std::vector <CCrop>CDataManager::crops;
int CDataManager::activeFarm = -1;
QString CDataManager::farmName;
QDir CDataManager::workDir;
int CDataManager::harvestYearEnd;
//STANDARD VALUES
int CDataManager::STANDARD_HARVEST_YEAR_END = 8;

CDataManager::CDataManager()
{

}


void CDataManager::addFarm(QString farmName){
    CFarm farm = CFarm(farmName);
    farms.push_back(farm);
}

bool CDataManager::loadFarm(int at){
    activeFarm = at;
    farmName = farms.at(activeFarm).getFarmName();
    workDir = farms.at(activeFarm).getWorkDir();
    emit instance().currentFarmChanged();
    return true;
}

bool CDataManager::loadFarm(QString at){
    if(at == "LAST"){
        loadFarm(farms.size() - 1);
        return true;
    }else if(at == "FIRST"){
        loadFarm(0);
        return true;
    }else if(at == ""){
        return false;
    }else{
        auto it = std::find(farms.begin(), farms.end(), CFarm(at, false));
        if(it != farms.end()){
            loadFarm(it - farms.begin());
            return true;
        }
    }
    return false;
}

bool CDataManager::tryAutoload(){
    return loadFarm(CFileManager::getLastFarmName());
}

void CDataManager::loadFarmNames(){
    std::vector<QString> farmNames = CFileManager::getFarmNames();
    if(!farmNames.empty()){
        for(int i = 0; i < (int)farmNames.size(); i++){
            farms.push_back(CFarm(farmNames.at(i)));
        }
    }
}

void CDataManager::unloadCurFarm(){
    activeFarm = -1;
    farmName = "";
    workDir.setPath(0);
    emit instance().currentFarmChanged();
}

void CDataManager::clearFarms(){
    farms.clear();
}

void CDataManager::deleteFarm(int at){
    QString name;
    if(activeFarm != -1){
        name = farms.at(activeFarm).getFarmName();
    }
    QDir dir = farms.at(at).getWorkDir();
    dir.removeRecursively();
    farms.erase(farms.begin() + at);
    if(activeFarm != -1){
        auto it = std::find(farms.begin(), farms.end(), CFarm(name, false));
        if(it != farms.end()){
            activeFarm = (it - farms.begin());
        }
    }
}

void CDataManager::deleteFarm(QString at){
    auto it = std::find(farms.begin(), farms.end(), CFarm(at, false));
    if(it != farms.end()){
        deleteFarm(it - farms.begin());
    }
}

void CDataManager::reloadFarm(){
    loadFarm(activeFarm);
}

void CDataManager::addCrop(CCrop crop){
    crops.push_back(crop);
}

void CDataManager::loadCrops(){
    std::vector<QStringList> cropData = CFileManager::getCropData();
    if(!cropData.empty()){
        for(int i = 0; i < (int)cropData.size(); i++){
            crops.push_back(CCrop(cropData.at(i).at(0)));
        }
    }
}

void CDataManager::deleteCrop(int at){
    crops.erase(crops.begin() + at);
}

void CDataManager::deleteCrop(QString at){
    auto it = std::find(crops.begin(), crops.end(), CCrop(at));
    if(it != crops.end()){
        deleteCrop(it - crops.begin());
    }
}


//--------------------------- Getter ----------------------------------

QString CDataManager::getCurrentFarmName(){
    return farmName;
}

QDir CDataManager::getCurrentWorkDir(){
    return workDir;
}

std::vector<QString> CDataManager::getAllFarmNames(){
    std::vector<QString> names;
    for(int i = 0; i < (int)farms.size(); i++){
        names.push_back(farms.at(i).getFarmName());
    }
    return names;
}

std::vector<CCrop> CDataManager::getAllCrops(){
    return crops;
}

QStringList CDataManager::getAllCropNames(){
    QStringList names;
    for(CCrop crop : crops){
        names.append(crop.getName());
    }
    return names;
}

CFarm* CDataManager::getCurrentFarm(){
    if(activeFarm == -1){
        return 0;
    }else{
        CFarm* farm = &farms.at(activeFarm);
        return farm;
    }
}

CFarm* CDataManager::getFarm(int at){
    CFarm* farm = &farms.at(at);
    return farm;
}

CFarm* CDataManager::getFarm(QString at){
    auto it = std::find(farms.begin(), farms.end(), CFarm(at, false));
    if(it != farms.end()){
        return &farms.at(it - farms.begin());
    }
    return 0;
}

CCrop* CDataManager::getCrop(QString name){
    auto it = std::find(crops.begin(), crops.end(), CCrop(name));
    if(it != crops.end()){
        return &crops.at(it - crops.begin());
    }
    return 0;
}

int CDataManager::getHarvestYearEnd(){
    return harvestYearEnd;
}


//-------------------------- Setter ------------------------------------

void CDataManager::setFarmName(int at, QString to){
    farms.at(at).setFarmName(to);
}

void CDataManager::setFarmName(QString at, QString to){
    auto it = std::find(farms.begin(), farms.end(), CFarm(at, false));
    if(it != farms.end()){
        setFarmName(it - farms.begin(), to);
    }
}

void CDataManager::setHarvestYearEnd(int to){
    if(to > 0 && to <= 12){
        harvestYearEnd = to;
    }
}
