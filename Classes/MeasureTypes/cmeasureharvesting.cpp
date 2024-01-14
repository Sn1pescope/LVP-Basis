#include "cmeasureharvesting.h"

CMeasureHarvesting::CMeasureHarvesting(QString field, QString val) : CMeasure(field, val)
{
    //Split string and start at value NUM_ATTRIBUTES
}

CMeasureHarvesting::CMeasureHarvesting(QString field, int state, QDate date, int type) : CMeasure(field, state, date, type)
{
    //Additional attributes
}
