/*
#   Things that will be saved in this class:
#       Tmp file ~/.tmp/TMP.lvp:
#           key: lastFarm       data: <LastLoadedFarm>
#
#       Config file /config/Config.lvp:
#           key: farmNames      data: <NamesOfAllCreatedFarms>
#           key: defaultLocale  data: <LastLoadedLanguage>
#
#       Crop file /config/Crops.lvp:
#           key: <CropName>     data: <AllAttributesOfThisCrop>
#           ...
#
#       Farm file /Data/<FarmName>.lvp:
#           key: fields         data: <AllAtributesOfAllFields>
#           key: name           data: <FarmName>
#           key: workDir        data: <PathOfWorkDir>
#           key: workUnits      data: <AllAtributesOfAllWorkUnits>
#
#       Field files /Data/<FieldName>/<FieldName_YEAR>.lvpf:
#           key: crop           data: <Crop of this year>
#           key: inter          data: <InterCrop of this year>
*/

#include "Classes/ccommunicator.h"
#include "Classes/cdatamanager.h"
#include "cfilemanager.h"

QDir CFileManager::tmpDir(QDir::tempPath());
QFile CFileManager::tmpFile(tmpDir.filePath("TMP.lvp"));
QMap<QString, QString> CFileManager::tmpData;                   //keys: lastFarm

QDir CFileManager::confDir;
QFile CFileManager::confFile;
QFile CFileManager::cropFile;
QMap<QString, QString> CFileManager::confData;                  //keys: farmNames | defaultLocale | harvestYearEnd
QMap<QString, QString> CFileManager::cropsData;                 //keys: for every crop one key

QMap<QString, QStringList> CFileManager::farmData;                  //keys: fields | name | workDir | workUnits

CFileManager::CFileManager()
{

}

void CFileManager::initialize(){
    confDir.setPath(qApp->applicationDirPath() + "/config");
    confFile.setFileName(confDir.filePath("Config.lvp"));
    cropFile.setFileName(confDir.filePath("Crops.lvp"));
    loadConfData();
    loadTmpData();
    loadCropData();
}

bool CFileManager::tmpFileExists(){
    return tmpFile.exists();
}

void CFileManager::checkDirs(){
    if(!tmpDir.exists()){
        tmpDir.mkpath(tmpDir.path());
    }
    if(!confDir.exists()){
        confDir.mkpath(confDir.path());
    }
}

void CFileManager::writeTmpFile(){
    //Write data to tmpFile
    if(!tmpFile.open(QIODevice::WriteOnly)){
        return;
    }

    tmpFile.resize(0);
    QMap<QString, QString> data;
    QString curFarmName = CDataManager::getCurrentFarmName();
    if(!curFarmName.isEmpty()){
        data.insert(QString::fromStdString("lastFarm"), curFarmName);
    }

    QDataStream out(&tmpFile);
    out.setVersion(QDataStream::Qt_5_5);
    out << data;
    tmpFile.close();
}

void CFileManager::writeConfFile(){
    //Write data to ConfFile
    if(!confFile.open(QIODevice::WriteOnly)){
        return;
    }

    confFile.resize(0);

    //Farm names
    QString farmNames;
    std::vector<QString> allNames = CDataManager::getAllFarmNames();
    for(int i = 0; i < (int)allNames.size(); i++){
        if(i != 0){
            farmNames += " ";
        }
        farmNames += allNames.at(i);
    }

    //DefaultLocale
    QString currentLocale = CLanguageManager::getDefaultLocale();

    //HarvestYearEnd
    int harvestYearEnd = CDataManager::getHarvestYearEnd();

    //LoadYearsBefore
    int loadYearsBefore = CDataManager::getLoadYearsBefore();

    //Write data in file
    QMap<QString, QString> data;
    if(!farmNames.isEmpty()){
        data.insert(QString::fromStdString("farmNames"), farmNames);
    }
    data.insert(QString::fromStdString("defaultLocale"), currentLocale);
    data.insert(QString::fromStdString("harvestYearEnd"), QString::number(harvestYearEnd));
    data.insert(QString::fromStdString("loadYearsBefore"), QString::number(loadYearsBefore));


    QDataStream out(&confFile);
    out.setVersion(QDataStream::Qt_5_5);
    out << data;
    confFile.close();
}

void CFileManager::writeCropFile(){
    //Write data to CropFile
    if(!cropFile.open(QIODevice::WriteOnly)){
        return;
    }

    cropFile.resize(0);

    //Data to save
    QMap<QString, QString> data;

    // Every crop with attributes
    std::vector<CCrop> allCrops = CDataManager::getAllCrops();
    for(int i = 0; i < (int)allCrops.size(); i++){
        QString cropData = "";
        std::vector<QString> attributes = allCrops.at(i).getAllAttributes();
        QString key = QString::number(i);
        for(int j = 0; j < (int)attributes.size(); j++){
            if(j != 0){
                cropData += " ";
            }
            cropData += attributes.at(j);
        }
        data.insert(key, cropData);
    }


    QDataStream out(&cropFile);
    out.setVersion(QDataStream::Qt_5_5);
    out << data;
    cropFile.close();
}

void CFileManager::writeFarmData(){
    //TODO: Just save essential field data here -> make file for each field and year with other data
    //Write data to workDir
    if(CDataManager::getCurrentFarm() == 0){
        return;
    }
    QDir workDir = CDataManager::getCurrentWorkDir();
    QFile farmFile(workDir.filePath(CDataManager::getCurrentFarmName() + ".lvp"));
    if(!workDir.exists()){
        workDir.mkpath(".");
    }
    if(!farmFile.open(QIODevice::WriteOnly)){
        return;
    }

    farmFile.resize(0);

    // Fields
    QString fieldData;
    std::vector<CField> fields = CDataManager::getCurrentFarm()->getAllFields();
    for(int i = 0; i < (int)fields.size(); i++){
        CField f = fields.at(i);
        if(i != 0){
            fieldData += ";";
        }
        std::vector<QString> attributes = f.getAllEssentialAttributes();
        for(int j = 0; j < (int)attributes.size(); j++){
            if(j != 0){
                fieldData += " ";
            }
            fieldData += attributes.at(j);
        }
        //Create field files
        writeFieldFiles(f, workDir);
    }

    // WorkUnits
    QString workUnitData;
    std::vector<CWorkUnit> workUnits = CDataManager::getCurrentFarm()->getAllWorkUnits();
    for(int i = 0; i < (int)workUnits.size(); i++){
        if(i != 0){
            workUnitData += ";";
        }
        std::vector<QString> attributes = workUnits.at(i).getAllAttributes();
        for(int j = 0; j < (int)attributes.size(); j++){
            if(j != 0){
                workUnitData += " ";
            }
            workUnitData += attributes.at(j);
        }
    }

    // Farm Name
    QString farmName = CDataManager::getCurrentFarmName();

    //Data to save
    QMap<QString, QString> data;
    if(!workUnitData.isEmpty()){
        data.insert(QString::fromStdString("workUnits"), workUnitData);
    }
    if(!fieldData.isEmpty()){
        data.insert(QString::fromStdString("fields"), fieldData);
    }
    if(farmName != ""){
        data.insert(QString::fromStdString("name"), farmName);
    }
    if(workDir.exists()){
        data.insert(QString::fromStdString("workDir"), workDir.absolutePath());
    }

    QDataStream out(&farmFile);
    out.setVersion(QDataStream::Qt_5_5);
    out << data;
    farmFile.close();
}

QString CFileManager::writeFieldFiles(CField of, QDir workDir){
    //Folder with fieldname; in it for every year one file with data of this year
    workDir.setPath(workDir.path() + "/" + of.getName());
    if(!workDir.exists()){
        workDir.mkpath(".");
    }

    //Save last 5 years -> all longer ago isnt editable anymore
    int year = CCommunicator::getCurrentHarvestYearBegin().year();

    for(int i = 0; i < 5; i++){
        //Create a file -> Save data in it
        QFile fieldFile(workDir.filePath(of.getName() + "_%1" + ".lvpf").arg(year-i));
        if(!fieldFile.open(QIODevice::WriteOnly)){
            return "";
        }
        fieldFile.resize(0);

        QMap<QString, QString> data = of.getFieldData(i);

        QDataStream out(&fieldFile);
        out.setVersion(QDataStream::Qt_5_5);
        out << data;
        fieldFile.close();
    }

    return workDir.path();
}

QString CFileManager::exportFieldData(CField f, QDir workDir){
    return writeFieldFiles(f, workDir);
}

QString CFileManager::openFarmData(QDir dir){
    //Create new folder in /Data
    QDir workDir(QCoreApplication::applicationDirPath() + "/Data/" + dir.dirName());
    if(!workDir.exists()){
        workDir.mkpath(".");
        //Copy .lvp File
        QFile lvpFile(dir.filePath(dir.dirName() + ".lvp"));
        if(lvpFile.exists()){
            lvpFile.copy(workDir.filePath(dir.dirName() + ".lvp"));
            return workDir.dirName();
        }
    }
    return "";

}

void CFileManager::loadNewFarm(QDir workDir, QString farmName){
    //Load farmData in Map
    QMap<QString, QString> data;
    QFile farmFile(workDir.filePath(farmName + ".lvp"));
    //Get data
    if(farmFile.open(QIODevice::ReadOnly)){
        QDataStream in(&farmFile);
        in.setVersion(QDataStream::Qt_5_5);
        in >> data;
        farmFile.close();
    }
    //QMap<QString, QStringList> farmData;

    //Put data to right place

    //key: fields
    if(data.contains("fields")){
        QString valString = data.value("fields");
        QStringList fields = valString.split(QLatin1Char(';'));
        farmData.insert(QString::fromStdString("fields"), fields);
    }
    //key: workUnits
    if(data.contains("workUnits")){
        QString valString = data.value("workUnits");
        QStringList workUnits = valString.split(QLatin1Char(';'));
        farmData.insert(QString::fromStdString("workUnits"), workUnits);
    }
    //key: name
    if(data.contains("name")){
        farmData.insert(QString::fromStdString("name"), QStringList(data.value("name")));
    }
    //key: workDir
    if(data.contains("workDir")){
        farmData.insert(QString::fromStdString("workDir"), QStringList(data.value("workDir")));
    }
}

void CFileManager::updateFarmDir(QDir oldDir, QString oldName, QString to){
    //Rename farmFile
    QFile farmFile(oldDir.filePath(oldName + ".lvp"));
    farmFile.rename(oldDir.filePath(to + ".lvp"));

    //Rename farmDir
    oldDir.cdUp();
    oldDir.rename(oldName, to);
}

void CFileManager::loadConfData(){
    confData.clear();
    if(confFile.open(QIODevice::ReadOnly)){
        QDataStream in(&confFile);
        in.setVersion(QDataStream::Qt_5_5);
        in >> confData;
        confFile.close();
    }
}

void CFileManager::loadTmpData(){
    if(CFileManager::tmpFileExists()){
        tmpData.clear();
        if(tmpFile.open(QIODevice::ReadOnly)){
            QDataStream in(&tmpFile);
            in.setVersion(QDataStream::Qt_5_5);
            in >> tmpData;
            tmpFile.close();
        }
    }
}

void CFileManager::loadCropData(){
    cropsData.clear();
    if(cropFile.open(QIODevice::ReadOnly)){
        QDataStream in(&cropFile);
        in.setVersion(QDataStream::Qt_5_5);
        in >> cropsData;
        cropFile.close();
    }
}


QMap<QString, QString> CFileManager::loadFieldData(QString name, int yearsBack, QDir workDir){
    QMap<QString, QString> data;
    workDir.setPath(workDir.path() + "/" + name);
    //Load special year
    int year = CCommunicator::getCurrentHarvestYearBegin().year();
    QFile fieldFile(workDir.filePath(name + "_%1" + ".lvpf").arg(year-yearsBack));
    if(fieldFile.open(QIODevice::ReadOnly)){
        QDataStream in(&fieldFile);
        in.setVersion(QDataStream::Qt_5_5);
        in >> data;
        fieldFile.close();
    }
    return data;
}

//-------------------------- Getter ---------------------------

int CFileManager::getNumberOfDataFiles(QString name, QDir workDir){
    workDir.setPath(workDir.path() + "/" + name);
    return workDir.entryList(QDir::Files | QDir::NoSymLinks).size();
}

QMap<QString, QString> CFileManager::getFieldData(QString name, int yearsBack, QDir workDir){
    return loadFieldData(name, yearsBack, workDir);
}

QString CFileManager::getLastFarmName(){
    if(tmpData.contains("lastFarm")){
        return tmpData.value("lastFarm");
    }
    return QString("");
}

std::vector<QString> CFileManager::getFarmNames(){
    std::vector<QString> farmNames;
    if(confData.contains("farmNames")){
        QString farmNameString = confData.value("farmNames");
        QStringList names = farmNameString.split(QLatin1Char(' '));
        for(int i = 0; i < names.length(); i++){
            farmNames.push_back(names.at(i));
        }
    }
    return farmNames;
}

std::vector<QStringList> CFileManager::getCropData(){
    std::vector<QStringList> cropData;

    //For amount of keys...
    QList<QString> keys = cropsData.keys();
    for(int i = 0; i < keys.size(); i++){
        //Get first value, second value, ...
        QString key = QString::number(i);
        QString valString = cropsData.value(key);
        cropData.push_back(valString.split(QLatin1Char(' ')));
    }
    return cropData;
}

QMap<QString, QStringList> CFileManager::getFarmData(){
    //Return from map
    return farmData;
}

QString CFileManager::getDefaultLocale(){
    if(confData.contains("defaultLocale")){
        return confData.value("defaultLocale");
    }
    return QString("");
}

int CFileManager::getHarvestYearEnd(){
    if(confData.contains("harvestYearEnd")){
        return confData.value("harvestYearEnd").toInt();
    }
    return CDataManager::STANDARD_HARVEST_YEAR_END;
}

int CFileManager::getLoadYearsBefore(){
    if(confData.contains("loadYearsBefore")){
        return confData.value("loadYearsBefore").toInt();
    }
    return CDataManager::STANDARD_LOAD_YEARS_BEFORE;
}
