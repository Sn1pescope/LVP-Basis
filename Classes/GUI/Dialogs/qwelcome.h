#ifndef QWELCOME_H
#define QWELCOME_H

#pragma once

#include <QDialog>
#include <QActionGroup>
#include <Classes/ccommunicator.h>
#include <Classes/GUI/Dialogs/qcreatefarm.h>
#include <Classes/GUI/Dialogs/qwarningdialog.h>
#include <Classes/GUI/qmain.h>
#include <QDialogButtonBox>
#include <QComboBox>

namespace Ui {
class QWelcome;
}

class QWelcome : public QDialog
{
    Q_OBJECT

    //---------------------------------- Constructors -----------------------------------------------
public:
    explicit QWelcome(QWidget* parent = nullptr);
    ~QWelcome();

    //---------------------------------- Methods -----------------------------------------------
protected:
    //@Overwrite
    void changeEvent(QEvent*);

private:
    void createLanguageMenu();
    //@Overwrite
    void done(int);

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QWelcome *ui;
    bool complete;

};

#endif // QWELCOME_H
