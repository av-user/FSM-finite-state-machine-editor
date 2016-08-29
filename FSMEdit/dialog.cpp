#include "dialog.h"
#include "ui_dialog.h"
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include "scene.h"

#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QMenu>
#include <QFileDialog>
#include <stdlib.h>
#include <time.h>
#include "StateItem.h"
#include "scene.h"
#include "itemdialog.h"
#include "eventdialog.h"
#include "statedialog.h"
#include "eventitem.h"
#include "../LogLib/logger.h"
#include "../CodeGen/codegenerator.h"
#include "../UtilLib/reg.h"

const QString Dialog::EVENT_LIST_FILENAME = "events.lst";

void Dialog::new_item2(const QPointF& pos){
    StateDialog stateDlg;
    stateDlg.setModal(true);
    QStringList lst;
    stateDlg.setEvents(m_pScene->getEventsList(), lst);
    stateDlg.setStateNames(m_pScene->getItemList(), "");
    stateDlg.exec();
    if (stateDlg.result() == QDialog::Accepted){
        StateItem *pTi2 = new StateItem();
		pTi2->setPos(pos);
		int h = pTi2->getHeight();
        pTi2->setName(stateDlg.getName().toStdString().c_str());
        pTi2->setEvents(stateDlg.getEvents());
		m_pScene->addItem(pTi2);
        m_pScene->addEventNames (stateDlg.getNewEvents());
	}
}
void Dialog::test(const QPoint& pos){

}
void Dialog::contextMenuView(const QPoint& pos, const QPointF& posScene){
	// for most widgets
	QPoint globalPos = m_ui->graphicsView->mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
	QMenu viewMenu;
    viewMenu.addAction("New state");
	// ...

	QAction* selectedMenuItem = viewMenu.exec(globalPos);
	if (selectedMenuItem != NULL){
        if (selectedMenuItem->text().compare("New state") == 0){
			new_item2(posScene);
		}
	} else {
		// nothing was chosen
	}
}
void Dialog::contextMenuItem2(const QPoint& pos, StateItem *pItem){
	QPoint globalPos = m_ui->graphicsView->mapToGlobal(pos);
	// for QAbstractScrollArea and derived classes you would use:
	// QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);
	QList<QString> menuList = pItem->getMenuList(pos);
	QMenu itemMenu;
	foreach (const QString pStr, menuList){
		itemMenu.addAction(pStr);
	}
	QAction* selectedMenuItem = itemMenu.exec(globalPos);
	if (selectedMenuItem != NULL){
		if (selectedMenuItem->text().compare("Delete") == 0){
            m_pScene->removeStateItem(pItem);
		} else if (selectedMenuItem->text().compare("Edit") == 0){
            StateDialog stateDlg;
            stateDlg.setModal(true);
            stateDlg.setEvents(m_pScene->getEventsList(), pItem->getEventsList());
            stateDlg.setStateNames(m_pScene->getItemList(), pItem->getName());
            stateDlg.exec();
            if (stateDlg.result() == QDialog::Accepted){
                pItem->updateEvents (stateDlg.getEvents());
                if (pItem->getName() != stateDlg.getName()){
                    m_pScene->renameItem(pItem->getName(), stateDlg.getName());
                }
                m_pScene->addEventNames (stateDlg.getNewEvents());
            }
		} else if (selectedMenuItem->text().compare("Test") == 0){
			;
		}
	} else {
		// nothing was chosen
	}
}
void Dialog::contextMenuEventItem(const QPoint& pos, EventItem *pItem){
	QPoint globalPos = m_ui->graphicsView->mapToGlobal(pos);
	QMenu itemMenu;
    itemMenu.addAction("Edit transitions");
	itemMenu.addAction("Delete");
	QAction* selectedMenuItem = itemMenu.exec(globalPos);
	if (selectedMenuItem != NULL){
        StateItem* pTestItem = qgraphicsitem_cast<StateItem*>(pItem->parentItem());
		if (selectedMenuItem->text().compare("Delete") == 0){
			pTestItem->removeEventItem (pItem);
			m_pScene->invalidate();
        } else if (selectedMenuItem->text().compare("Edit transitions") == 0){
            EventDialog eventDlg;
            eventDlg.setModal(true);
            eventDlg.setStates(m_pScene->getItemList(), pItem->getTransitionList());
            eventDlg.setNames (pTestItem->getName(), pItem->getName());
            eventDlg.exec();
            if (eventDlg.result() == QDialog::Accepted){
                StateItem *pStateItem =  qgraphicsitem_cast<StateItem*>(pItem->parentItem());
                pStateItem->setTransitions(pItem, eventDlg.getTransitions());
            }
		}
	} else {
		// nothing was chosen
	}
}
void Dialog::showContextMenu(const QPoint& pos){ // this is a slot
	QPointF posScene = m_ui->graphicsView->mapToScene(pos);
	QList<QGraphicsItem*> itemList = m_pScene->items(posScene);
	if (itemList.empty()){
		this->contextMenuView(pos, posScene);
	} else {
		QGraphicsItem *pItem = itemList.first();
		int t = pItem->type();
		int t0 = QGraphicsRectItem::Type;
		int t1 = QGraphicsTextItem::Type;
		int t2 = QGraphicsItem::Type;
        if (pItem->type() == StateItem::Type){
            this->contextMenuItem2(pos, qgraphicsitem_cast<StateItem*>(pItem));
		} else if (pItem->type() == EventItem::Type){
			this->contextMenuEventItem(pos, qgraphicsitem_cast<EventItem*>(pItem));
		} else {
			this->contextMenuView(pos, posScene);
		}
	}
}
void Dialog::on_pushButton_import_clicked(){
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Open state machine"), ".", tr("FSM files (*.fsm)"));
    QStringList importedlist;
    foreach (QString filename, filenames) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QString str (file.readAll());
            file.close();
            QStringList list = StateDialog::xml2eventlist (str);
            importedlist.append(list);
        }
    }
    importedlist.removeDuplicates();
    m_pScene->addEventNames(importedlist);
}
void Dialog::on_scaleSlider_valueChanged(int value){
    double newScale = (double)value / 100.0;
    QMatrix oldMatrix = m_ui->graphicsView->matrix();
    m_ui->graphicsView->resetMatrix();
    m_ui->graphicsView->translate(oldMatrix.dx(), oldMatrix.dy());
    m_ui->graphicsView->scale(newScale, newScale);
}
void Dialog::on_pushButton_new_clicked(){
    setFilename (QFileDialog::getSaveFileName(this, tr("New state machine"), ".", tr("FSM files (*.fsm)")));
}
void Dialog::on_pushButton_open_clicked(){
    std::string str;
    if (!UtilLib::Reg::getStr(&str, "Common", "Initdir")){
        str = ".";
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open state machine"), str.c_str()
                                                    , tr("FSM files (*.fsm)"));
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QFileInfo fileinfo (file);
        UtilLib::Reg::set("Common", "Initdir", fileinfo.canonicalPath().toStdString().c_str());
        setFilename(fileName);
        QByteArray bytes = file.readAll();
        file.close();
        QString str (bytes);
        m_pScene->clear();
        m_pScene->deserialize (str);
    }
}
void Dialog::on_pushButton_save_clicked(){
    if (m_Filename == ""){
        on_pushButton_saveas_clicked();
    } else {
        QFile file(m_Filename);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)){
            QString str = m_pScene->serialize (m_ui->checkBox_save_all_events->isChecked());
            QByteArray barray;
            barray.append(str);
            file.write(barray);
            file.close();
        }
    }
}
void Dialog::on_pushButton_saveas_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open state machine"), ".", tr("FSM files (*.fsm)"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        setFilename(fileName);
        QString str = m_pScene->serialize (m_ui->checkBox_save_all_events->isChecked());
        QByteArray barray;
        barray.append(str);
        file.write(barray);
        file.close();
    }
}
void Dialog::on_pushButton_generate_one_clicked(){
    CodeGenerator codegen (CodeGenerator::Qt, "\n");
    codegen.setFilename(m_Filename);
    bool ok = codegen.generate();
}
void Dialog::on_pushButton_generate_all_clicked(){
    CodeGenerator codegen (CodeGenerator::Qt, "\n");
    QFileInfo fileinfo (m_Filename);
    QString path = fileinfo.absolutePath();
    codegen.setDirname(path);
    bool ok = codegen.generate();
}
void Dialog::on_pushButton_generate_lib_clicked(){
    CodeGenerator codegen (CodeGenerator::Qt, "\n");
    QFileInfo fileinfo (m_Filename);
    codegen.generatelib(fileinfo.absolutePath());
}
void Dialog::on_pushButton_exit_clicked(){
	close();
}
void  Dialog::setFilename (QString filename){
    if (filename != ""){
        QFileInfo info (filename);
        QString name = info.completeBaseName ();
        this->setName(name);
        m_Filename = filename;
    }
}
void Dialog::setName (QString name){
    if (m_pScene != nullptr){
        m_pScene->setName(name);
        this->setWindowTitle("FSM editor: " + name);
    }
}

Dialog::Dialog(QWidget *parent)
:	QDialog(parent)
,	m_ui(new Ui::Dialog)
,   m_grayPen(QColor(Qt::gray), 0.5)
{
	m_ui->setupUi(this);
	this->m_pScene = new Scene (this);
    m_pScene->setSceneRect(QRectF(0, 0, WIN_WIDTH, WIN_HEIGHT));
	m_ui->graphicsView->setRenderHints(QPainter::Antialiasing);
	m_ui->graphicsView->setScene(this->m_pScene);
    setName ("noname");

	m_ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui->graphicsView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));

    for (int x = GRID_STEP; x < WIN_WIDTH; x += GRID_STEP){
        m_pScene->addLine(x, 0, x, WIN_HEIGHT, m_grayPen);
    }
    for (int y = GRID_STEP; y < WIN_HEIGHT; y += GRID_STEP){
        m_pScene->addLine(0, y, WIN_WIDTH, y, m_grayPen);
    }
}

Dialog::~Dialog(){
	delete m_ui;
}
