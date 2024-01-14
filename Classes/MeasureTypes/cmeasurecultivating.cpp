#include "cmeasurecultivating.h"

CMeasureCultivating::CMeasureCultivating(QString field, QString string) : CMeasure(field, string)
{
    //Split string and start at value NUM_ATTRIBUTES
}

CMeasureCultivating::CMeasureCultivating(QString field, int state, QDate date, int type) : CMeasure(field, state, date, type)
{
    //Additional attributes
}
