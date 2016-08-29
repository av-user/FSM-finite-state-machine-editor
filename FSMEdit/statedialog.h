#ifndef STATEDIALOG_H
#define STATEDIALOG_H

#include <QDialog>

namespace Ui {
class StateDialog;
}

class StateDialog : public QDialog
{
    Q_OBJECT

public:
    void setEvents (QStringList allEvents, QStringList selectedEvents);
    void setStateNames (QStringList existingNames, QString name);
    QStringList getEvents () const ;
    QStringList getNewEvents() const { return m_NewEventsList; }
    QString getName () const ;
    static QStringList xml2eventlist (QString str);

    explicit StateDialog(QWidget *parent = 0);
    ~StateDialog();
private slots:
    void on_clearAllPushButton_clicked();
    void on_addAllPushButton_clicked();
    void on_selectedEventsListWidget_itemSelectionChanged();
    void on_stateNameLineEdit_textChanged(const QString &arg1);
    void on_availableEventsListWidget_itemSelectionChanged();
    void on_newEventLineEdit_textChanged(const QString &arg1);
    void on_newEventButton_clicked();
//protected:
//  void showEvent(QShowEvent *);
    void on_importPushButton_clicked();

private:
    QStringList m_ExistingNamesList;
    QStringList m_NewEventsList;
    QString m_InitialName;
    Ui::StateDialog *ui;
    bool isNameValid () const ;
    void updateOkButton ();
    void updateButtons ();
    QStringList availableList() const;
};

#endif // STATEDIALOG_H
