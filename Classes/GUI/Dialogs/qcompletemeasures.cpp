#include "qcompletemeasures.h"
#include "ui_qcompletemeasures.h"

#include "Classes/cdatamanager.h"
#include "Classes/GUI/Dialogs/qwarningdialog.h"

QCompleteMeasures::QCompleteMeasures(QWidget *parent, std::vector<QSharedPointer<CMeasure> > meas) :
    QDialog(parent),
    ui(new Ui::QCompleteMeasures)
{
    ui->setupUi(this);

    //Set list
    ui->tableWidget->hideColumn(4);
    ui->tableWidget->setRowCount(meas.size());
    ui->dateEdit->setDate(QDate::currentDate());
    int i = 0;
    for(QSharedPointer<CMeasure> m : meas){
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(CMeasure::STATES.value(m->getState())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(m->getDate().toString("dd.MM.yyyy")));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(m->getField()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(CMeasure::TYPES.value(m->getType())));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(m->getKey()));
        i++;
    }
    ui->tableWidget->sortItems(1);
}

void QCompleteMeasures::setDone(){
    if(!ui->tableWidget->selectedItems().empty() && ui->tableWidget->selectedItems().at(0)->background() != Qt::darkGray){
        int row = ui->tableWidget->selectedItems().at(0)->row();

        QString f = ui->tableWidget->item(row, 2)->text();
        QString key = ui->tableWidget->item(row, 4)->text();

        QSharedPointer<CMeasure> meas = CDataManager::getCurrentFarm()->getField(f)->getMeasure(key);

        meas->setState(CMeasure::STATE_DONE);

        ui->tableWidget->item(row, 0)->setText(CMeasure::STATES.value(meas->getState()));
        for(QTableWidgetItem* item : ui->tableWidget->selectedItems()){
            item->setBackground(Qt::darkGray);
        }
    }
}

void QCompleteMeasures::scheduleLater(){
    if(!ui->tableWidget->selectedItems().empty() && ui->tableWidget->selectedItems().at(0)->background() != Qt::darkGray){
        int row = ui->tableWidget->selectedItems().at(0)->row();
        QString f = ui->tableWidget->item(row, 2)->text();
        QString key = ui->tableWidget->item(row, 4)->text();
        QSharedPointer<CMeasure> meas = CDataManager::getCurrentFarm()->getField(f)->getMeasure(key);
        QDate date = meas->getDate();
        QDate later = ui->dateEdit->date();
        if(later < date){
            //Later is to early
            QWarningDialog(this, tr("You need to specifie a date in the future!"), true).exec();
            return;
        }
        meas->setDate(later);
        ui->tableWidget->item(row, 1)->setText(meas->getDate().toString("dd.MM.yyyy"));
        ui->tableWidget->sortItems(1);
        for(QTableWidgetItem* item : ui->tableWidget->selectedItems()){
            item->setBackground(Qt::darkGray);
        }
    }
}

void QCompleteMeasures::deleteTask(){
    if(!ui->tableWidget->selectedItems().empty() && ui->tableWidget->selectedItems().at(0)->background() != Qt::darkGray){
        int row = ui->tableWidget->selectedItems().at(0)->row();
        QString f = ui->tableWidget->item(row, 2)->text();
        QString key = ui->tableWidget->item(row, 4)->text();
        if(QWarningDialog(this, tr("Do you really want to delete this measure?\nIt can't be restored!"), true, true).exec()){
            CDataManager::getCurrentFarm()->getField(f)->removeMeasure(key);
            ui->tableWidget->removeRow(row);
        }
    }
}

QCompleteMeasures::~QCompleteMeasures()
{
    delete ui;
}
