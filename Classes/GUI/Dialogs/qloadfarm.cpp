#include "qloadfarm.h"
#include "ui_qloadfarm.h"
#include "Classes/cdatamanager.h"
#include "Classes/ccommunicator.h"
#include "Classes/GUI/Dialogs/qwarningdialog.h"

QLoadFarm::QLoadFarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QLoadFarm)
{
    ui->setupUi(this);

    //Init farm list
    ui->listWidget->clear();
    std::vector<QString> farmNames = CDataManager::getAllFarmNames();
    foreach (QString name, farmNames) {
        new QListWidgetItem(name, ui->listWidget);
    }
}

void QLoadFarm::done(int res){
    if(res == QDialog::Accepted){
        //Load selected farm
        if(ui->listWidget->currentItem() != nullptr){
            QString name = ui->listWidget->currentItem()->text();
            if(name != CDataManager::getCurrentFarmName()){
                //Close open Farm
                if(CDataManager::getCurrentFarmName() != ""){
                    CCommunicator::closeFarm();
                }
                //Load farm
                if(!CCommunicator::loadFarm(name)){
                    QWarningDialog w(this, tr("Error"), tr("Unable to load farm! Try again!"), true, false);
                    w.exec();
                    return;
                }
            }
        }
    }
    QDialog::done(res);
}

QLoadFarm::~QLoadFarm()
{
    delete ui;
}
