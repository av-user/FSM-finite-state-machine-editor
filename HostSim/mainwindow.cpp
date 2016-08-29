#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QDebug>
#include "../LogLib/logger.h"

void MainWindow::on_pushButton_listen_clicked(){
	Log::Logger::trace("HOSTSIM", "listen button clicked");
	if (m_Server.isListening()){
		Log::Logger::trace("HOSTSIM", "socket already listening");
	} else {
		QString ipAddress;
		QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		// use the first non-localhost IPv4 address
		QHostAddress ha;
		for (int i = 0; i < ipAddressesList.size(); ++i) {
			if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
				ipAddressesList.at(i).toIPv4Address()) {
				ha = ipAddressesList.at(i);
				ipAddress = ipAddressesList.at(i).toString();
				Log::Logger::traceV("HOSTSIM", "address %s selected", ipAddress.toStdString().c_str());
				break;
			}
		}
		// if we did not find one, use IPv4 localhost
		if (ipAddress.isEmpty()){
			ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
			Log::Logger::trace("HOSTSIM", "localhost selected");
			ha = QHostAddress::LocalHost;
		}
		qDebug() << ipAddress << "; " << m_Server.serverPort();
		if (m_Server.listen(ha, 2067)){
			Log::Logger::traceV("HOSTSIM", "server port: %u", m_Server.serverPort());
			qDebug() << ipAddress << "; " << m_Server.serverPort();
		} else {
			QMessageBox::critical(this, tr("Threaded Fortune Server"),
										 tr("Unable to start the server: %1.")
										 .arg(m_Server.errorString()));
				   close();
		}
	}
}
void MainWindow::add_output_msg (QString s0, QString s1, QString s2){
	Log::Logger::traceV("HOSTSIM", "add_output_msg: %s; %s; %s", s0.toStdString().c_str()
		, s1.toStdString().c_str(), s2.toStdString().c_str());
	m_pModel->insertRow(m_pModel->rowCount());
	m_pModel->setItem(m_pModel->rowCount()-1, 0, new QStandardItem(s0));
	m_pModel->setItem(m_pModel->rowCount()-1, 1, new QStandardItem(s1));
	m_pModel->setItem(m_pModel->rowCount()-1, 2, new QStandardItem(s2));

}
void MainWindow::add_input_msg (QString s0, QString s1, QString s2){
	Log::Logger::traceV("HOSTSIM", "add_input_msg: %s; %s; %s", s0.toStdString().c_str()
		, s1.toStdString().c_str(), s2.toStdString().c_str());
	m_pModel->insertRow(m_pModel->rowCount());
	m_pModel->setItem(m_pModel->rowCount()-1, 0, new QStandardItem(s0));
	m_pModel->setItem(m_pModel->rowCount()-1, 1, new QStandardItem(s1));
	m_pModel->setItem(m_pModel->rowCount()-1, 2, new QStandardItem(s2));

}

void MainWindow::on_pushButton_exit_clicked(){
	Log::Logger::trace("HOSTSIM", "exit button clicked");
	this->close();
}

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
, m_pModel (nullptr)
{
	ui->setupUi(this);
	m_Server.setWindow(this);

	m_pModel = new QStandardItemModel(0,0,this); //0 Rows and 0 Columns
	m_pModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Date time")));
	m_pModel->setHorizontalHeaderItem(1, new QStandardItem(QString("in/ont")));
	m_pModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Message")));
	ui->tableView->setModel(m_pModel);
	ui->tableView->setColumnWidth(0, 150);
	ui->tableView->setColumnWidth(1, 60);
	ui->tableView->setColumnWidth(2, 300);
}

MainWindow::~MainWindow(){
	delete ui;
	delete m_pModel;
}

