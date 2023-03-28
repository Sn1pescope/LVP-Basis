#include "qwelcome.h"
#include "ui_qwelcome.h"

QWelcome::QWelcome(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::QWelcome)
{
    ui->setupUi(this);
    CCommunicator::initialize();

    createLanguageMenu();

    complete = false;
    // Try autoload farm or create new
    ui->statusLabel->setText(tr("Try to load farm automatically..."));
    if(CCommunicator::tryAutoload()){
        QString str = CDataManager::getCurrentFarm()->getFarmName();
        ui->statusLabel->setText(tr("Loaded farm %n", str.toLocal8Bit().data()));
        complete = true;
        ui->progressBar->setMaximum(1);
        ui->progressBar->setValue(1);
    }else{
        //Create and load a new Farm
        ui->statusLabel->setText(tr("Please create a new farm."));
    }
}

void QWelcome::done(int res){
    if(res == QDialog::Accepted){
        if(!complete){
            //Create farm
            QCreateFarm dia(this);
            dia.exec();
            complete = true;
        }
    }else if(res ==QDialog::Rejected){
        exit(0);
    }
    QDialog::done(res);
}


//-------------------------- Languages ------------------------------------

//Create menu entries depending on existing translation files
void QWelcome::createLanguageMenu(){

    connect(ui->languageBox, SIGNAL (currentIndexChanged(int)), this->parent(), SLOT (slotWelcomeBoxChanged(int)));

    ui->languageBox->clear();

    QList<QString> entries = CLanguageManager::getMenuEntries();
    for (int i = 0; i < entries.size(); ++i) {
        // get locale extracted by filename
        QString locale;
        locale = entries[i]; // e.g. "de"

        QString lang = QLocale::languageToString(QLocale(locale).language());
        // QIcon ico(QString("%1/%2.png").arg(CDataManager::getLangPath()).arg(locale));

        ui->languageBox->addItem(lang, locale);

        // set default translators and language active
        if (CLanguageManager::getDefaultLocale() == locale) {
            ui->languageBox->setCurrentIndex(i);
        }
    }
}

// Run time translation
void QWelcome::changeEvent(QEvent * e){
    if(e->type() == QEvent::LanguageChange) {
        // this event is send if a translator is loaded
        ui->retranslateUi(this);
    }
    QDialog::changeEvent(e);
}

QWelcome::~QWelcome()
{
    delete ui;
}
