#include "qsplitmergefield.h"
#include "Classes/GUI/Dialogs/qwarningdialog.h"
#include "Classes/ccommunicator.h"
#include "Classes/csecuritymanager.h"
#include "Classes/cdatamanager.h"
#include "qicon.h"
#include "ui_qsplitmergefield.h"

QSplitMergeField::QSplitMergeField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSplitMergeField)
{
    ini();
}

QSplitMergeField::QSplitMergeField(QWidget *parent, CField f) : QDialog(parent), ui(new Ui::QSplitMergeField){
    ini();
    f1 = f;
    //Split mode
    this->setWindowTitle(tr("Split field '%1'").arg(f.getName()));
    merge = false;
    ui->widgetLeft->setEnabled(true);
    ui->widgetMiddle->setEnabled(false);
    ui->widgetRight->setEnabled(true);
    //set icons
    ui->labelRight->setPixmap(QIcon(":/icons/arrow-right-solid.png").pixmap(32, 32));
    ui->labelLeft->setPixmap(QIcon(":/icons/arrow-left-solid.png").pixmap(32, 32));

    ui->nameMiddle->setText(f1.getName());
    ui->sizeMiddle->setValue(f1.getSize());
    ui->regNumMiddle->setText(f1.getRegNumber());
    ui->nameLeft->setText(f1.getName() + "-1");
    ui->sizeLeft->setValue(f1.getSize() / 2);
    ui->regNumLeft->setText(f1.getRegNumber());
    ui->nameRight->setText(f1.getName() + "-2");
    ui->sizeRight->setValue(f1.getSize() / 2);
    ui->regNumRight->setText(f1.getRegNumber());
}

QSplitMergeField::QSplitMergeField(QWidget *parent, CField f1_par, CField f2_par) : QDialog(parent), ui(new Ui::QSplitMergeField){
    ini();
    f1 = f1_par;
    f2 = f2_par;
    this->setWindowTitle(tr("Merge fields '%1' and '%2'").arg(f1.getName(), f2.getName()));
    //Merge mode
    merge = true;
    ui->widgetLeft->setEnabled(false);
    ui->widgetMiddle->setEnabled(true);
    ui->widgetRight->setEnabled(false);
    //set icons
    ui->labelRight->setPixmap(QIcon(":/icons/arrow-left-solid.png").pixmap(32, 32));
    ui->labelLeft->setPixmap(QIcon(":/icons/arrow-right-solid.png").pixmap(32, 32));

    QString regNum = "";
    if(f1.getRegNumber() == f2.getRegNumber()){
        regNum = f1.getRegNumber();
    }
    ui->nameMiddle->setText(f1.getName());
    ui->sizeMiddle->setValue(f1.getSize() + f2.getSize());
    ui->regNumMiddle->setText(regNum);
    ui->nameLeft->setText(f1.getName());
    ui->sizeLeft->setValue(f1.getSize());
    ui->regNumLeft->setText(f1.getRegNumber());
    ui->nameRight->setText(f2.getName());
    ui->sizeRight->setValue(f2.getSize());
    ui->regNumRight->setText(f2.getRegNumber());
}

void QSplitMergeField::ini(){
    ui->setupUi(this);
    //Set security
    ui->nameLeft->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS));
    ui->nameMiddle->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS));
    ui->nameRight->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS));
    ui->regNumLeft->setValidator(new QRegExpValidator(CSecurityManager::REG_NUMBER));
    ui->regNumMiddle->setValidator(new QRegExpValidator(CSecurityManager::REG_NUMBER));
    ui->regNumRight->setValidator(new QRegExpValidator(CSecurityManager::REG_NUMBER));
}

void QSplitMergeField::done(int res){
    if(res == QDialog::Accepted){
    if(merge){
        //MERGE
        //Read in values
        QString nameNew = ui->nameMiddle->text();
        double sizeNew = ui->sizeMiddle->value();
        QString regNumNew = ui->regNumMiddle->text();
        //Check values
        if(nameNew.isEmpty() || sizeNew <= 0 || regNumNew.isEmpty()){
            QWarningDialog(this, tr("All fields need to be filled!"), true).exec();
            return;
        }
        if(nameNew != f1.getName() && nameNew != f2.getName()){
            std::vector<QString> fields = CDataManager::getCurrentFarm()->getAllFieldNames();
            auto it = std::find(fields.begin(), fields.end(), nameNew);
            if(it != fields.end()){
                QWarningDialog(this, tr("The field '%1' already exists! Choose another name!").arg(nameNew), true).exec();
                return;
            }
        }
        //Make crops
        QString crops[5];
        QString interCrops[5];
        QString nextCrop;
        QString nextInterCrop;
        for(int i = 0; i < 5; i++){
            if(f1.getLastCrop(i) != nullptr && f1.getLastCrop(i) == f2.getLastCrop(i)){
                crops[i] = f1.getLastCrop(i)->getName();
            }else{
                crops[i] = "";
            }
            if(f1.getLastInterCrop(i) != nullptr && f1.getLastInterCrop(i) == f2.getLastInterCrop(i)){
                interCrops[i] = f1.getLastInterCrop(i)->getName();
            }else{
                interCrops[i] = "";
            }
        }
        if(f1.getNextCrop() != nullptr && f1.getNextCrop() == f2.getNextCrop()){
            nextCrop = f1.getNextCrop()->getName();
        }else{
            nextCrop = "";
        }
        if(f1.getNextInterCrop() != nullptr && f1.getNextInterCrop() == f2.getNextInterCrop()){
            nextInterCrop = f1.getNextInterCrop()->getName();
        }else{
            nextInterCrop = "";
        }
        //delete old fields
        QString wu = CDataManager::getCurrentFarm()->getWorkUnitOfField(f1.getName());
        CCommunicator::deleteField(f1.getName());
        CCommunicator::deleteField(f2.getName());
        //add new field
        CDataManager::getCurrentFarm()->addField(CField(nameNew, sizeNew, regNumNew, crops[0], crops[1], crops[2], crops[3], crops[4],
                                                        interCrops[0], interCrops[1], interCrops[2], interCrops[3], interCrops[4], nextCrop, nextInterCrop, false, false));
        CDataManager::getCurrentFarm()->addFieldToWorkUnit(nameNew, wu);
    }else{
        //SPLIT
        //Read in values
        QString name1 = ui->nameLeft->text();
        double size1 = ui->sizeLeft->value();
        QString regNum1 = ui->regNumLeft->text();
        QString name2 = ui->nameRight->text();
        double size2 = ui->sizeRight->value();
        QString regNum2 = ui->regNumRight->text();
        //Check values
        if(name1.isEmpty() || size1 <= 0 || regNum1.isEmpty() || name2.isEmpty() || size2 <= 0 || regNum2.isEmpty()){
            QWarningDialog(this, tr("All fields need to be filled!"), true).exec();
            return;
        }
        std::vector<QString> fields = CDataManager::getCurrentFarm()->getAllFieldNames();
        auto it = std::find(fields.begin(), fields.end(), name1);
        if(it != fields.end()){
            QWarningDialog(this, tr("The field '%1' already exists! Choose another name!").arg(name1), true).exec();
            return;
        }
        auto its = std::find(fields.begin(), fields.end(), name2);
        if(its != fields.end()){
            QWarningDialog(this, tr("The field '%1' already exists! Choose another name!").arg(name2), true).exec();
            return;
        }
        if(name1 == name2){
            QWarningDialog(this, tr("You need to specify two different names for your fields!"), true).exec();
            return;
        }
        //Work unit
        QString wu = CDataManager::getCurrentFarm()->getWorkUnitOfField(f1.getName());
        if(wu.isEmpty()){
            wu = f1.getName();
            CDataManager::getCurrentFarm()->addWorkUnit(CWorkUnit(wu));
        }

        QString crops[5];
        QString interCrops[5];
        QString nextCrop;
        QString nextInterCrop;
        for(int i = 0; i < 5; i++){
            if(f1.getLastCrop(i) != nullptr){
                crops[i] = f1.getLastCrop(i)->getName();
            }else{
                crops[i] = "";
            }
            if(f1.getLastInterCrop(i) != nullptr){
                interCrops[i] = f1.getLastInterCrop(i)->getName();
            }else{
                interCrops[i] = "";
            }
        }
        if(f1.getNextCrop() != nullptr){
            nextCrop = f1.getNextCrop()->getName();
        }else{
            nextCrop = "";
        }
        if(f1.getNextInterCrop() != nullptr){
            nextInterCrop = f1.getNextInterCrop()->getName();
        }else{
            nextInterCrop = "";
        }

        //add new fields
        CDataManager::getCurrentFarm()->addField(CField(name1, size1, regNum1, crops[0], crops[1], crops[2], crops[3], crops[4],
                                                        interCrops[0], interCrops[1], interCrops[2], interCrops[3], interCrops[4], nextCrop, nextInterCrop, f1.isCropPlanted(), f1.isInterCropPlanted()));
        CDataManager::getCurrentFarm()->addField(CField(name2, size2, regNum2, crops[0], crops[1], crops[2], crops[3], crops[4],
                                                        interCrops[0], interCrops[1], interCrops[2], interCrops[3], interCrops[4], nextCrop, nextInterCrop, f1.isCropPlanted(), f1.isInterCropPlanted()));
        CDataManager::getCurrentFarm()->addFieldToWorkUnit(name1, wu);
        CDataManager::getCurrentFarm()->addFieldToWorkUnit(name2, wu);
        //delete old field
        CCommunicator::deleteField(f1.getName());
    }
    }


    QDialog::done(res);
}

QSplitMergeField::~QSplitMergeField()
{
    delete ui;
}
