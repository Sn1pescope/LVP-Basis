#ifndef CWORKUNIT_H
#define CWORKUNIT_H

#pragma once

#include <QString>
#include <vector>

class CWorkUnit
{
public:
    CWorkUnit(QString);
    //TODO: Some other data: CWorkUnit(QString, )

    void addField(QString);
    void removeField(QString);
    void removeField(int);
    bool hasField(QString);

    std::vector<QString> getAllAttributes();

private:
    QString name;
    std::vector<QString> fields;
};

#endif // CWORKUNIT_H
