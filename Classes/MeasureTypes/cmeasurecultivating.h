#ifndef CMEASURECULTIVATING_H
#define CMEASURECULTIVATING_H


#include "Classes/cmeasure.h"

class CMeasureCultivating : public CMeasure
{
public:
    CMeasureCultivating(QString, QString);
    CMeasureCultivating(QString, int, QDate, int);
};

#endif // CMEASURECULTIVATING_H
