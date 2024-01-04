#include "qcropplantingplanningtool.h"
#include "Classes/cdatamanager.h"
#include "ui_qcropplantingplanningtool.h"

QCropPlantingPlanningTool::QCropPlantingPlanningTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCropPlantingPlanningTool)
{
    ui->setupUi(this);

    //TODO: Edit chart display mode
    auto chart = new QChart;
    auto view = new QChartView;
    series = new QPieSeries();

    chart->setTitle(tr("Percentages of crops planned"));
    chart->legend()->setVisible(true);

    chart->addSeries(series);
    view->setChart(chart);

    ui->splitter->addWidget(view);

    //Fill table with data
    ui->tableWidget->setColumnCount(12);
    //Table horizontal header
    int num = 0;
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
        //Generate text
        QString text;
        if(i < 2){
            text += tr("Planned ");
        }
        if(i % 2 != 0){
            //Ungerade
            text += tr("Intercrop");
        }else{
            text += tr("Crop");
            num++;
        }
        if(i > 1){
            text += tr(" %1 year ago").arg(num-1);
        }
        //Add header
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(text));
    }

    //Table data
    std::vector<CField> fields = CDataManager::getCurrentFarm()->getAllFields();
    for (int j = 0; j < (int)fields.size(); j++){
        CField f = fields.at(j);
        //Add row header and row data
        ui->tableWidget->insertRow(j);
        ui->tableWidget->setVerticalHeaderItem(j, new QTableWidgetItem(f.getName() + "\n" + QString::number(f.getSize()) + " ha\n" + f.getRegNumber()));
        float size = f.getSize();

        //Add combo boxes and set them enabled
        QComboBox* cb = new QComboBox();
        QComboBox* cbs = new QComboBox();

        cb->addItem("");
        cbs->addItem("");

        for(QString name : CDataManager::getAllCropNames()){
            cb->addItem(name);
            cbs->addItem(name);
        }
        CCrop* lastCrop = f.getNextCrop();
        if(lastCrop != nullptr){
            cb->setCurrentText(lastCrop->getName());
        }
        currSelection.insert(f.getName(), cb->currentText());
        CCrop* lc = f.getNextInterCrop();
        if(lc != nullptr){
            cbs->setCurrentText(lc->getName());
        }
        cb->setProperty("field", f.getName());
        cb->setProperty("row", j);
        cb->setProperty("column", 0);
        cb->setEnabled(!f.isCropPlanted());
        cb->setProperty("fieldSize", size);

        cbs->setProperty("field", f.getName());
        cbs->setProperty("row", j);
        cbs->setProperty("column", 1);
        cbs->setEnabled(!f.isInterCropPlanted());

        ui->tableWidget->setCellWidget(j, 0, cb);
        ui->tableWidget->setCellWidget(j, 1, cbs);

        connect(cb, &QComboBox::currentTextChanged, this, &QCropPlantingPlanningTool::updateChart);
        emit cb->currentTextChanged("");
        //connect(cbs, &QComboBox::currentTextChanged, this, &QCropPlantingPlanningTool::updateChart);

        //Add last crops to table
        for(int i = 0; i < 5; i++){
            //For every table cell;
            //getLastCrop can be zero
            CCrop* lastCrop = f.getLastCrop(i);
            if(lastCrop != nullptr){
                ui->tableWidget->setItem(j, i+2, new QTableWidgetItem(lastCrop->getName()));
            }
            //getLastInterCrop can be zero
            CCrop* lastInterCrop = f.getLastInterCrop(i);
            if(lastInterCrop != nullptr){
                ui->tableWidget->setItem(j, i+3, new QTableWidgetItem(lastInterCrop->getName()));
            }
        }
    }
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();

}

void QCropPlantingPlanningTool::updateChart(){
    //Update chart
    int row = sender()->property("row").toInt();
    int col = sender()->property("column").toInt();
    float size = sender()->property("fieldSize").toFloat();
    QString field = sender()->property("field").toString();
    QComboBox* c = (QComboBox*)ui->tableWidget->cellWidget(row, col);
    QString crop = c->currentText();
    if(crop == ""){
        crop = "--";
    }
    QString oldCrop = currSelection.value(field);
    if(oldCrop == ""){
        oldCrop = "--";
    }
    float oldVal = 0;
    float currVal = 0;

    series->clear();

    oldVal = sizes.value(oldCrop);
    currVal = sizes.value(crop);

    //Reduce old crop value
    oldVal = oldVal - size;
    if(oldVal < 0){
        oldVal = 0;
    }

    //Increase new crop value
    currVal = currVal + size;

    //Set size map

    if(oldCrop == crop){
        sizes.remove(oldCrop);
        sizes.insert(oldCrop, currVal);
    }else{
        sizes.remove(oldCrop);
        sizes.insert(oldCrop, oldVal);
        sizes.remove(crop);
        sizes.insert(crop, currVal);
    }

    //Set Map
    currSelection.remove(field);
    currSelection.insert(field, crop);

    //Readd slices
    QStringList keys = sizes.keys();
    for(int i = 0; i < keys.size(); i++){
        QString key = keys.at(i);
        series->append(key, sizes.value(key));
    }

    for(int i = 0; i < series->slices().size(); i++){
        QString label = series->slices().at(i)->label();
        //series->slices().at(i)->setLabel(QString::number(100 * series->slices().at(i)->percentage()));
        series->chart()->legend()->markers(series)[i]->setLabel(label + ": " + QString::number(100 * series->slices().at(i)->percentage()) + "%");
    }
    series->chart()->legend()->setAlignment(Qt::AlignRight);
}

QCropPlantingPlanningTool::~QCropPlantingPlanningTool()
{
    delete ui;
}

void QCropPlantingPlanningTool::done(int res){
    if(res == QDialog::Accepted){
        //Get for every row the cbs, get their field property, set data to field
        for(int i = 0; i < ui->tableWidget->rowCount(); i++){
            QComboBox* cb = (QComboBox*)ui->tableWidget->cellWidget(i, 0);
            QComboBox* cbInter = (QComboBox*)ui->tableWidget->cellWidget(i, 1);
            QString fieldName = cb->property("field").toString();
            QString nextCrop = cb->currentText();
            QString nextInter = cbInter->currentText();
            CDataManager::getCurrentFarm()->getField(fieldName)->setNextCrop(CDataManager::getCrop(nextCrop));
            CDataManager::getCurrentFarm()->getField(fieldName)->setNextInterCrop(CDataManager::getCrop(nextInter));
        }
    }
    QDialog::done(res);
}
