#include <QFile>
#include <QFileDialog>
#include <QThread>
#include "statedialog.h"
#include "ui_statedialog.h"
#include "StateItem.h"
#include "../LogLib/logger.h"

void StateDialog::setStateNames(QStringList existingNames, QString name){
    this->m_ExistingNamesList = existingNames;
    this->m_InitialName = name;
    ui->stateNameLineEdit->setText(name);
    updateOkButton();

}
void StateDialog::setEvents (QStringList allEvents, QStringList selectedEvents){
    foreach (QString name, allEvents) {
        if (selectedEvents.contains(name)){
            ui->selectedEventsListWidget->addItem(name);
        } else {
            ui->availableEventsListWidget->addItem(name);
        }
    }
    updateButtons();
}
QString StateDialog::getName () const {
    return ui->stateNameLineEdit->text();
}
QStringList StateDialog::getEvents () const {
    QStringList list;
    for (int i = 0; i < ui->selectedEventsListWidget->count(); i++){
        list.append (ui->selectedEventsListWidget->item(i)->text());
    }
    return list;
}
bool StateDialog::isNameValid () const {
    return ui->stateNameLineEdit->text().count() > 0;
}
void StateDialog::updateOkButton (){
    QString text = ui->stateNameLineEdit->text();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled (
        (text == m_InitialName || !this->m_ExistingNamesList.contains(text)) && isNameValid ());
}
void StateDialog::updateButtons (){
    ui->addAllPushButton->setEnabled(ui->availableEventsListWidget->count() > 0);
    ui->clearAllPushButton->setEnabled(ui->selectedEventsListWidget->count() > 0);
}
QStringList StateDialog::availableList() const {
    QStringList list;
    for (int i = 0; i < ui->availableEventsListWidget->count(); i++){
        list.append (ui->availableEventsListWidget->item(i)->text());
    }
    return list;
}
void StateDialog::on_stateNameLineEdit_textChanged(const QString &arg1){
    updateOkButton();
}
void StateDialog::on_availableEventsListWidget_itemSelectionChanged(){
    if (ui->availableEventsListWidget->selectedItems().count() > 0){
        ui->availableEventsListWidget->setEnabled(false);
        QListWidgetItem *pItem = ui->availableEventsListWidget->selectedItems().first();
        ui->selectedEventsListWidget->addItem(pItem->text());
        QStringList list = availableList();
        list.removeOne(pItem->text());
        ui->availableEventsListWidget->clear();
        ui->availableEventsListWidget->addItems(list);
        updateButtons();
        this->thread()->msleep(100);
        ui->availableEventsListWidget->setEnabled(true);
    }
}
void StateDialog::on_selectedEventsListWidget_itemSelectionChanged(){
    if (ui->selectedEventsListWidget->selectedItems().count() > 0){
        QListWidgetItem *pItem = ui->selectedEventsListWidget->selectedItems().first();
        ui->availableEventsListWidget->addItem(pItem->text());
        QStringList list = getEvents();
        list.removeOne(pItem->text());
        ui->selectedEventsListWidget->clear();
        ui->selectedEventsListWidget->addItems(list);
        updateButtons();
    }
}
void StateDialog::on_clearAllPushButton_clicked(){
    ui->availableEventsListWidget->addItems(getEvents());
    ui->selectedEventsListWidget->clear();
    updateButtons();
}
void StateDialog::on_addAllPushButton_clicked(){
    ui->selectedEventsListWidget->addItems(availableList());
    ui->availableEventsListWidget->clear();
    updateButtons();
}
void StateDialog::on_newEventLineEdit_textChanged(const QString &arg1){
    QString str = ui->newEventLineEdit->text();
    ui->newEventButton->setEnabled(str.length() > 0 && !availableList().contains(str) && !getEvents().contains(str));
}
void StateDialog::on_newEventButton_clicked(){
    QString str = ui->newEventLineEdit->text();
    ui->availableEventsListWidget->addItem(str);
    m_ExistingNamesList.append(str);
    m_NewEventsList.append(str);
    ui->newEventLineEdit->clear();
}
QStringList StateDialog::xml2eventlist (QString str){
    QStringList list;
    QDomDocument domdoc;
    domdoc.setContent(str);
    QDomElement root = domdoc.documentElement();
    if (!root.isNull()){
        QDomElement node = root.firstChildElement("state");
        while(!node.isNull()){
            StateItem *pState = StateItem::createState (&node);
            if (pState != nullptr){
                QStringList tmplist = pState->getEventsList();
                list += tmplist;
                delete pState;
                pState = nullptr;
            }
            node = node.nextSiblingElement("state");
        }
    }
    list.removeDuplicates();
    return list;
}

void StateDialog::on_importPushButton_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open state machine"), ".", tr("FSM files (*.fsm)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString str (file.readAll());
        file.close();
        QStringList list = xml2eventlist (str);
        QStringList availablelist = availableList();
        QStringList selectedlist = getEvents();
        foreach (QString eventname, list) {
            if (!availablelist.contains(eventname)
                    && !selectedlist.contains(eventname)
                    && !m_NewEventsList.contains(eventname)){
                ui->availableEventsListWidget->addItem(eventname);
                m_NewEventsList.append(eventname);
            }
        }
    }
}

StateDialog::StateDialog(QWidget *parent)
:   QDialog(parent)
,   ui(new Ui::StateDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    this->setWindowTitle("Edit state");
    ui->stateNameLineEdit->setFocus();
}

StateDialog::~StateDialog()
{
    delete ui;
}
