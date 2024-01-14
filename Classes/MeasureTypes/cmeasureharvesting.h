#ifndef CMEASUREHARVESTING_H
#define CMEASUREHARVESTING_H


#include "Classes/cmeasure.h"
class CMeasureHarvesting : public CMeasure
{
public:
    CMeasureHarvesting(QString, QString);
    CMeasureHarvesting(QString, int, QDate, int);
};

#endif // CMEASUREHARVESTING_H
