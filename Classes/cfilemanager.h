#ifndef CFILEMANAGER_H
#define CFILEMANAGER_H

#pragma once

#include <QFile>
#include <QDataStream>
#include <QMap>
#include <QTextStream>
#include <QDateTime>
#include <QString>
#include <QDir>
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

    static void updateFarmDir(QDir oldDir, QString oldName, QString to);


    //---------------------------- Getter ----------------------------

    static QString getLastFarmName();
    static std::vector<QString> getFarmNames();
    static std::vector<QStringList> getCropData();
    static QMap<QString, QStringList> getFarmData(QDir, QString);
    static QString getDefaultLocale();
    static int getHarvestYearEnd();

private:
    static void loadConfData();
    static void loadTmpData();
    static void loadCropData();

private:
    static QDir tmpDir;
    static QFile tmpFile;
    static QMap<QString, QString> tmpData;
    static QDir confDir;
    static QFile confFile;
    static QFile cropFile;
    static QMap<QString, QString> confData;
    static QMap<QString, QString> cropsData;
};

#endif // CFILEMANAGER_H
