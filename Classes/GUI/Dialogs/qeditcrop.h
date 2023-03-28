#ifndef QEDITCROP_H
#define QEDITCROP_H

#pragma once

#include <QDialog>
#include <QRegExpValidator>
#include <QListWidget>
#include <Classes/csecuritymanager.h>
#include <Classes/cdatamanager.h>
#include <Classes/ccommunicator.h>

namespace Ui {
class QEditCrop;
}

class QEditCrop : public QDialog
{
    Q_OBJECT

    //---------------------------------- Constructors -----------------------------------------------
public:
    explicit QEditCrop(QWidget *parent = nullptr, bool create = true);
    explicit QEditCrop(QWidget *parent = nullptr, QString name = "", bool create = false);
    ~QEditCrop();

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QEditCrop *ui;
    bool create = false;

    //---------------------------------- Methods -----------------------------------------------
private:
    void done(int);
};

#endif // QEDITCROP_H
