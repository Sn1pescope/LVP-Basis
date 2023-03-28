#include "csecuritymanager.h"

QRegExp CSecurityManager::LETTERS_NUMBERS_SPACERS("[0-9a-zA-Z_]+");
QRegExp CSecurityManager::LETTERS_SPACERS("[a-zA-Z_]+");
QRegExp CSecurityManager::LETTERS_ONLY("[a-zA-Z]+");
QRegExp CSecurityManager::LETTERS_NUMBERS("[a-zA-Z0-9]+");
QRegExp CSecurityManager::NUMBERS_ONLY("[0-9]+");
QRegExp CSecurityManager::NUMBERS_SPACERS("[0-9_]+");
QRegExp CSecurityManager::NUMBERS_AND_POINT("[0-9]+\\.[0-9]+");
QRegExp CSecurityManager::REG_NUMBER("[A-Z][A-Z][A-Z][A-Z][A-Z][A-Z][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]");

CSecurityManager::CSecurityManager()
{

}

bool CSecurityManager::checkInput(QString in, QRegExp rx){
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State st = v.validate(in, pos);
    switch(st){
    case QValidator::Acceptable:
        return true;
    default:
        return false;
    }
    return false;
}
