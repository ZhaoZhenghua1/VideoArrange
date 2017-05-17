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
#include "Play/PlayWidget.h"

VideoEditor::VideoEditor(QWidget *parent)
	: QMainWindow(parent)
{
	QGraphicsView* view = new QGraphicsView;
	setCentralWidget(view);

	VideoArrange *videoArrange = new VideoArrange;
	TableView* tableView = new TableView;
	PlayWidget* playWidget = new PlayWidget;

	QGraphicsScene* scene = new QGraphicsScene;
	view->setScene(scene);

	QGraphicsProxyWidget* proxyWVideo = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
	proxyWVideo->setWindowTitle(QString::fromLocal8Bit("节目编排"));
	proxyWVideo->setWidget(videoArrange);

	QGraphicsProxyWidget* proxyWTable = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
	proxyWTable->setWindowTitle(QString::fromLocal8Bit("媒体资源"));
	proxyWTable->setWidget(tableView);

	QGraphicsProxyWidget* proxyWPlay = new QGraphicsProxyWidget(nullptr, playWidget->windowFlags() | Qt::Window);
	proxyWPlay->setWindowTitle(QString::fromLocal8Bit("屏幕"));
	proxyWPlay->setWidget(playWidget);


	scene->addItem(proxyWVideo);
	scene->addItem(proxyWTable);
	scene->addItem(proxyWPlay);

	{
		QMenu * menu = new QMenu(QString::fromLocal8Bit("文件"));
		menuBar()->addMenu(menu);
		QAction* open = menu->addAction(QString::fromLocal8Bit("打开"));
		connect(open, &QAction::triggered, this, &VideoEditor::onOpen);
		QAction* save = menu->addAction(QString::fromLocal8Bit("保存"));
		connect(save, &QAction::triggered, this, &VideoEditor::onSave);
		QAction* neW = menu->addAction(QString::fromLocal8Bit("新建"));
		connect(neW, &QAction::triggered, this, &VideoEditor::onNew);
	}


	{
		QMenu * menu = new QMenu(QString::fromLocal8Bit("节目编排"));
		menuBar()->addMenu(menu);
		QAction* addlayer = menu->addAction(QString::fromLocal8Bit("添加图层"));
		connect(addlayer, &QAction::triggered, this, &VideoEditor::onAddLayer);
		QAction* delayer = menu->addAction(QString::fromLocal8Bit("删除图层"));
		connect(delayer, &QAction::triggered, this, &VideoEditor::onDelLayer);
		QAction* timelenset = menu->addAction(QString::fromLocal8Bit("设置时间长度"));
		connect(timelenset, &QAction::triggered, this, &VideoEditor::onSetProjTimeLen);
	}

}

VideoEditor::~VideoEditor()
{

}

void VideoEditor::onOpen()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("打开工程"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "xml(*.xml)");
	Document::instance()->open(fileName);
}

void VideoEditor::onSave()
{
	Document::instance()->save();
}

void VideoEditor::onNew()
{
	QString fileName = QFileDialog::getSaveFileName(nullptr, QString::fromLocal8Bit("新建工程"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "xml(*.xml)");
}

void VideoEditor::onAddLayer()
{

}

void VideoEditor::onDelLayer()
{

}

void VideoEditor::onSetProjTimeLen()
{

}
