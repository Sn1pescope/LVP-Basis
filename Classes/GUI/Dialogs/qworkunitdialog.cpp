#include "qworkunitdialog.h"
#include "Classes/GUI/Dialogs/qnewworkunit.h"
#include "Classes/cdatamanager.h"
#include "ui_qworkunitdialog.h"

QWorkUnitDialog::QWorkUnitDialog(QWidget *parent, QString fieldName) :
    QDialog(parent),
    ui(new Ui::QWorkUnitDialog)
{
    ui->setupUi(this);
    ui->label_2->setText(tr("Select Work Unit of field '%1':").arg(fieldName));
    field = fieldName;
    refreshCB();
}

void QWorkUnitDialog::refreshCB(){
    ui->comboBox->clear();
    std::vector<CWorkUnit> wus = CDataManager::getCurrentFarm()->getAllWorkUnits();
    ui->comboBox->addItem("");
    foreach(CWorkUnit w, wus){
        ui->comboBox->addItem(w.getName());
    }
    QString wu = CDataManager::getCurrentFarm()->getWorkUnitOfField(field);
    ui->comboBox->setCurrentText(wu);
}

void QWorkUnitDialog::newWu(){
    QNewWorkUnit(this).exec();
    refreshCB();
}

QWorkUnitDialog::~QWorkUnitDialog()
{
    delete ui;
}

void QWorkUnitDialog::done(int res){
    if(res == QDialog::Accepted){
        QString wu = ui->comboBox->currentText();
        QString oldWu = CDataManager::getCurrentFarm()->getWorkUnitOfField(field);
        if(wu != oldWu){
            CDataManager::getCurrentFarm()->removeFieldFromWorkUnit(field, oldWu);
            CDataManager::getCurrentFarm()->addFieldToWorkUnit(field, wu);
        }
    }

    QDialog::done(res);
}
