#ifndef QMAIN_H
#define QMAIN_H

#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QComboBox>
#include <QLabel>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QCloseEvent>
#include <QTimer>
#include <Classes/GUI/Dialogs/qwelcome.h>
#include <Classes/GUI/Dialogs/qsettingsdialog.h>
#include <Classes/GUI/Dialogs/qcreatefield.h>
#include <Classes/GUI/Dialogs/qsplitmergefield.h>
#include <Classes/GUI/Overwritten/qmeasurescalendarview.h>

namespace Ui {
class QMain;
}

class QMain : public QMainWindow
{
    Q_OBJECT

    //---------------------------------- @Overwrite -----------------------------------------------
protected:
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent*);

    //---------------------------------- Constructors -----------------------------------------------
public:
    explicit QMain(QWidget *parent = nullptr);
    ~QMain();

    //---------------------------------- Attributes -----------------------------------------------
private:
    Ui::QMain *ui;
    QLabel* lbls[18];
    std::vector<QSharedPointer<CMeasure>> calMeasures;

    //---------------------------------- Methods -----------------------------------------------
private:
    void loadLanguage(const QString& rLanguage);

    //---------------------------------- Slots -----------------------------------------------
protected slots:
    void slotLangBoxChanged(int index);
    void slotWelcomeBoxChanged(int index);

private slots:
    //--------------- General -------------
    void openSettings();
    void save();
    void eventLoopStarted();

    //---------------- Tabs ---------------
    void homeTab();
    void cropPlanning();
    void tasksTab();

    //---------------- Home ---------------
    void updateMainScreenActive();
    void updateCurrentField();

    void newField();
    void editField();
    void deleteField();

    void splitField();
    void mergeFields();
    void setWorkUnit();

    void createFieldList();

    //--------------- Tasks -----------------
    void newMeasure();
    void editMeasure();
    void deleteMeasure();

    void createMeasureList();
    void updateTasksCbEntrys();
    void updateCalendar(int);
    void updateCalendarList();
    void sortMeasureList();
    void sortMeasureList(int);
    void setDone();

    //------------------------------------------------ Signals -------------------------------------------
signals:
    void shutdown();

};

#endif // QMAIN_H
