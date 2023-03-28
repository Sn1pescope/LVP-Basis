#include "clanguagemanager.h"

QList<QString> CLanguageManager::menuEntries;
int CLanguageManager::currentLanguage = -1;
QString CLanguageManager::defaultLocale = "en";

QTranslator* CLanguageManager::m_translator_de = new QTranslator();
QTranslator* CLanguageManager::m_translator_en = new QTranslator();

const int CLanguageManager::LANG_DE;
const int CLanguageManager::LANG_EN;

CLanguageManager::CLanguageManager()
{

}

void CLanguageManager::init(){
    menuEntries.insert(LANG_DE, "de");
    menuEntries.insert(LANG_EN, "en");

    // format systems language
    defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));// e.g. "de"

    QString loadedLocale = CFileManager::getDefaultLocale();
    if(loadedLocale != ""){
        defaultLocale = loadedLocale;
    }

    m_translator_de->load(QStringLiteral(":/languages/LVP-Basis_de.qm"));
    m_translator_en->load(QStringLiteral(":/languages/LVP-Basis_en.qm"));
}

QTranslator* CLanguageManager::loadLanguage(const QString& rLanguage) {
    int rLang = 0;
    while(rLang < menuEntries.size()){
        if(menuEntries[rLang] == rLanguage){
            break;
        }

        rLang++;
    }

    if(currentLanguage != rLang) {
        currentLanguage = rLang;
        defaultLocale = menuEntries[rLang];

        switch(currentLanguage){
        case LANG_DE: {return m_translator_de;}
        case LANG_EN: {return m_translator_en;}
        default: {return m_translator_en;}
        }
    }
    return nullptr;
}

void CLanguageManager::updateTranslator() {
    // load the new translator
    switch(currentLanguage){
    case LANG_DE: {qApp->installTranslator(m_translator_de);}
    case LANG_EN: {qApp->installTranslator(m_translator_en);}
    }
}


//-------------------------- Getter ---------------------------------

QList<QString> CLanguageManager::getMenuEntries(){
    return menuEntries;
}

QString CLanguageManager::getDefaultLocale(){
    return defaultLocale;
}


//-------------------------- Setter -----------------------------------

