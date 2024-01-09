#include "qwarningdialog.h"
#include "ui_qwarningdialog.h"

QWarningDialog::QWarningDialog(QWidget *parent, QString title, QString text, bool okButton, bool cancelButton, bool noButton) :
    QDialog(parent),
    ui(new Ui::QWarningDialog)
{
    ui->setupUi(this);
    ui->label->setText(text);
    this->setWindowTitle(title);
    if(okButton){
        ui->buttonBox->addButton(QDialogButtonBox::Ok);
    }
    if(cancelButton){
        ui->buttonBox->addButton(QDialogButtonBox::Cancel);
    }
    if(noButton){
        ui->buttonBox->addButton(QDialogButtonBox::No);
    }
}

QWarningDialog::QWarningDialog(QWidget *parent, QString text, bool okButton, bool cancelButton) :
    QDialog(parent),
    ui(new Ui::QWarningDialog)
{
    ui->setupUi(this);
    ui->label->setText(text);
    this->setWindowTitle(tr("Warning!"));
    if(okButton){
        ui->buttonBox->addButton(QDialogButtonBox::Ok);
    }
    if(cancelButton){
        ui->buttonBox->addButton(QDialogButtonBox::Cancel);
    }
}

QWarningDialog::QWarningDialog(QWidget *parent, bool okButton, bool cancelButton) :
    QDialog(parent),
    ui(new Ui::QWarningDialog)
{
    ui->setupUi(this);
    ui->label->setText(tr("This is a warning!"));
    this->setWindowTitle(tr("Warning!"));
    if(okButton){
        ui->buttonBox->addButton(QDialogButtonBox::Ok);
    }
    if(cancelButton){
        ui->buttonBox->addButton(QDialogButtonBox::Cancel);
    }
}



QWarningDialog::~QWarningDialog()
{
    delete ui;
}
