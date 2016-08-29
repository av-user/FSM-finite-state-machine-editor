#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>

namespace Ui {
	class ItemDialog;
}

class ItemDialog : public QDialog {
	Q_OBJECT

public:
	explicit ItemDialog(QWidget *parent = 0);
	~ItemDialog();

	std::string getName () const;
	QString getItemName () const;
	std::vector<std::string> getEvents() const;
	QList<QString> getEventList() const;
	void setEvents (QStringList eventList);
	void setExistingNames (QStringList existingNames);
private slots:
	void on_events_listWidget_itemSelectionChanged();

	void on_name_lineEdit_textChanged(const QString &arg1);

private:
	Ui::ItemDialog *ui;
	QStringList m_ExistingNamesList;
	bool isNameValid () const ;
	void updateOkButton ();
};

#endif // ITEMDIALOG_H
