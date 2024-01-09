#include "cmeasure.h"

CMeasure::CMeasure()
{

}

CMeasure::CMeasure(int state_par, QDate date_par, int type_par){
    state = state_par;
    date = date_par;
    type = type_par;
}

//-------- Getter -----------

QDate CMeasure::getDate(){
    return date;
}

int CMeasure::getState(){
    return state;
}

int CMeasure::getType(){
    return type;
}

//--------- Setter ------------

void CMeasure::setDate(QDate to){
    date = to;
}

void CMeasure::setState(int to){
    state = to;
}

void CMeasure::setType(int to){
    type = to;
}

bool CMeasure::operator<(CMeasure &rhs){
    return this->date < rhs.date;
}
