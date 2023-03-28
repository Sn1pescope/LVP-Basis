#ifndef CSECURITYMANAGER_H
#define CSECURITYMANAGER_H

#pragma once

#include <QRegExpValidator>


class CSecurityManager
{
public:
    CSecurityManager();

    static bool checkInput(QString, QRegExp);

public:
    static QRegExp LETTERS_NUMBERS_SPACERS;     //FieldNames
    static QRegExp LETTERS_SPACERS;             //FarmNames
    static QRegExp LETTERS_NUMBERS;             //CropNames
    static QRegExp LETTERS_ONLY;
    static QRegExp NUMBERS_ONLY;
    static QRegExp NUMBERS_SPACERS;
    static QRegExp NUMBERS_AND_POINT;
    static QRegExp REG_NUMBER;
};

#endif // CSECURITYMANAGER_H
