#include "eventdialog.h"
#include "ui_eventdialog.h"

void EventDialog::updateButtons (){
    ui->addAllPushButton->setEnabled(ui->availableStatesListWidget->count() > 0);
    ui->clearAllPushButton->setEnabled(ui->transitions2StatesListWidget->count() > 0);
}
void EventDialog::on_clearAllPushButton_clicked(){
    ui->availableStatesListWidget->addItems(getTransitions());
    ui->transitions2StatesListWidget->clear();
    updateButtons();
}
void EventDialog::on_addAllPushButton_clicked(){
    ui->transitions2StatesListWidget->addItems(availableList());
    ui->availableStatesListWidget->clear();
    updateButtons();
}
void EventDialog::on_availableStatesListWidget_itemSelectionChanged(){
    if (ui->availableStatesListWidget->selectedItems().count() > 0){
        QListWidgetItem *pItem = ui->availableStatesListWidget->selectedItems().first();
        ui->transitions2StatesListWidget->addItem(pItem->text());
        QStringList list = availableList();
        list.removeOne(pItem->text());
        ui->availableStatesListWidget->clear();
        ui->availableStatesListWidget->addItems(list);
        updateButtons();
    }
}
void EventDialog::on_transitions2StatesListWidget_itemSelectionChanged(){
    if (ui->transitions2StatesListWidget->selectedItems().count() > 0){
        QListWidgetItem *pItem = ui->transitions2StatesListWidget->selectedItems().first();
        ui->availableStatesListWidget->addItem(pItem->text());
        QStringList list = getTransitions();
        list.removeOne(pItem->text());
        ui->transitions2StatesListWidget->clear();
        ui->transitions2StatesListWidget->addItems(list);
        updateButtons();
    }
}
QStringList EventDialog::availableList() const {
    QStringList list;
    for (int i = 0; i < ui->availableStatesListWidget->count(); i++){
        list.append (ui->availableStatesListWidget->item(i)->text());
    }
    return list;
}
QStringList EventDialog::getTransitions () const {
    QStringList list;
    for (int i = 0; i < ui->transitions2StatesListWidget->count(); i++){
        list.append (ui->transitions2StatesListWidget->item(i)->text());
    }
    return list;
}
void EventDialog::setStates (QStringList allStates, QStringList selectedStates) {
//    this->m_ExistingStateNamesList = allStates;
    foreach (QString name, allStates) {
        if (selectedStates.contains(name)){
            ui->transitions2StatesListWidget->addItem(name);
        } else {
            ui->availableStatesListWidget->addItem(name);
        }
    }
}
void EventDialog::setNames (QString statename, QString eventname){
    this->setWindowTitle("State: " + statename);
    ui->eventNameLabel->setText(eventname);
}

EventDialog::EventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
}

EventDialog::~EventDialog()
{
    delete ui;
}
