#include "cfield.h"
#include "Classes/cdatamanager.h"

CField::CField(){

}

CField::CField(QString name_par){
    name = name_par;
}

CField::CField(QString name_par, float size_par, QString regNumber_par)
{
    name = name_par;
    size = size_par;
    regNumber = regNumber_par;
}

CField::CField(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5)
{
    setAllParameters(name_par, size_par, regNumber_par, lastCrop1, lastCrop2, lastCrop3, lastCrop4, lastCrop5, lastInterCrop1, lastInterCrop2, lastInterCrop3, lastInterCrop4, lastInterCrop5);
}

bool CField::operator==(const CField &rhs){
    return rhs.name == name;
}


//------------------------------------ Methods -----------------------------------------
bool CField::usesCrop(QString name){
    for(int i=0; i < 5; i++){
        if(lastCrops[i] != nullptr && lastCrops[i]->getName() == name){
            return true;
        }
        if(lastInterCrops[i] != nullptr && lastInterCrops[i]->getName() == name){
            return true;
        }
    }
    return false;
}


//---------------- Getter ----------------------

CCrop* CField::getLastCrop(int at){
    return lastCrops[at];
}

CCrop* CField::getLastInterCrop(int at){
    return lastInterCrops[at];
}

QString CField::getName(){
    return name;
}

float CField::getSize(){
    return size;
}

QString CField::getRegNumber(){
    return regNumber;
}

std::vector<QString> CField::getAllAttributes(){
    std::vector<QString> data;
    //TODO: Add all attributes to save
    data.push_back(getName());
    data.push_back(QString::number(getSize()));
    data.push_back(getRegNumber());
    //Can also be empty
    for(int i = 0; i < 5; i++){
        if(getLastCrop(i) != 0){
            data.push_back(getLastCrop(i)->getName());
        }else{
            data.push_back("NULL");
        }
    }
    for(int i = 0; i < 5; i++){
        if(getLastInterCrop(i) != 0){
            data.push_back(getLastInterCrop(i)->getName());
        }else{
            data.push_back("NULL");
        }
    }

    return data;
}

//---------------- Setter ----------------------

void CField::setLastCrop(CCrop* newCrop){
    //Put every pointer one step further and add new Crop pointer
    int i = 0;
    while(lastCrops[i] != 0){
        if(i == 5){
            break;
        }
        lastCrops[i] = lastCrops[i+1];
        if(lastCrops[i] == 0){
            break;
        }
        i++;
    }
    lastCrops[i] = newCrop;
}

void CField::setLastInterCrop(CCrop* newInterCrop){
    //Put every pointer one step further and add new Crop pointer
    int i = 0;
    while(lastInterCrops[i] != 0){
        if(i == 5){
            break;
        }
        lastInterCrops[i] = lastInterCrops[i+1];
        if(lastInterCrops[i] == 0){
            break;
        }
        i++;
    }
    lastInterCrops[i] = newInterCrop;
}

void CField::setName(QString to){
    name = to;
}

void CField::setSize(float to){
    size = to;
}

void CField::setRegNumber(QString to){
    regNumber = to;
}

void CField::setAllParameters(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5){
    name = name_par;
    size = size_par;
    regNumber = regNumber_par;
    lastCrops[0] = CDataManager::getCrop(lastCrop1);
    lastCrops[1] = CDataManager::getCrop(lastCrop2);
    lastCrops[2] = CDataManager::getCrop(lastCrop3);
    lastCrops[3] = CDataManager::getCrop(lastCrop4);
    lastCrops[4] = CDataManager::getCrop(lastCrop5);
    lastInterCrops[0] = CDataManager::getCrop(lastInterCrop1);
    lastInterCrops[1] = CDataManager::getCrop(lastInterCrop2);
    lastInterCrops[2] = CDataManager::getCrop(lastInterCrop3);
    lastInterCrops[3] = CDataManager::getCrop(lastInterCrop4);
    lastInterCrops[4] = CDataManager::getCrop(lastInterCrop5);
}
