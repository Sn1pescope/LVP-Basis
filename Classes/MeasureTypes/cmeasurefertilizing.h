#ifndef CMEASUREFERTILIZING_H
#define CMEASUREFERTILIZING_H


#include "Classes/cmeasure.h"
class CMeasureFertilizing : public CMeasure
{
public:
    CMeasureFertilizing(QString, QString);
    CMeasureFertilizing(QString, int, QDate, int);
};

#endif // CMEASUREFERTILIZING_H
