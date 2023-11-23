#ifndef QWORKUNITDIALOG_H
#define QWORKUNITDIALOG_H

#include <QDialog>

namespace Ui {
class QWorkUnitDialog;
}

class QWorkUnitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QWorkUnitDialog(QWidget *parent = nullptr, QString fieldName = "");
    ~QWorkUnitDialog();

private:
    void done(int);
    void refreshCB();

private slots:
    void newWu();

private:
    Ui::QWorkUnitDialog *ui;
    QString field;
};

#endif // QWORKUNITDIALOG_H
