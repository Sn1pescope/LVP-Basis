#ifndef QCROPPLANTINGPLANNINGTOOL_H
#define QCROPPLANTINGPLANNINGTOOL_H

#include <QDialog>

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
    Ui::QCropPlantingPlanningTool *ui;
};

#endif // QCROPPLANTINGPLANNINGTOOL_H
