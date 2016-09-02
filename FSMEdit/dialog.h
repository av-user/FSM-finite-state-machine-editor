#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

class QGraphicsItem;
class TestItem;
class StateItem;
class EventItem;
class Scene;

namespace Ui {
	class Dialog;
	}

class Dialog : public QDialog {
    static const int WIN_WIDTH = 5000;
    static const int WIN_HEIGHT = 5000;
    static const int GRID_STEP = 100;
    static const QString EVENT_LIST_FILENAME;
    Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

private slots:
    void on_scaleSlider_valueChanged(int value);
    void showContextMenu(const QPoint& pos);

    void on_pushButton_new_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_save_clicked();
	void on_pushButton_exit_clicked();
    void on_pushButton_saveas_clicked();
    void on_pushButton_import_clicked();
    void on_pushButton_generate_one_clicked();
    void on_pushButton_generate_all_clicked();

    void on_pushButton_generate_lib_clicked();

private:
	void contextMenuView(const QPoint& pos, const QPointF& posScene);
    void contextMenuState(const QPoint& pos, StateItem *pItem);
	void contextMenuEventItem(const QPoint& pos, EventItem *pItem);
    void new_state(const QPointF& pos);
private:
	Ui::Dialog *m_ui;
    QString             m_Filename;
	Scene				*m_pScene;
	QBrush				m_redBrush;
	QBrush				m_blueBrush;
	QPen				m_blackPen;
    QPen				m_grayPen;
    void                setName (QString name);
    void                setFilename (QString filename);
};

#endif // DIALOG_H
