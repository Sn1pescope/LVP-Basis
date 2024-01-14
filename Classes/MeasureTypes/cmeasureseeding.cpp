#include "cmeasureseeding.h"

CMeasureSeeding::CMeasureSeeding(QString field, QString val) : CMeasure(field, val)
{
    //Split string and start at value NUM_ATTRIBUTES
}

CMeasureSeeding::CMeasureSeeding(QString field, int state, QDate date, int type) : CMeasure(field, state, date, type)
{
    //Additional attributes
}
