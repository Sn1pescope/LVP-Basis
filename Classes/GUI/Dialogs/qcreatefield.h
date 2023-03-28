#ifndef QCREATEFIELD_H
#define QCREATEFIELD_H

#include <QDialog>
#include <QGroupBox>
#include <QDate>
#include <QComboBox>
#include <QListWidget>
#include <QCheckBox>
#include <Classes/cdatamanager.h>
#include <Classes/GUI/Dialogs/qwarningdialog.h>
#include <Classes/ccommunicator.h>
#include <Classes/cfield.h>
#include <Classes/csecuritymanager.h>

namespace Ui {
class QCreateField;
}

class QCreateField : public QDialog
{
    Q_OBJECT

    //---------------------------------- Constructors ---------------------------------------------
public:
    explicit QCreateField(QWidget *parent = nullptr, bool create = true);
    explicit QCreateField(QWidget *parent = nullptr, QString name = "", bool create = false);
    ~QCreateField();

    //---------------------------------- Methods --------------------------------------------------
public:
    void ini();

private:
    void done(int);

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QCreateField *ui;
    bool create;
    QString oldName;
    QComboBox* cbs[10];
    QWidget* widgets[10];
    QGroupBox* gbs[5];
    QCheckBox* chbs[5];

};

#endif // QCREATEFIELD_H
