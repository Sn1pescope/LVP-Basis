#ifndef QWARNINGDIALOG_H
#define QWARNINGDIALOG_H

#pragma once

#include <QDialog>

namespace Ui {
class QWarningDialog;
}

class QWarningDialog : public QDialog
{
    Q_OBJECT

    //---------------------------------- Constructors -----------------------------------------------
public:
    explicit QWarningDialog(QWidget *parent = nullptr, QString title = tr("Warning!"), QString text = tr("This is a warning!"), bool okButton = false, bool cancelButton = false);
    explicit QWarningDialog(QWidget *parent = nullptr, QString text = tr("This is a warning!"), bool okButton = false, bool cancelButton = false);
    explicit QWarningDialog(QWidget *parent = nullptr, bool okButton = false, bool cancelButton = false);
    ~QWarningDialog();

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QWarningDialog *ui;
};

#endif // QWARNINGDIALOG_H
