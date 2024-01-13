#ifndef CMEASURE_H
#define CMEASURE_H

#include "qcoreapplication.h"
#include "qdatetime.h"
#include <QMap>
class CMeasure
{
    Q_DECLARE_TR_FUNCTIONS(CMeasure)
public:
    //For loading
    CMeasure(QString, QString);
    //For generation
    CMeasure(QString, int state, QDate date, int type);

    virtual ~CMeasure();
    //States
    static const int STATE_PLANNED = 0;
    static const int STATE_IN_PROGRESS = 1;
    static const int STATE_DONE = 2;
    static const QMap<int, QString> STATES;
    //Types
    static const int TYPE_CULTIVATING = 0;
    static const int TYPE_SEEDING = 1;
    static const int TYPE_FERTILIZING = 2;
    static const int TYPE_SPRAYING = 3;
    static const int TYPE_HARVESTING = 4;
    static const QMap<int, QString> TYPES;

    void setState(int);
    void setDate(QDate);
    void setType(int);

    int getState();
    QDate getDate();
    int getType();
    QString getField();

    QString getSavingData();

    bool operator<(CMeasure &rhs);

private:
    static const int NUM_ATTRIBUTES = 3;

    QString field;
    int state;
    QDate date;
    int type;
};

#endif // CMEASURE_H
