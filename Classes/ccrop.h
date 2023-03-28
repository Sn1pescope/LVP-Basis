#ifndef CCROP_H
#define CCROP_H

#pragma once

#include <QString>
#include <vector>

class CCrop
{
public:
    CCrop();
    CCrop(QString name);

    //--------------------------------- Getter ------------------------------------
    std::vector<QString> getAllAttributes();
    QString getName() const;

    //--------------------------------- Setter ------------------------------------
    void setName(QString);

    //--------------------------------- Operators ------------------------------------
    bool operator==(const CCrop &rhs);

    //--------------------------------- Attributes ------------------------------------
private:
    QString name;
};

#endif // CCROP_H
