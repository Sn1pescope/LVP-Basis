#ifndef CMEASURESPRAYING_H
#define CMEASURESPRAYING_H


#include "Classes/cmeasure.h"
class CMeasureSpraying : public CMeasure
{
public:
    CMeasureSpraying(QString, QString);
    CMeasureSpraying(QString, int, QDate, int);
};

#endif // CMEASURESPRAYING_H
