#include "cmeasure.h"

const QMap<int, QString> CMeasure::STATES{{0, tr("Planned")},
                                          {1, tr("In Progress")}, {2, tr("Done")}};
const QMap<int, QString> CMeasure::TYPES{{0, tr("Cultivating")}, {1, tr("Seeding")},
                                         {2, tr("Fertilizing")}, {3, tr("Spraying")},
                                         {4, tr("Harvesting")}};

CMeasure::CMeasure(QString field_par, QString savingString)
{
    QStringList l = savingString.split(QLatin1Char(' '));
    type = l.at(0).toInt();
    state = l.at(1).toInt();
    date = QDate::fromString(l.at(2), "dd.MM.yyyy");
    field = field_par;
}

CMeasure::CMeasure(QString field_par, int state_par, QDate date_par, int type_par){
    state = state_par;
    date = date_par;
    type = type_par;
    field = field_par;
}

CMeasure::~CMeasure(){
    //Do cleanup
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

QString CMeasure::getSavingData(){
    return QString::number(type) + " " + QString::number(state) + " " + date.toString("dd.MM.yyyy");
}

QString CMeasure::getField(){
    return field;
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
