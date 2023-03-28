#ifndef CLANGUAGEMANAGER_H
#define CLANGUAGEMANAGER_H

#pragma once

#include <QTranslator>
#include <QList>
#include <QApplication>
#include <Classes/cfilemanager.h>

class CLanguageManager
{

public:
    CLanguageManager();
    static void init();
    static QTranslator* loadLanguage(const QString& rLanguage);

    static QList<QString> getMenuEntries();
    static QString getDefaultLocale();

    static const int LANG_DE = 0;
    static const int LANG_EN = 1;

private:
    static void updateTranslator();

    static QList<QString> menuEntries;

    static int currentLanguage;
    static QString defaultLocale;

    static QTranslator* m_translator_de;
    static QTranslator* m_translator_en;
};

#endif // CLANGUAGEMANAGER_H
