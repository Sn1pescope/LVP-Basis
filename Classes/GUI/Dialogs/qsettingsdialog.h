#ifndef QSETTINGSDIALOG_H
#define QSETTINGSDIALOG_H

#pragma once

#include <QDialog>
#include <QFileDialog>
#include <Classes/clanguagemanager.h>
#include <Classes/cdatamanager.h>
#include <Classes/GUI/Dialogs/qwarningdialog.h>
#include <Classes/GUI/Dialogs/qeditcrop.h>
#include <Classes/GUI/Dialogs/qcreatefarm.h>

namespace Ui {
class QSettingsDialog;
}

class QSettingsDialog : public QDialog
{
    Q_OBJECT

    //---------------------------------- Constructors -----------------------------------------------
public:
    explicit QSettingsDialog(QWidget *parent = nullptr);
    ~QSettingsDialog();

    //---------------------------------- Methods -----------------------------------------------
protected:
    //@Overwrite
    void changeEvent(QEvent*);

private:
    void createLanguageMenu();
    void createCropList();
    void createFarmList();

    //@Overwrite
    void done(int);

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QSettingsDialog *ui;

    //---------------------------------- Slots -----------------------------------------------
private slots:
    void updateEditTab();

    void delCrop();
    void editCrop();
    void newCrop();

    void newFarm();
    void delFarm();
    void closeFarm();
    void loadFarm();
    void openFarm();
};

#endif // QSettingsDialog_H
