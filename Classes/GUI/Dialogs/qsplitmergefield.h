#ifndef QSPLITMERGEFIELD_H
#define QSPLITMERGEFIELD_H

#include "Classes/cfield.h"
#include <QDialog>

namespace Ui {
class QSplitMergeField;
}

class QSplitMergeField : public QDialog
{
    Q_OBJECT

public:
    explicit QSplitMergeField(QWidget *parent = nullptr);
    QSplitMergeField(QWidget *parent, CField f);
    QSplitMergeField(QWidget *parent, CField f1_par, CField f2_par);
    ~QSplitMergeField();

private:
    void done(int);
    void ini();

private:
    Ui::QSplitMergeField *ui;
    bool merge;
    CField f1;
    CField f2;
};

#endif // QSPLITMERGEFIELD_H
