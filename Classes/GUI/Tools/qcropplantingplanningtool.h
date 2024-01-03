#ifndef QCROPPLANTINGPLANNINGTOOL_H
#define QCROPPLANTINGPLANNINGTOOL_H

#include <QDialog>
#include <QtCharts>
#include <QComboBox>

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

private slots:
    void updateChart();

private:
    Ui::QCropPlantingPlanningTool *ui;
    QPieSeries* series;
    float wholeSize;
    QMap<QString, QString> currSelection;
    QMap<QString, float> sizes;
};

#endif // QCROPPLANTINGPLANNINGTOOL_H
