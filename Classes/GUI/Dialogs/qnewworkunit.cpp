#include "qnewworkunit.h"
#include "Classes/GUI/Dialogs/qwarningdialog.h"
#include "Classes/csecuritymanager.h"
#include "ui_qnewworkunit.h"

QNewWorkUnit::QNewWorkUnit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QNewWorkUnit)
{
    ui->setupUi(this);

    ui->leName->setValidator(new QRegExpValidator(CSecurityManager::LETTERS_NUMBERS_SPACERS));
}

void QNewWorkUnit::done(int res){
    if(res == QDialog::Accepted){
        QString name = ui->leName->text();
        if(!name.isEmpty()){
            std::vector<CWorkUnit> wus = CDataManager::getCurrentFarm()->getAllWorkUnits();
            auto it = std::find(wus.begin(), wus.end(), CWorkUnit(name));
            if(it != wus.end()){
                QWarningDialog(this, tr("Work Unit %1 already exists, choose another name!").arg(name), true).exec();
                return;
            }
            CDataManager::getCurrentFarm()->addWorkUnit(CWorkUnit(name));
        }
    }
    QDialog::done(res);
}

QNewWorkUnit::~QNewWorkUnit()
{
    delete ui;
}
