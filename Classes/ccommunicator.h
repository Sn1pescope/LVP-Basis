#ifndef CCOMMUNICATOR_H
#define CCOMMUNICATOR_H

#pragma once

#include "Classes/cdatamanager.h"
#include "Classes/cfarm.h"
#include "Classes/csecuritymanager.h"
#include "Classes/cfilemanager.h"
#include "Classes/clanguagemanager.h"
#include <QString>
#include <QDate>


class CCommunicator
{
    Q_DECLARE_TR_FUNCTIONS(CCommunicator)

public:
    CCommunicator();

    //General Management
    static void initialize();
    static void saveData();
    //Farms
    static void createNewFarm(QString);
    static bool loadFarm(int);
    static bool loadFarm(QString);
    static bool tryAutoload();
    static void closeFarm();
    static void deleteFarm(QString);
    static void reloadFarm();
    static bool openFarm(QDir dir);
    //Fields
    static void deleteFieldOfCurrentFarm(QString);
    static void deleteField(QString name);
    static QString exportFieldData(QString name);
    //Crops
    static void deleteCrop(QString);
    //Settings
    static QString generateHarvestYearString(int yearsBack);
    static void setHarvestYearEnd(int);
};

#endif // CCOMMUNICATOR_H
