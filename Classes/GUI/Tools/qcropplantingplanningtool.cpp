#include "qcropplantingplanningtool.h"
#include "ui_qcropplantingplanningtool.h"

QCropPlantingPlanningTool::QCropPlantingPlanningTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCropPlantingPlanningTool)
{
    ui->setupUi(this);

    //TODO: Edit data series dynamically
    auto chart = new QChart;
    auto view = new QChartView;
    QPieSeries* series = new QPieSeries();
    series->append("Weizen", 20);
    series->append("Roggen", 10);
    chart->addSeries(series);
    view->setChart(chart);

    ui->horizontalLayout->addWidget(view);

    //TODO: Fill table with data
}

QCropPlantingPlanningTool::~QCropPlantingPlanningTool()
{
    delete ui;
}

void QCropPlantingPlanningTool::done(int res){
    if(res == QDialog::Accepted){
        //TODO: Write data to every field
    }
    QDialog::done(res);
}
