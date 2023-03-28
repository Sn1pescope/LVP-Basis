#ifndef CDATAMANAGER_H
#define CDATAMANAGER_H

#pragma once

#include "Classes/cfarm.h"
#include "Classes/cfilemanager.h"
#include "Classes/ccrop.h"
#include <QDir>
#include <QObject>
#include <vector>
#include <QTranslator>

using std::find_if;

class CDataManager : public QObject
{
    Q_OBJECT

public:
    CDataManager();

    static CDataManager& instance(){
        static CDataManager m_instance;
        return m_instance;
    }

    //--------------- Signals -----------------------

    Q_SIGNAL void currentFarmChanged();

    //--------------- Functions ---------------------
public:
    static void addFarm(QString);
    static bool loadFarm(int);
    static bool loadFarm(QString);
    static bool tryAutoload();
    static void loadFarmNames();
    static void unloadCurFarm();
    static void clearFarms();
    static void deleteFarm(int);
    static void deleteFarm(QString);
    static void reloadFarm();
    static void addCrop(CCrop);
    static void loadCrops();
    static void deleteCrop(int);
    static void deleteCrop(QString);

    //------------------ Getter -----------------------

    static QString getCurrentFarmName();
    static QDir getCurrentWorkDir();
    static std::vector<QString> getAllFarmNames();
    static std::vector<CCrop> getAllCrops();
    static QStringList getAllCropNames();
    static CFarm* getCurrentFarm();
    static CFarm* getFarm(int);
    static CFarm* getFarm(QString);
    static CCrop* getCrop(QString);
    static int getHarvestYearEnd();

    //------------------ Setter -----------------------

    static void setFarmName(int, QString);
    static void setFarmName(QString, QString);
    static void setHarvestYearEnd(int to);

private:
    //------------------ Attributes ---------------------

    static std::vector<CFarm> farms;
    static std::vector<CCrop> crops;
    static int activeFarm;
    static QString farmName;
    static QDir workDir;
    static int harvestYearEnd;

public:
    static int STANDARD_HARVEST_YEAR_END;
};

#endif // CDATAMANAGER_H
