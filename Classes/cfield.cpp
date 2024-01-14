#include "cfield.h"
#include "Classes/cdatamanager.h"
#include "Classes/ccommunicator.h"
#include <unordered_set>

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
    int numPos = CFileManager::getNumberOfDataFiles(name, work);
    int max = CDataManager::getLoadYearsBefore();
    int toLoad;
    if(numPos < max){
        toLoad = numPos;
    }else{
        toLoad = max;
    }
    for(int i = 0; i < toLoad; i++){
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

        //Get measures
        if(data.contains("measures")){
            QString valString = data.value("measures");
            if(valString != "NULL"){
                QStringList meas = valString.split(QLatin1Char(';'));
                foreach(QString measure, meas){
                    int type = measure.split(QLatin1Char(' ')).at(0).toInt();
                    switch (type) {
                    case 0:
                        //Cultivating constructor
                        measures.push_back(QSharedPointer<CMeasure>(new CMeasureCultivating(name, measure)));
                        break;
                    case 1:
                        //Seeding constructor
                        measures.push_back(QSharedPointer<CMeasure>(new CMeasureSeeding(name, measure)));
                        break;
                    case 2:
                        //Fertilizing constructor
                        measures.push_back(QSharedPointer<CMeasure>(new CMeasureFertilizing(name, measure)));
                        break;
                    case 3:
                        //Spraying constructor
                        measures.push_back(QSharedPointer<CMeasure>(new CMeasureSpraying(name, measure)));
                        break;
                    case 4:
                        //Harvesting constructor
                        measures.push_back(QSharedPointer<CMeasure>(new CMeasureHarvesting(name, measure)));
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        //More data...
    }
    std::sort(measures.begin(), measures.end(), CMeasure::greater);
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

void CField::addMeasure(QSharedPointer<CMeasure> measure){
    measures.push_back(measure);
    //Sort after date; earliest date first
    std::sort(measures.begin(), measures.end(), CMeasure::greater);
}

void CField::removeMeasure(QString key){
    for (int i = 0; i < measures.size(); ++i) {
        if(measures.at(i)->getKey() == key){
            measures.erase(measures.begin() + i);
        }
    }
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

    //Last Crops and interCrops: Can also be empty -------------------------------
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

    //Get measures for year -------------------------------------------------------
    QDate begin = CCommunicator::getCurrentHarvestYearBegin();
    //Substrate years back
    begin = begin.addYears(-yearsBack);
    std::vector<QSharedPointer<CMeasure>> meas = getMeasuresInRange(begin.addYears(-yearsBack), begin.addYears(1).addDays(-1), (yearsBack == 0));
    QString measuresString = "";
    for(int i = 0; i < (int)meas.size(); i++){
        //Get its values and put it in saving form
        if(i != 0){
            measuresString += ";";
        }
        measuresString += meas.at(i)->getSavingData();
    }
    if(measuresString.isEmpty()){
        measuresString = "NULL";
    }
    data.insert(QString::fromStdString("measures"), measuresString);

    //More data... ----------------------------------------------------------------

    return data;
}

QSharedPointer<CMeasure> CField::getMeasure(QString key){
    for(QSharedPointer<CMeasure> meas : measures){
        if(meas->getKey() == key){
            return meas;
        }
    }
    return nullptr;
}

std::vector<QSharedPointer<CMeasure>> CField::getMeasuresInRange(QDate from, QDate to, bool allFuture){
    //Sort range
    QDate first;
    QDate last;
    if(from > to){
        first = to;
        last = from;
    }else{
        first = from;
        last = to;
    }
    std::vector<QSharedPointer<CMeasure>> ret;
    for(int i = 0; i < (int)measures.size(); i++){
        //Check if its already in range
        if(measures.at(i)->getDate() < first){
            continue;
        }
        if(!allFuture){
            if(measures.at(i)->getDate() > last){
                //Break
                break;
            }
        }
        //Put in vector and return
        ret.push_back(measures.at(i));
    }
    std::sort(ret.begin(), ret.end(), CMeasure::greater);
    return ret;
}

std::vector<QSharedPointer<CMeasure>> CField::getMeasuresOfType(int type){
    std::vector<QSharedPointer<CMeasure>> ret;
    for(int i = 0; i < (int)measures.size(); i++){
        if(measures.at(i)->getType() == type){
            ret.push_back(measures.at(i));
        }
    }
    return ret;
}

std::vector<QSharedPointer<CMeasure>> CField::getMeasuresOfState(int state){
    std::vector<QSharedPointer<CMeasure>> ret;
    for(int i = 0; i < (int)measures.size(); i++){
        if(measures.at(i)->getState() == state){
            ret.push_back(measures.at(i));
        }
    }
    return ret;
}

std::vector<QSharedPointer<CMeasure>> CField::checkMeasureFilters(int state, int type, QDate date, bool before, bool onlyDay){
    if(state == -1 && type == -1 && !date.isValid()){
        return measures;
    }

    bool dateFilter = date.isValid();
    bool stateFilter = state != -1;
    bool typeFilter = type != -1;

    std::vector<QSharedPointer<CMeasure>> dateMeasures;
    std::vector<QSharedPointer<CMeasure>> stateMeasures;
    std::vector<QSharedPointer<CMeasure>> typeMeasures;
    if(dateFilter){
        if(before){
            QDate b;
            if(onlyDay){
                b = date;
            }else{
                b = QDate::fromString("01.01.2000", "dd.MM.yyyy");
            }
            dateMeasures = getMeasuresInRange(b, date);
        }else{
            dateMeasures = getMeasuresInRange(date, date, true);
        }
    }
    if(stateFilter){
        stateMeasures = getMeasuresOfState(state);
    }
    if(typeFilter){
        typeMeasures = getMeasuresOfType(type);
    }

    std::vector<QSharedPointer<CMeasure>> ret;
    for(QSharedPointer<CMeasure> m : measures){
        if(dateFilter){
            //Has date filter
            if(std::find(dateMeasures.begin(), dateMeasures.end(), m) != dateMeasures.end()){
                //Is in date filter
                if(stateFilter){
                    //Has state filter
                    if(std::find(stateMeasures.begin(), stateMeasures.end(), m) != stateMeasures.end()){
                        //Is in state filter
                        if(typeFilter){
                            //Has type filter
                            if(std::find(typeMeasures.begin(), typeMeasures.end(), m) != typeMeasures.end()){
                                //is in type filter
                                ret.push_back(m);
                                continue;
                            }else{
                                //Not in type filter
                                continue;
                            }
                        }else{
                            //Has no type filter
                            ret.push_back(m);
                            continue;
                        }
                    }else{
                        //Not in state filter
                        continue;
                    }
                }else if(typeFilter){
                    //Has type filter
                    if(std::find(typeMeasures.begin(), typeMeasures.end(), m) != typeMeasures.end()){
                        //Is in type filter
                        ret.push_back(m);
                        continue;
                    }else{
                        //Not in type
                        continue;
                    }
                }else{
                    //Has no state and type filter
                    ret.push_back(m);
                    continue;
                }
            }else{
                //Not in date filter
                continue;
            }
        }else if(stateFilter){
            //Has state filter, no date filter
            if(std::find(stateMeasures.begin(), stateMeasures.end(), m) != stateMeasures.end()){
                //Is in state filter
                if(typeFilter){
                    //Has type filter
                    if(std::find(typeMeasures.begin(), typeMeasures.end(), m) != typeMeasures.end()){
                        //is in type filter
                        ret.push_back(m);
                        continue;
                    }else{
                        //Not in type filter
                        continue;
                    }
                }else{
                    //Has no type filter
                    ret.push_back(m);
                    continue;
                }
            }else{
                //Not in state filter
                continue;
            }
        }else if(typeFilter){
            //Has type filter, no date and state filter
            if(std::find(typeMeasures.begin(), typeMeasures.end(), m) != typeMeasures.end()){
                //Is in type filter
                ret.push_back(m);
                continue;
            }else{
                //Not in filter
                continue;
            }
        }else{
            //Has no filter
            ret = measures;
            break;
        }
    }
    return ret;
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
