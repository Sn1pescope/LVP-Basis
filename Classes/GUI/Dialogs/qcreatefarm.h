#ifndef QCREATEFARM_H
#define QCREATEFARM_H

#pragma once

#include <QDialog>
#include <Classes/ccommunicator.h>

namespace Ui {
class QCreateFarm;
}

class QCreateFarm : public QDialog
{
    Q_OBJECT

    //---------------------- Constructors ---------------------------
public:
    explicit QCreateFarm(QWidget *parent = nullptr);
    ~QCreateFarm();

    //---------------------- Attributes -------------------------------
private:
    Ui::QCreateFarm *ui;

    //---------------------- Methods ---------------------------------
private:
    void done(int);
};

#endif // QCREATEFARM_H
