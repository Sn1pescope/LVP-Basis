#include "cmeasure.h"

const QMap<int, QString> CMeasure::STATES{{STATE_PLANNED, tr("Planned")},
                                          {STATE_IN_PROGRESS, tr("In Progress")}, {STATE_DONE, tr("Done")}};
const QMap<int, QString> CMeasure::TYPES{{TYPE_CULTIVATING, tr("Cultivating")}, {TYPE_SEEDING, tr("Seeding")},
                                         {TYPE_FERTILIZING, tr("Fertilizing")}, {TYPE_SPRAYING, tr("Spraying")},
                                         {TYPE_HARVESTING, tr("Harvesting")}};

CMeasure::CMeasure(QString savingString)
{
    QStringList l = savingString.split(QLatin1Char(' '));
    type = l.at(0).toInt();
    state = l.at(1).toInt();
    date = QDate::fromString(l.at(2), "dd.MM.yyyy");
}

CMeasure::CMeasure(int state_par, QDate date_par, int type_par){
    state = state_par;
    date = date_par;
    type = type_par;
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
