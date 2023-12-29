QT += gui widgets

CONFIG += c++11
CONFIG += app_bundle

QMAKE_LFLAGS += -static-libgcc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Classes/GUI/Dialogs/qcreatefarm.cpp \
        Classes/GUI/Dialogs/qcreatefield.cpp \
        Classes/GUI/Dialogs/qeditcrop.cpp \
        Classes/GUI/Dialogs/qnewworkunit.cpp \
        Classes/GUI/Dialogs/qsettingsdialog.cpp \
        Classes/GUI/Dialogs/qsplitmergefield.cpp \
        Classes/GUI/Dialogs/qwarningdialog.cpp \
        Classes/GUI/Dialogs/qwelcome.cpp \
        Classes/GUI/Dialogs/qworkunitdialog.cpp \
        Classes/GUI/Tools/qcropplantingplanningtool.cpp \
        Classes/GUI/qmain.cpp \
        Classes/ccommunicator.cpp \
        Classes/ccrop.cpp \
        Classes/cdatamanager.cpp \
        Classes/cfarm.cpp \
        Classes/cfield.cpp \
        Classes/cfilemanager.cpp \
        Classes/clanguagemanager.cpp \
        Classes/csecuritymanager.cpp \
        Classes/cworkunit.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Classes/GUI/Dialogs/qcreatefarm.h \
    Classes/GUI/Dialogs/qcreatefield.h \
    Classes/GUI/Dialogs/qeditcrop.h \
    Classes/GUI/Dialogs/qnewworkunit.h \
    Classes/GUI/Dialogs/qsettingsdialog.h \
    Classes/GUI/Dialogs/qsplitmergefield.h \
    Classes/GUI/Dialogs/qwarningdialog.h \
    Classes/GUI/Dialogs/qwelcome.h \
    Classes/GUI/Dialogs/qworkunitdialog.h \
    Classes/GUI/Tools/qcropplantingplanningtool.h \
    Classes/GUI/qmain.h \
    Classes/ccommunicator.h \
    Classes/ccrop.h \
    Classes/cdatamanager.h \
    Classes/cfarm.h \
    Classes/cfield.h \
    Classes/cfilemanager.h \
    Classes/clanguagemanager.h \
    Classes/csecuritymanager.h \
    Classes/cworkunit.h

FORMS += \
    Classes/GUI/Dialogs/qcreatefarm.ui \
    Classes/GUI/Dialogs/qcreatefield.ui \
    Classes/GUI/Dialogs/qeditcrop.ui \
    Classes/GUI/Dialogs/qnewworkunit.ui \
    Classes/GUI/Dialogs/qsettingsdialog.ui \
    Classes/GUI/Dialogs/qsplitmergefield.ui \
    Classes/GUI/Dialogs/qwarningdialog.ui \
    Classes/GUI/Dialogs/qwelcome.ui \
    Classes/GUI/Dialogs/qworkunitdialog.ui \
    Classes/GUI/Tools/qcropplantingplanningtool.ui \
    Classes/GUI/qmain.ui
TRANSLATIONS += \
    languages/LVP-Basis_de.ts \
    languages/LVP-Basis_en.ts

DISTFILES += \
    languages/LVP-Basis_de.qm \
    languages/LVP-Basis_en.qm

RESOURCES += \
    ressources.qrc
