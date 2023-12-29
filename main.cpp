/*
 * @mysterious
 * Changelog LVP-Basis Module:
 *
 * --- V 0.1    "Beta"      :   -added Farm, Field and Crop Systems
 *                              -added Save system
 *
 * ----- Out of release     :   -bugfixes
 *
 * --- V 1.0    "Grundlage" :
*/

#include <QApplication>
#include <Classes/GUI/qmain.h>
#include <Classes/ccommunicator.h>
#include <Classes/GUI/Dialogs/qwelcome.h>
#include <Classes/GUI/Dialogs/qwarningdialog.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //--------------------------- Space for planning ----------------------------------

    //TODO: -crop planting planing tool
    //TODO: Program planning tool

    //TODO: Main screen shows current state of fields (Add label with current year): if crop has planned crops -> show it

    //TODO: Check if saving works everywhere in right directory (use beta for this)




    //----- Release process ------------

    //Search: 'TODO'
    //Check window titles
    //Translate (lupdate, translate, lrelease)
    //Write changelog
    //Write manual, dok, web
    //publish code
    //Release version
    //----------------------------------


    //Start of program
    QMain m;
    m.resize(500, 500);
    return app.exec();
}
