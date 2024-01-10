/*
 * @mysterious
 * Changelog LVP-Basis Module:
 *
 * --- V 0.1    "Beta"      :   -added Farm, Field and Crop Systems
 *                              -added Save system
 *
 * ----- Out of release     :   -bugfixes
 *
 * --- V 1.0    "Grundlage" :   - Added crop planting planning tool
 *                              - Let user load farm at begin
 *                              - Added calendar planning tool and measures system
 *                              - reworked field data saving
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

    //TODO: !!! ATTENTION: ALL DATA OLDER THAN 5 YEARS CANT BE EDITED ANYMORE !!!
    //      state of field,
    //      -> need function to show data out of range too -> In Reports later

    //TODO: Calendar tool to plan measures


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
