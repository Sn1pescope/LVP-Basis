#ifndef CFARM_H
#define CFARM_H

#include "Classes/cworkunit.h"
#pragma once

#include <QDir>
#include "Classes/cfield.h"
#include "Classes/cfilemanager.h"


class CFarm
{
public:
    CFarm(QString);
    CFarm(QString, bool);

    void addField(CField);
    void deleteField(int);
    void deleteField(QString);

    void addWorkUnit(CWorkUnit);
    void deleteWorkUnit(int);
    void deleteWorkUnit(QString);
    void addFieldToWorkUnit(QString, QString);
    void removeFieldFromWorkUnit(QString, QString);
    bool fieldIsInWorkUnit(QString, QString);

    bool operator==(const CFarm &rhs);

    // ------- Getter ---------
    QString getFarmName();
    QDir getWorkDir();
    std::vector<QString> getAllFieldNames();
    std::vector<CField> getAllFields();
    std::vector<CWorkUnit> getAllWorkUnits();
    CField* getField(int);
    CField* getField(QString);

    // ------- Setter ---------
    void setFarmName(QString);

private:
    void ini(QString);

private:
    //---------- Attribute -----------------
    QString farmName;
    QDir workDir;

    std::vector<CField> fields;
    std::vector<CWorkUnit> workUnits;
};

#endif // CFARM_H
