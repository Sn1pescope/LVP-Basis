#ifndef QNEWWORKUNIT_H
#define QNEWWORKUNIT_H

#include <QDialog>
#include <QRegExpValidator>
#include "Classes/cdatamanager.h"

namespace Ui {
class QNewWorkUnit;
}

class QNewWorkUnit : public QDialog
{
    Q_OBJECT

public:
    explicit QNewWorkUnit(QWidget *parent = nullptr);
    ~QNewWorkUnit();

private:
    void done(int);

private:
    Ui::QNewWorkUnit *ui;
};

#endif // QNEWWORKUNIT_H
