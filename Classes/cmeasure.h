#ifndef CMEASURE_H
#define CMEASURE_H

#include "qdatetime.h"
class CMeasure
{
public:
    CMeasure();
    CMeasure(int state, QDate date, int type);
    //States
    static const int STATE_PLANNED = 0;
    static const int STATE_IN_PROGRESS = 1;
    static const int STATE_DONE = 2;
    //Types
    static const int TYPE_CULTIVATING = 0;
    static const int TYPE_SEEDING = 1;
    static const int TYPE_FERTILIZING = 2;
    static const int TYPE_SPRAYING = 3;
    static const int TYPE_HARVESTING = 4;

    void setState(int);
    void setDate(QDate);
    void setType(int);

    int getState();
    QDate getDate();
    int getType();

    bool operator<(CMeasure &rhs);

private:
    int state;
    QDate date;
    int type;
};

#endif // CMEASURE_H
