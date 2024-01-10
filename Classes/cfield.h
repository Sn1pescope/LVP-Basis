#ifndef CFIELD_H
#define CFIELD_H

#include "Classes/cmeasure.h"
#include "Classes/MeasureTypes/cmeasurecultivating.h"
#include "Classes/MeasureTypes/cmeasureseeding.h"
#include "Classes/MeasureTypes/cmeasurefertilizing.h"
#include "Classes/MeasureTypes/cmeasurespraying.h"
#include "Classes/MeasureTypes/cmeasureharvesting.h"
#include "qdir.h"
#pragma once

#include <QString>
#include "Classes/ccrop.h"

class CField
{
public:
    CField();
    //For searching
    CField(QString name);
    //For creation by loading farm
    CField(QString name, float size, QString regNum, QString, QString, bool, bool, QDir);
    //For use in app (createfield, splitmergefield, ...)
    CField(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop, QString nextInterCrop, bool cropPlanted, bool interCropPlanted);

    bool operator==(const CField &rhs);

    //------------ Methods ------------------------
    bool usesCrop(QString name);

    void addMeasure(CMeasure*);
    void removeMeasure(int);

    //----- Getter -----
    CCrop* getLastCrop(int at);
    CCrop* getLastInterCrop(int at);
    CCrop* getNextCrop();
    CCrop* getNextInterCrop();
    bool isInterCropPlanted();
    bool isCropPlanted();
    QString getName();
    float getSize();
    QString getRegNumber();
    std::vector<QString> getAllEssentialAttributes();
    QMap<QString, QString> getFieldData(int);
    std::vector<CMeasure*> getMeasuresInRange(QDate from, QDate to, bool allFuture = false);

    //----- Setter -----
    void setLastCrop(CCrop*);
    void setLastInterCrop(CCrop*);
    void setNextCrop(CCrop*);
    void setNextInterCrop(CCrop*);
    void setInterCropPlanted(bool);
    void setCropPlanted(bool);

    void setName(QString);
    void setSize(float);
    void setRegNumber(QString);

    void setAllParameters(QString name_par, float size_par, QString regNumber_par, QString lastCrop1, QString lastCrop2, QString lastCrop3, QString lastCrop4, QString lastCrop5, QString lastInterCrop1, QString lastInterCrop2, QString lastInterCrop3, QString lastInterCrop4, QString lastInterCrop5, QString nextCrop, QString nextInterCrop, bool cropPlanted, bool interCropPlanted);

private:
    QString name;
    float size;
    QString regNumber;
    CCrop* lastCrops[5] = {0};
    CCrop* lastInterCrops[5] = {0};
    CCrop* nextCrop;
    CCrop* nextInterCrop;
    bool interCropPlanted;
    bool cropPlanted;
    std::vector<CMeasure*> measures;

};

#endif // CFIELD_H
