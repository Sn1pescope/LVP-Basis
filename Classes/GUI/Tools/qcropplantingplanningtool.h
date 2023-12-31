#ifndef QCROPPLANTINGPLANNINGTOOL_H
#define QCROPPLANTINGPLANNINGTOOL_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class QCropPlantingPlanningTool;
}

class QCropPlantingPlanningTool : public QDialog
{
    Q_OBJECT

public:
    explicit QCropPlantingPlanningTool(QWidget *parent = nullptr);
    ~QCropPlantingPlanningTool();

private:
    void done(int);

private:
    Ui::QCropPlantingPlanningTool *ui;
};

#endif // QCROPPLANTINGPLANNINGTOOL_H
