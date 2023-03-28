#include "ccrop.h"


CCrop::CCrop(){

}
CCrop::CCrop(QString name_par)
{
    name = name_par;
}

bool CCrop::operator==(const CCrop &rhs){
    return rhs.name == name;
}


//---------------------------- Setter ----------------------------------

void CCrop::setName(QString to){
    name = to;
}


//---------------------------- Getter -----------------------------------

QString CCrop::getName() const{
    return name;
}

std::vector<QString> CCrop::getAllAttributes(){
    std::vector<QString> data;
    //TODO: Add all attributes to save
    data.push_back(getName());

    return data;
}
