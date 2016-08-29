#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hostserver.h"

namespace Ui {
	class MainWindow;
	}

class QStandardItemModel;

class MainWindow : public QMainWindow{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void add_output_msg (QString s0, QString s1, QString s2);
	void add_input_msg (QString s0, QString s1, QString s2);
	void on_pushButton_exit_clicked();

	void on_pushButton_listen_clicked();

private:
	Ui::MainWindow *ui;
	QStandardItemModel *m_pModel;
	HostServer m_Server;
};

#endif // MAINWINDOW_H
