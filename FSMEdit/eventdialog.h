#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

public:

    void setStates (QStringList allStates, QStringList selectedStates);
    void setNames (QString statename, QString eventname);
    QStringList getTransitions () const ;

    explicit EventDialog(QWidget *parent = 0);
    ~EventDialog();

private slots:
    void on_clearAllPushButton_clicked();
    void on_addAllPushButton_clicked();
    void on_availableStatesListWidget_itemSelectionChanged();
    void on_transitions2StatesListWidget_itemSelectionChanged();

private:
    Ui::EventDialog *ui;
    bool isNameValid () const ;
    void updateOkButton ();
    void updateButtons ();
    QStringList availableList() const;
};

#endif // EVENTDIALOG_H
