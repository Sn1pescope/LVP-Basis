#ifndef QCOMPLETEMEASURES_H
#define QCOMPLETEMEASURES_H

#include "Classes/cmeasure.h"
#include <QDialog>

namespace Ui {
class QCompleteMeasures;
}

class QCompleteMeasures : public QDialog
{
    Q_OBJECT

public:
    explicit QCompleteMeasures(QWidget *parent = nullptr, std::vector<QSharedPointer<CMeasure>> meas = {});
    ~QCompleteMeasures();

private slots:
    void setDone();
    void scheduleLater();
    void deleteTask();

private:
    Ui::QCompleteMeasures *ui;
};

#endif // QCOMPLETEMEASURES_H
