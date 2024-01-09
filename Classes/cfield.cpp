#include "cfield.h"
#include "Classes/cdatamanager.h"

CField::CField(){

}

CField::CField(QString name_par){
    name = name_par;
}

CField::CField(QString name_par, float size_par, QString regNumber_par, QString nextCrop_par, QString nextInterCrop_par, bool cropPlanted_par, bool interCropPlanted_par, QDir work)
{
    name = name_par;
    size = size_par;
    regNumber = regNumber_par;

    nextCrop = CDataManager::getCrop(nextCrop_par);
    nextInterCrop = CDataManager::getCrop(nextInterCrop_par);
    cropPlanted = cropPlanted_par;
    interCropPlanted = interCropPlanted_par;

    //Load fieldData
    for(int i = 0; i < CFileManager::getNumberOfDataFiles(name, work); i++){
        QMap<QString, QString> data = CFileManager::getFieldData(name, i, work);

        //Crops
        if(i < 5){
            QString crop = "NULL";
            QString inter = "NULL";
            if(data.contains("crop")){
                crop = data.value("crop");
            }
            if(data.contains("interCrop")){
                inter = data.value("interCrop");
            }
            lastCrops[i] = CDataManager::getCrop(crop);
            lastInterCrops[i] = CDataManager::getCrop(inter);
        }

        //More data...
    }
}

CField::CField(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop, QString nextInterCrop, bool cropPlanted_par, bool interCropPlanted_par)
{
    setAllParameters(name_par, size_par, regNumber_par, lastCrop1, lastCrop2, lastCrop3, lastCrop4, lastCrop5, lastInterCrop1, lastInterCrop2, lastInterCrop3, lastInterCrop4, lastInterCrop5, nextCrop, nextInterCrop, cropPlanted_par, interCropPlanted_par);
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

CCrop* CField::getNextCrop(){
    return nextCrop;
}

CCrop* CField::getNextInterCrop(){
    return nextInterCrop;
}

bool CField::isCropPlanted(){
    return cropPlanted;
}

bool CField::isInterCropPlanted(){
    return interCropPlanted;
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

std::vector<QString> CField::getAllEssentialAttributes(){
    std::vector<QString> data;
    //Add all attributes to save
    data.push_back(getName());
    data.push_back(QString::number(getSize()));
    data.push_back(getRegNumber());

    //Can also be empty
    if(getNextCrop() != 0){
        data.push_back(getNextCrop()->getName());
    }else{
        data.push_back("NULL");
    }
    if(getNextInterCrop() != 0){
        data.push_back(getNextInterCrop()->getName());
    }else{
        data.push_back("NULL");
    }
    data.push_back(QString::number(cropPlanted));
    data.push_back(QString::number(interCropPlanted));

    return data;
}

QMap<QString, QString> CField::getFieldData(int yearsBack){
    QMap<QString, QString> data;

    //Last Crops and interCrops: Can also be empty
    QString crop;
    QString interCrop;
    if(getLastCrop(yearsBack) != 0){
        crop = getLastCrop(yearsBack)->getName();
    }else{
        crop = "NULL";
    }
    if(getLastInterCrop(yearsBack) != 0){
        interCrop = getLastInterCrop(yearsBack)->getName();
    }else{
        interCrop = "NULL";
    }
    data.insert(QString::fromStdString("crop"), crop);
    data.insert(QString::fromStdString("interCrop"), interCrop);


    //More data...

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

void CField::setNextCrop(CCrop* newCrop){
    nextCrop = newCrop;
}

void CField::setNextInterCrop(CCrop* newInterCrop){
    nextInterCrop = newInterCrop;
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

void CField::setAllParameters(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop_par, QString nextInterCrop_par, bool cropPlanted_par, bool interCropPlanted_par){
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
    nextCrop = CDataManager::getCrop(nextCrop_par);
    nextInterCrop = CDataManager::getCrop(nextInterCrop_par);
    cropPlanted = cropPlanted_par;
    interCropPlanted = interCropPlanted_par;
}
