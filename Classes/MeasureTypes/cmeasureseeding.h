#ifndef CMEASURESEEDING_H
#define CMEASURESEEDING_H


#include "Classes/cmeasure.h"
class CMeasureSeeding : public CMeasure
{
public:
    CMeasureSeeding(QString, QString);
    CMeasureSeeding(QString, int, QDate, int);
};

#endif // CMEASURESEEDING_H
