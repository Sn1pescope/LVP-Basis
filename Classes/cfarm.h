#ifndef CFARM_H
#define CFARM_H

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

    bool operator==(const CFarm &rhs);

    // ------- Getter ---------
    QString getFarmName();
    QDir getWorkDir();
    std::vector<QString> getAllFieldNames();
    std::vector<CField> getAllFields();
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
};

#endif // CFARM_H
