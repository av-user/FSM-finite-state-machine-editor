#include "itemdialog.h"
#include "ui_itemdialog.h"
#include <QPushButton>
#include "../LogLib/logger.h"

std::string ItemDialog::getName () const {
	return std::string(this->ui->name_lineEdit->text().toStdString());
}
QString ItemDialog::getItemName() const {
	return this->ui->name_lineEdit->text();
}
std::vector<std::string> ItemDialog::getEvents() const {
	std::vector<std::string> events;
	foreach (QListWidgetItem* item, ui->events_listWidget->selectedItems()){
		events.insert (events.end(), item->text().toStdString());
	}
	return events;
}
QList<QString> ItemDialog::getEventList() const {
	QList<QString> events;
	foreach (QListWidgetItem* item, ui->events_listWidget->selectedItems()){
		events.insert (events.end(), item->text());
	}
	return events;
}
void ItemDialog::setEvents(QStringList eventList){
	ui->events_listWidget->clear();
	ui->events_listWidget->addItems(eventList);
}
void ItemDialog::setExistingNames(QStringList existingNamesList){
	this->m_ExistingNamesList = existingNamesList;
}
void ItemDialog::updateOkButton (){
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled (!ui->events_listWidget->selectedItems().empty()
	&& isNameValid ()
	&& !this->m_ExistingNamesList.contains(ui->name_lineEdit->text()));
}
void ItemDialog::on_events_listWidget_itemSelectionChanged(){
	updateOkButton();
}
void ItemDialog::on_name_lineEdit_textChanged(const QString &arg1){
	updateOkButton();
}
bool ItemDialog::isNameValid() const {
	return ui->name_lineEdit->text().count() > 0;
}

ItemDialog::ItemDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ItemDialog)
{
	ui->setupUi(this);
	ui->events_listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
ItemDialog::~ItemDialog()
{
	delete ui;
}
