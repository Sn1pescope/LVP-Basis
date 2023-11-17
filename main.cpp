
#include <QApplication>
#include <Classes/GUI/qmain.h>
#include <Classes/ccommunicator.h>
#include <Classes/GUI/Dialogs/qwelcome.h>
#include <Classes/GUI/Dialogs/qwarningdialog.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //--------------------------- Main Program ----------------------------------

    //Start main program

    //TODO:
    //QTree view with work units -> backend is programmed, overwork gui
    //-> Split, merge

    //Publish beta
    //publish code

    //TODO: Go on with V1 Grundlage


    QMain m;
    m.resize(500, 500);
    return app.exec();
}
