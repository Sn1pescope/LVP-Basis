#include "cmeasurespraying.h"

CMeasureSpraying::CMeasureSpraying(QString field, QString val) : CMeasure(field, val)
{
    //Split string and start at value NUM_ATTRIBUTES
}

CMeasureSpraying::CMeasureSpraying(QString field, int state, QDate date, int type) : CMeasure(field, state, date, type)
{
    //Additional attributes
}
