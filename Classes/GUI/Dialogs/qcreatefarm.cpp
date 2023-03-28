#include "qcreatefarm.h"
#include "ui_qcreatefarm.h"

QCreateFarm::QCreateFarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCreateFarm)
{
    ui->setupUi(this);
    ui->le_Name->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_SPACERS, ui->le_Name));
}

QCreateFarm::~QCreateFarm()
{
    delete ui;
}

//@Override
void QCreateFarm::done(int res){
    if(res == QDialog::Accepted){
        QString name = ui->le_Name->text();
        if(!name.isEmpty()){
            std::vector<QString> names = CDataManager::getAllFarmNames();
            auto it = std::find(names.begin(), names.end(), name);
            if(it == names.end()){
                CCommunicator::createNewFarm(name);
                CCommunicator::loadFarm("LAST");
                CCommunicator::saveData();
            }else{
                ui->labelStatus->setText(tr("Farm %1 already exists!").arg(name));
                return;
            }
        }else{
            ui->labelStatus->setText(tr("All input fields need to be filled!"));
            return;
        }
    }

    QDialog::done(res);
}

