#ifndef CFIELD_H
#define CFIELD_H

#pragma once

#include <QString>
#include "Classes/ccrop.h"

class CField
{
public:
    CField();
    CField(QString name);
    CField(QString name, float size, QString regNum);
    CField(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop, QString nextInterCrop);

    bool operator==(const CField &rhs);

    //------------ Methods ------------------------
    bool usesCrop(QString name);

    //----- Getter -----
    CCrop* getLastCrop(int at);
    CCrop* getLastInterCrop(int at);
    CCrop* getNextCrop();
    CCrop* getNextInterCrop();
    QString getName();
    float getSize();
    QString getRegNumber();
    std::vector<QString> getAllAttributes();

    //----- Setter -----
    void setLastCrop(CCrop*);
    void setLastInterCrop(CCrop*);
    void setNextCrop(CCrop*);
    void setNextInterCrop(CCrop*);

    void setName(QString);
    void setSize(float);
    void setRegNumber(QString);

    void setAllParameters(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop, QString nextInterCrop);

private:
    QString name;
    float size;
    QString regNumber;
    CCrop* lastCrops[5] = {0};
    CCrop* lastInterCrops[5] = {0};
    CCrop* nextCrop;
    CCrop* nextInterCrop;
};

#endif // CFIELD_H
