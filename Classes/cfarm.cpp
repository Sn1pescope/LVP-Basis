#include "cfarm.h"

CFarm::CFarm(QString farmName_par)
{
    ini(farmName_par);
}

CFarm::CFarm(QString farmName_par, bool createStuff)
{
    if(createStuff){
        ini(farmName_par);
    }else{
        farmName = farmName_par;
    }
}

void CFarm::ini(QString farmName_par){
    farmName = farmName_par;
    workDir = QDir(QString::fromStdString("Data/") + farmName);

    QMap<QString, QStringList> farmData = CFileManager::getFarmData(workDir, farmName);

    if(!farmData.empty()){
        //Fields
        if(farmData.contains("fieldData")){
            QStringList fieldData = farmData.value("fieldData");
            foreach(QString field, fieldData){
                QStringList l = field.split(QLatin1Char(' '));
                //Check length and append empty values
                int needed = 17 - l.length();
                for(int i = 0; i < needed; i++){
                    l.push_back("NULL");
                }
                bool one = l.at(15) == "1";
                bool two = l.at(16) == "1";

                fields.push_back(CField(l.at(0), l.at(1).toFloat(), l.at(2), l.at(3), l.at(4), l.at(5), l.at(6), l.at(7), l.at(8), l.at(9), l.at(10), l.at(11), l.at(12), l.at(13), l.at(14), one, two));
            }
        }
        //WorkUnits
        if(farmData.contains("workUnitData")){
            QStringList workUnitData = farmData.value("workUnitData");
            foreach(QString workUnit, workUnitData){
                QStringList l = workUnit.split(QLatin1Char(' '));
                QString name = l.at(0);
                workUnits.push_back(CWorkUnit(name));
                for(int i = 1; i < l.length(); i++){
                    addFieldToWorkUnit(l.at(i), l.at(0));
                }
            }
        }
    }
}

void CFarm::addField(CField toAdd){
    fields.push_back(toAdd);
}

void CFarm::deleteField(int at){
    QString wu = getWorkUnitOfField(fields.at(at).getName());
    if(wu != nullptr){
        //Remove field from wu
        removeFieldFromWorkUnit(fields.at(at).getName(), wu);
    }

    fields.erase(fields.begin() + at);
}

void CFarm::deleteField(QString at){
    auto it = std::find(fields.begin(), fields.end(), CField(at));
    if(it != fields.end()){
        deleteField(it - fields.begin());
    }
}

void CFarm::addWorkUnit(CWorkUnit wu){
    workUnits.push_back(wu);
}
void CFarm::deleteWorkUnit(int at){
    workUnits.erase(workUnits.begin() + at);
}
void CFarm::deleteWorkUnit(QString at){
    auto it = std::find(workUnits.begin(), workUnits.end(), CWorkUnit(at));
    if(it != workUnits.end()){
        deleteWorkUnit(it - workUnits.begin());
    }
}
void CFarm::addFieldToWorkUnit(QString field, QString wu){
    auto it = std::find(workUnits.begin(), workUnits.end(), CWorkUnit(wu));
    if(it != workUnits.end()){
        workUnits.at(it - workUnits.begin()).addField(field);
    }
}
void CFarm::removeFieldFromWorkUnit(QString field, QString wu){
    auto it = std::find(workUnits.begin(), workUnits.end(), CWorkUnit(wu));
    if(it != workUnits.end()){
        CWorkUnit* w = &workUnits.at(it - workUnits.begin());
        w->removeField(field);
        if(w->isEmpty()){
            deleteWorkUnit(it - workUnits.begin());
        }
    }
}
QString CFarm::getWorkUnitOfField(QString field){
    for(int i = 0; i < (int)workUnits.size(); i++){
        if(workUnits.at(i).hasField(field)){
            return workUnits.at(i).getName();
        }
    }
    return 0;
}

bool CFarm::operator==(const CFarm &rhs){
    return rhs.farmName == farmName;
}



//----------------- Getter ---------------------

QString CFarm::getFarmName(){
    return farmName;
}

QDir CFarm::getWorkDir(){
    return workDir;
}

std::vector<QString> CFarm::getAllFieldNames(){
    std::vector<QString> names;
    for(int i = 0; i < (int)fields.size(); i++){
        names.push_back(fields.at(i).getName());
    }
    return names;
}

std::vector<CField> CFarm::getAllFields(){
    return fields;
}

std::vector<CWorkUnit> CFarm::getAllWorkUnits(){
    return workUnits;
}

CField* CFarm::getField(int at){
    return &fields.at(at);
}

CField* CFarm::getField(QString at){
    auto it = std::find(fields.begin(), fields.end(), CField(at));
    if(it != fields.end()){
        return &fields.at(it - fields.begin());
    }
    return 0;
}


//----------------- Setter ---------------------

void CFarm::setFarmName(QString to){
    QString oldName = farmName;
    farmName = to;
    //Update folder
    QDir oldDir = workDir;
    workDir = QDir(QString::fromStdString("Data/") + farmName);
    CFileManager::updateFarmDir(oldDir, oldName, farmName);
}
