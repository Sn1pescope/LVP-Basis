#include "qeditcrop.h"
#include "ui_qeditcrop.h"

QEditCrop::QEditCrop(QWidget *parent, bool create) :
    QDialog(parent),
    ui(new Ui::QEditCrop)
{
    ui->setupUi(this);
    this->create = create;

    //Validators
    ui->lineEditName->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS, ui->lineEditName));
}

QEditCrop::QEditCrop(QWidget *parent, QString name, bool create) :
    QDialog(parent),
    ui(new Ui::QEditCrop)
{
    ui->setupUi(this);
    this->create = create;

    //Validators and autofill
    ui->lineEditName->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS, ui->lineEditName));
    ui->lineEditName->setText(name);
}

QEditCrop::~QEditCrop()
{
    delete ui;
}

//@Override
void QEditCrop::done(int res){
    if(res == QDialog::Accepted){
        QString name = ui->lineEditName->text();
        if(!name.isEmpty()){
            if(create){
                //Create crop
                QStringList names = CDataManager::getAllCropNames();
                if(names.contains(name) || name == "NULL"){
                    ui->statusLabel->setText(tr("The crop name %1 is forbidden!").arg(name));
                    return;
                }else{
                    CDataManager::addCrop(CCrop(name));
                    CCommunicator::saveData();
                    //Update list view
                    new QListWidgetItem(name, this->parent()->findChild<QListWidget*>("listWidget"));
                }
            }else{
                //Update Crop
                QString oldName = this->parent()->findChild<QListWidget*>("listWidget")->currentItem()->text();
                CCrop* toUpdate = CDataManager::getCrop(oldName);
                toUpdate->setName(name);
                //Update listView
                this->parent()->findChild<QListWidget*>("listWidget")->currentItem()->setText(name);
                CCommunicator::saveData();
            }
        }else{
            ui->statusLabel->setText(tr("All input fields need to be filled!"));
            return;
        }
    }

    QDialog::done(res);
}
