#include "qcreatefield.h"
#include "qtreewidget.h"
#include "ui_qcreatefield.h"

QCreateField::QCreateField(QWidget *parent, bool create) :
    QDialog(parent),
    ui(new Ui::QCreateField)
{
    ui->setupUi(this);

    this->create = create;
    ini();
}

QCreateField::QCreateField(QWidget *parent, QString name, bool create) :
    QDialog(parent),
    ui(new Ui::QCreateField)
{
    ui->setupUi(this);

    this->create = create;
    ini();

    //Autofill and set filled crop cbs checked
    oldName = name;
    CField* field = CDataManager::getCurrentFarm()->getField(oldName);
    ui->le_Name->setText(field->getName());
    ui->spBox_Size->setValue(field->getSize());
    ui->le_RegNumber->setText(field->getRegNumber());
    for(int i = 0; i < 10; i++){
        //For every cb; till index 4 = Crops, 5+ = InterCrops
        QComboBox* cb = cbs[i];
        if(i < 5){
            //getLastCrop can be zero
            CCrop* lastCrop = field->getLastCrop(i);
            if(lastCrop != nullptr){
                cb->setCurrentText(lastCrop->getName());
                widgets[i]->setVisible(true);
                gbs[i]->setChecked(true);
            }
        }else{
            //getLastInterCrop can be zero
            CCrop* lastInterCrop = field->getLastInterCrop(i-5);
            if(lastInterCrop != nullptr){
                cb->setCurrentText(lastInterCrop->getName());
                widgets[i]->setVisible(true);
                widgets[i-5]->setVisible(true);
                gbs[i-5]->setChecked(true);
                chbs[i-5]->setChecked(true);
            }
        }
    }
}

void QCreateField::ini(){
    //Validators
    ui->le_RegNumber->setValidator(new QRegExpValidator(CSecurityManager::REG_NUMBER, ui->le_RegNumber));
    ui->le_Name->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS, ui->le_Name));

    widgets[0]=ui->widget1; widgets[1]=ui->widget2; widgets[2]=ui->widget3; widgets[3]=ui->widget4;
    widgets[4]=ui->widget5; widgets[5]=ui->widget6; widgets[6]=ui->widget7; widgets[7]=ui->widget8;
    widgets[8]=ui->widget9; widgets[9]=ui->widget10;
    chbs[0]=ui->check1; chbs[1]=ui->check2; chbs[2]=ui->check3; chbs[3]=ui->check4; chbs[4]=ui->check5;

    //Make groupBoxes collapseable/expandable and set them collapsed
    connect(ui->gbCrop1, &QGroupBox::clicked, ui->widget1, &QWidget::setVisible);
    connect(ui->gbCrop2, &QGroupBox::clicked, ui->widget2, &QWidget::setVisible);
    connect(ui->gbCrop3, &QGroupBox::clicked, ui->widget3, &QWidget::setVisible);
    connect(ui->gbCrop4, &QGroupBox::clicked, ui->widget4, &QWidget::setVisible);
    connect(ui->gbCrop5, &QGroupBox::clicked, ui->widget5, &QWidget::setVisible);
    for(int i = 0; i < 5; i++){
        QWidget* widget = widgets[i];
        widget->setVisible(false);
        widget->setEnabled(true);
    }

    //Make InterCrop sections collapseable/expandable and set them collapsed
    connect(ui->check1, &QCheckBox::clicked, ui->widget6, &QWidget::setVisible);
    connect(ui->check2, &QCheckBox::clicked, ui->widget7, &QWidget::setVisible);
    connect(ui->check3, &QCheckBox::clicked, ui->widget8, &QWidget::setVisible);
    connect(ui->check4, &QCheckBox::clicked, ui->widget9, &QWidget::setVisible);
    connect(ui->check5, &QCheckBox::clicked, ui->widget10, &QWidget::setVisible);
    for(int j = 5; j < 10; j++){
        QWidget* w = widgets[j];
        w->setVisible(false);
        w->setEnabled(true);
    }

    //Generate year texts for groupBoxes and set them unchecked

    gbs[0]=ui->gbCrop1; gbs[1]=ui->gbCrop2; gbs[2]=ui->gbCrop3; gbs[3]=ui->gbCrop4; gbs[4]=ui->gbCrop5;
    int i = 0;
    for(QGroupBox* gb : gbs){
        gb->setTitle(CCommunicator::generateHarvestYearString(i));
        gb->setChecked(false);
        i++;
    }

    //Generate comboBox entrys
    cbs[0] = ui->cbCrop1_2; cbs[1] = ui->cbCrop2; cbs[2] = ui->cbCrop3; cbs[3] = ui->cbCrop4; cbs[4] = ui->cbCrop5;
    cbs[5] = ui->cbInterCrop1_2; cbs[6] = ui->cbInterCrop2; cbs[7] = ui->cbInterCrop3; cbs[8] = ui->cbInterCrop4;
    cbs[9] = ui->cbInterCrop5;
    QStringList names = CDataManager::getAllCropNames();
    for(QComboBox* cb : cbs){
        cb->addItem("");
        for(QString name : names){
            cb->addItem(name);
        }
    }  
}

//Read input at done and check if field with name already exists -> if yes: warning and no adding
void QCreateField::done(int res){
    if(res == QDialog::Accepted){
        //General data
        QString name = ui->le_Name->text();
        QString regNumber = ui->le_RegNumber->text();
        QString size = ui->spBox_Size->text();
        size.replace(',', '.');
        if(name.isEmpty() || regNumber.isEmpty() || size.isEmpty()){
            //Warning all fields need to be filled
            QWarningDialog(this, 0, tr("All input fields need to be filled!"), true, false).exec();
            return;
        }else{
            std::vector<QString> names = CDataManager::getCurrentFarm()->getAllFieldNames();
            if(create){
                auto it = std::find(names.begin(), names.end(), name);
                if(it != names.end()){
                    QWarningDialog(this, 0, tr("Field %1 already exists!").arg(name), true, false).exec();
                    return;
                }
            }
            //Get crops and intercrops
            std::vector<QString> sC;
            int i = 0;
            for(QWidget* w : widgets){
                //If widget not visible -> not activated -> set to "NULL"
                if(!w->isVisible()){
                    sC.push_back(QString::fromStdString("NULL"));
                }else{
                    QString selCropName = cbs[i]->currentText();
                    if(selCropName.isEmpty()){
                        sC.push_back(QString::fromStdString("NULL"));
                    }else{
                        sC.push_back(selCropName);
                    }
                }
                i++;
            }
            if(create){
                //Add field
                CDataManager::getCurrentFarm()->addField(CField(name, size.toFloat(), regNumber, sC.at(0), sC.at(1),
                                                                sC.at(2), sC.at(3), sC.at(4), sC.at(5), sC.at(6),
                                                                sC.at(7), sC.at(8), sC.at(9), 0, 0, false, false));
            }else{
                //Update Field
                CField* toUpdate = CDataManager::getCurrentFarm()->getField(oldName);
                toUpdate->setAllParameters(name, size.toFloat(), regNumber, sC.at(0), sC.at(1), sC.at(2),
                                           sC.at(3), sC.at(4), sC.at(5), sC.at(6), sC.at(7), sC.at(8), sC.at(9), toUpdate->getNextCrop()->getName(), toUpdate->getNextInterCrop()->getName(), toUpdate->isCropPlanted(), toUpdate->isInterCropPlanted());
                //Update listView
                this->parent()->findChild<QTreeWidget*>("listFields")->currentItem()->setText(0, name);
                CCommunicator::saveData();
            }
        }
    }
    QDialog::done(res);
}

QCreateField::~QCreateField()
{
    delete ui;
}
