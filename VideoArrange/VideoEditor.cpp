#include "VideoEditor.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include "VideoArrange.h"
#include "Table/TableView.h"
#include "Document/Document.h"

VideoEditor::VideoEditor(QWidget *parent)
	: QMainWindow(parent)
{
	QGraphicsView* view = new QGraphicsView;
	setCentralWidget(view);

	VideoArrange *videoArrange = new VideoArrange;
	TableView* tableView = new TableView;

	QGraphicsScene* scene = new QGraphicsScene;
	view->setScene(scene);

	QGraphicsProxyWidget* proxyWVideo = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
	proxyWVideo->setWindowTitle(QString::fromLocal8Bit("��Ŀ����"));
	proxyWVideo->setWidget(videoArrange);

	QGraphicsProxyWidget* proxyWTable = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
	proxyWTable->setWindowTitle(QString::fromLocal8Bit("ý����Դ"));
	proxyWTable->setWidget(tableView);

	scene->addItem(proxyWVideo);
	scene->addItem(proxyWTable);

	QMenu * menu = new QMenu(QString::fromLocal8Bit("�ļ�"));
	menuBar()->addMenu(menu);
	QAction* open = menu->addAction(QString::fromLocal8Bit("��"));
	connect(open, &QAction::triggered, this, &VideoEditor::onOpen);
	QAction* save = menu->addAction(QString::fromLocal8Bit("����"));
	connect(save, &QAction::triggered, this, &VideoEditor::onSave);
	QAction* neW = menu->addAction(QString::fromLocal8Bit("�½�"));
	connect(neW, &QAction::triggered, this, &VideoEditor::onNew);
}

VideoEditor::~VideoEditor()
{

}

void VideoEditor::onOpen()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("�򿪹���"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "xml(*.xml)");
	Document::instance()->open(fileName);
}

void VideoEditor::onSave()
{
	Document::instance()->save();
}

void VideoEditor::onNew()
{
	QString fileName = QFileDialog::getSaveFileName(nullptr, QString::fromLocal8Bit("�½�����"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "xml(*.xml)");
}
