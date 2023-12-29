#include "qcropplantingplanningtool.h"
#include "ui_qcropplantingplanningtool.h"

QCropPlantingPlanningTool::QCropPlantingPlanningTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QCropPlantingPlanningTool)
{
    ui->setupUi(this);
}

QCropPlantingPlanningTool::~QCropPlantingPlanningTool()
{
    delete ui;
}
