#ifndef QCREATEEDITMEASURE_H
#define QCREATEEDITMEASURE_H

#include "Classes/cmeasure.h"
#include <QDialog>

namespace Ui {
class QCreateEditMeasure;
}

class QCreateEditMeasure : public QDialog
{
    Q_OBJECT

public:
    explicit QCreateEditMeasure(QWidget *parent = nullptr, bool create = true);
    explicit QCreateEditMeasure(QWidget *parent = nullptr, QSharedPointer<CMeasure> = nullptr, bool create = false);
    ~QCreateEditMeasure();

    void ini();

private:
    void done(int);

private slots:
    void updateType(int);

private:
    Ui::QCreateEditMeasure *ui;
    bool create;
    int type = -1;
    QSharedPointer<CMeasure> measure;
};

#endif // QCREATEEDITMEASURE_H
