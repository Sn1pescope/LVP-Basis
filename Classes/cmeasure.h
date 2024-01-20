#ifndef CMEASURE_H
#define CMEASURE_H

#include "qcoreapplication.h"
#include "qdatetime.h"
#include <QMap>
#include <QSharedPointer>
#include <QObject>
class CMeasure : public QObject
{
    Q_OBJECT

public:
    static CMeasure& instance(){
        static CMeasure m_instance;
        return m_instance;
    }
    CMeasure();

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
    static const QMap<int, QString> ICONS_TYPES;

    void setState(int);
    void setDate(QDate);
    void setType(int);
    void setField(QString);

    int getState();
    QDate getDate();
    int getType();
    QString getField();
    QString getKey();

    QString getSavingData();

    bool operator<(CMeasure &rhs);
    bool operator==(CMeasure &rhs);

    static bool greater(QSharedPointer<CMeasure> th, QSharedPointer<CMeasure> rhs);

public slots:
    static void shutdown();

private:
    void generateKey();

private:
    static const int NUM_ATTRIBUTES = 3;
    static std::vector<QString> keys;

    QString field;
    int state;
    QDate date;
    int type;
    QString key;
    static bool shut;
};

#endif // CMEASURE_H
