#include "qcreateeditmeasure.h"
#include "Classes/GUI/Dialogs/qwarningdialog.h"
#include "Classes/cdatamanager.h"
#include "ui_qcreateeditmeasure.h"

QCreateEditMeasure::QCreateEditMeasure(QWidget *parent, bool create) :
    QDialog(parent),
    ui(new Ui::QCreateEditMeasure)
{
    ui->setupUi(this);
    this->create = create;
    ini();
}

QCreateEditMeasure::QCreateEditMeasure(QWidget *parent, QSharedPointer<CMeasure> meas, bool create) :
    QDialog(parent),
    ui(new Ui::QCreateEditMeasure)
{
    ui->setupUi(this);
    this->create = create;
    this->measure = meas;
    ini();

    //Autofill
    ui->cbField->setCurrentText(meas->getField());
    ui->cbState->setCurrentIndex(meas->getState());
    ui->deDate->setDate(meas->getDate());
    ui->cbType->setCurrentIndex(meas->getType());
    switch(meas->getType()){
    case CMeasure::TYPE_CULTIVATING:
        //TODO: Fill specific input fields
        break;
    case CMeasure::TYPE_SEEDING:
        //TODO: Fill specific input fields
        break;
    case CMeasure::TYPE_FERTILIZING:
        //TODO: Fill specific input fields
        break;
    case CMeasure::TYPE_SPRAYING:
        //TODO: Fill specific input fields
        break;
    case CMeasure::TYPE_HARVESTING:
        //TODO: Fill specific input fields
        break;
    default:
        break;
    }
}

void QCreateEditMeasure::ini(){
    QString title;
    if(create){
        title = tr("Create ");
    }else{
        title = tr("Edit ");
    }
    title = title + tr("Measure");
    this->setWindowTitle(title);
    //Validators and standard values

    //Field
    ui->cbField->clear();
    std::vector<QString> fields = CDataManager::getCurrentFarm()->getAllFieldNames();
    foreach(QString field, fields){
        ui->cbField->addItem(field);
    }
    //States
    ui->cbState->clear();
    QMap<int, QString> states = CMeasure::STATES;
    foreach(int key, states.keys()){
        ui->cbState->insertItem(key, states.value(key));
    }
    ui->cbState->setCurrentIndex(CMeasure::STATE_PLANNED);
    //Date
    ui->deDate->setDate(QDate::currentDate());
    //Type
    ui->cbType->clear();
    QMap<int, QString> types = CMeasure::TYPES;
    foreach(int key, types.keys()){
        ui->cbType->insertItem(key, types.value(key));
    }
    ui->cbType->setCurrentIndex(0);
}

void QCreateEditMeasure::updateType(int to){
    if(to != type){
        //Update
        type = to;
        switch (type) {
        case CMeasure::TYPE_CULTIVATING:
            //TODO: Show specific input fields in gbType
            break;
        case CMeasure::TYPE_SEEDING:
            //TODO: Show specific input fields in gbType
            break;
        case CMeasure::TYPE_FERTILIZING:
            //TODO: Show specific input fields in gbType
            break;
        case CMeasure::TYPE_SPRAYING:
            //TODO: Show specific input fields in gbType
            break;
        case CMeasure::TYPE_HARVESTING:
            //TODO: Show specific input fields in gbType
            break;
        default:
            break;
        }
    }
}

void QCreateEditMeasure::done(int res){
    if(res == QDialog::Accepted){
        //Get values and create or edit measure
        CField* field = CDataManager::getCurrentFarm()->getField(ui->cbField->currentText());
        int state = ui->cbState->currentIndex();
        QDate date = ui->deDate->date();
        int typ = ui->cbType->currentIndex();
        if(state < 0 || typ < 0 || !date.isValid() || field == nullptr){
            QWarningDialog(this, tr("You need to fill all fields!"), true).exec();
            return;
        }
        switch(typ){
        case CMeasure::TYPE_CULTIVATING:
            //TODO: Get specific input fields and check them
            if(create){
                field->addMeasure(QSharedPointer<CMeasure>(new CMeasureCultivating(field->getName(), state, date, typ)));
            }else{
                measure->setDate(date);
                measure->setType(typ);
                measure->setState(state);
                measure->setField(field->getName());
            }
            break;
        case CMeasure::TYPE_SEEDING:
            //TODO: Get specific input fields and check them
            if(create){
                field->addMeasure(QSharedPointer<CMeasure>(new CMeasureSeeding(field->getName(), state, date, typ)));
            }else{
                measure->setDate(date);
                measure->setType(typ);
                measure->setState(state);
                measure->setField(field->getName());
            }
            break;
        case CMeasure::TYPE_FERTILIZING:
            //TODO: Get specific input fields and check them
            if(create){
                field->addMeasure(QSharedPointer<CMeasure>(new CMeasureFertilizing(field->getName(), state, date, typ)));
            }else{
                measure->setDate(date);
                measure->setType(typ);
                measure->setState(state);
                measure->setField(field->getName());
            }
            break;
        case CMeasure::TYPE_SPRAYING:
            //TODO: Get specific input fields and check them
            if(create){
                field->addMeasure(QSharedPointer<CMeasure>(new CMeasureSpraying(field->getName(), state, date, typ)));
            }else{
                measure->setDate(date);
                measure->setType(typ);
                measure->setState(state);
                measure->setField(field->getName());
            }
            break;
        case CMeasure::TYPE_HARVESTING:
            //TODO: Get specific input fields and check them
            if(create){
                field->addMeasure(QSharedPointer<CMeasure>(new CMeasureHarvesting(field->getName(), state, date, typ)));
            }else{
                measure->setDate(date);
                measure->setType(typ);
                measure->setState(state);
                measure->setField(field->getName());
            }
            break;
        default:
            break;
        }

    }
    QDialog::done(res);
}

QCreateEditMeasure::~QCreateEditMeasure()
{
    delete ui;
}
