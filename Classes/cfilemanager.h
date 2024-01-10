#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#include "Classes/cfield.h"
#pragma once

#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QTextStream>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QPrinter>
#include <Classes/clanguagemanager.h>


class CFileManager
{
public:
    CFileManager();

    static void initialize();
    static bool tmpFileExists();
    static void checkDirs();
    static void writeTmpFile();
    static void writeConfFile();
    static void writeCropFile();
    static void writeFarmData();
    static QString openFarmData(QDir);
    static void loadNewFarm(QDir, QString);
    static QString exportFieldData(CField, QDir);

    static void updateFarmDir(QDir oldDir, QString oldName, QString to);


    //---------------------------- Getter ----------------------------

    static QMap<QString, QString> getFieldData(QString, int, QDir);
    static QString getLastFarmName();
    static std::vector<QString> getFarmNames();
    static std::vector<QStringList> getCropData();
    static QMap<QString, QStringList> getFarmData();
    static QString getDefaultLocale();
    static int getHarvestYearEnd();
    static int getLoadYearsBefore();
    static int getNumberOfDataFiles(QString, QDir);


private:
    static void loadConfData();
    static void loadTmpData();
    static void loadCropData();

    static QString writeFieldFiles(CField, QDir);
    static QMap<QString, QString> loadFieldData(QString, int, QDir);

private:
    static QDir tmpDir;
    static QFile tmpFile;
    static QMap<QString, QString> tmpData;

    static QDir confDir;
    static QFile confFile;
    static QFile cropFile;
    static QMap<QString, QString> confData;
    static QMap<QString, QString> cropsData;

    static QMap<QString, QStringList> farmData;
};

#endif // CFILEMANAGER_H
