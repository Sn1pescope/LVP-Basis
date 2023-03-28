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

    std::vector<QStringList> farmData = CFileManager::getFarmData(workDir, farmName);
    if(!farmData.empty()){
        for(int i = 0; i < (int)farmData.size(); i++){
            QStringList l = farmData.at(i);
            fields.push_back(CField(l.at(0), l.at(1).toFloat(), l.at(2), l.at(3), l.at(4), l.at(5), l.at(6), l.at(7), l.at(8), l.at(9), l.at(10), l.at(11), l.at(12)));
        }
    }
}

void CFarm::addField(CField toAdd){
    fields.push_back(toAdd);
}

void CFarm::deleteField(int at){
    fields.erase(fields.begin() + at);
}

void CFarm::deleteField(QString at){
    auto it = std::find(fields.begin(), fields.end(), CField(at));
    if(it != fields.end()){
        deleteField(it - fields.begin());
    }
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
