#include "cworkunit.h"

CWorkUnit::CWorkUnit(QString name_par)
{
    name = name_par;
}

void CWorkUnit::addField(QString name){
    fields.push_back(name);
}

void CWorkUnit::removeField(QString name){
    auto it = std::find(fields.begin(), fields.end(), name);
    if(it != fields.end()){
        removeField(it-fields.begin());
    }
}

void CWorkUnit::removeField(int at){
    fields.erase(fields.begin() + at);
}

bool CWorkUnit::hasField(QString name){
    auto it = std::find(fields.begin(), fields.end(), name);
    return (it != fields.end());
}

bool CWorkUnit::isEmpty(){
    return (fields.size() < 1);
}

std::vector<QString> CWorkUnit::getAllAttributes(){
    std::vector<QString> data;
    data.push_back(name);
    foreach(QString f, fields){
        data.push_back(f);
    }
    return data;
}

QString CWorkUnit::getName(){
    return name;
}

std::vector<QString> CWorkUnit::getAllFieldNames(){
    return fields;
}

bool CWorkUnit::operator==(const CWorkUnit rhs){
    return (rhs.name == this->name);
}
