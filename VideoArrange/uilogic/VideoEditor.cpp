#include "VideoEditor.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDockWidget>

#include "TimeLineField.h"
#include "Table/ResourcePanel.h"
#include "Document/Document.h"
#include "Play/PlayWidget.h"

#include <QLayout>
#include <QPushButton>
#include <QSplitter>

VideoEditor::VideoEditor(QWidget *parent)
	: QMainWindow(parent)
{
//	delete takeCentralWidget();
	//takeCentralWidget();

	//setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
	setDocumentMode(true);

	TimeLineField *videoArrange = new TimeLineField;
	ResourcePanel* tableView = new ResourcePanel;
	PlayWidget* playWidget = new PlayWidget;
	
	QSplitter* hSplitter = new QSplitter(Qt::Horizontal);
	hSplitter->setHandleWidth(3);
	hSplitter->setChildrenCollapsible(false);
	hSplitter->setStyleSheet("QSplitter{background-color: rgb(0, 0, 0);}");
	hSplitter->addWidget(playWidget);
	hSplitter->addWidget(tableView);
	QSplitterHandle* handle = hSplitter->handle(1);
	handle->setStyleSheet("background-color: rgb(22, 22, 22);");
	QSplitter* vSplitter = new QSplitter(Qt::Vertical);
	vSplitter->setHandleWidth(3);
	vSplitter->setChildrenCollapsible(false);
	vSplitter->setStyleSheet("QSplitter{background-color: rgb(0, 0, 0);}");
	vSplitter->addWidget(hSplitter);
	vSplitter->addWidget(videoArrange);
	handle = vSplitter->handle(1);
	handle->setStyleSheet("background-color: rgb(22, 22, 22);");
	setCentralWidget(vSplitter);

//	QDockWidget* proxyWVideo = new QDockWidget();
//	proxyWVideo->layout()->setSpacing(10);
//	proxyWVideo->setWindowTitle(QString::fromLocal8Bit("节目编排"));
//	proxyWVideo->setFeatures(QDockWidget::NoDockWidgetFeatures);
//	proxyWVideo->setWidget(videoArrange);

//	QDockWidget* proxyWTable = new QDockWidget();
//	proxyWTable->setWindowTitle(QString::fromLocal8Bit("媒体资源"));
//	proxyWTable->setFeatures(QDockWidget::NoDockWidgetFeatures);
//	proxyWTable->setWidget(tableView);

//	QDockWidget* proxyWPlay = new QDockWidget();
//	proxyWPlay->setWindowTitle(QString::fromLocal8Bit("屏幕"));
//	proxyWPlay->setFeatures(QDockWidget::NoDockWidgetFeatures);
//	proxyWPlay->setWidget(playWidget);


//	addDockWidget(Qt::LeftDockWidgetArea, proxyWPlay);
// 	addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget);
// 	addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget);
// 	addDockWidget(Qt::LeftDockWidgetArea, new QDockWidget);
//	addDockWidget(Qt::RightDockWidgetArea, proxyWTable);
//	addDockWidget(Qt::BottomDockWidgetArea, proxyWVideo);

// 	QGraphicsView* view = new QGraphicsView;
//  	QGraphicsScene* scene = new QGraphicsScene;
// 	view->setScene(scene);
// 	scene->setSceneRect(QRect(0, 0, 2000, 2000));
// 	view->show();
// 
// 	QGraphicsProxyWidget* proxyWVideo = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
// 	proxyWVideo->setWindowTitle(QString::fromLocal8Bit("节目编排"));
// 	proxyWVideo->setWidget(videoArrange);
// 
// 	QGraphicsProxyWidget* proxyWTable = new QGraphicsProxyWidget(nullptr, Qt::Dialog);
// 	proxyWTable->setWindowTitle(QString::fromLocal8Bit("媒体资源"));
// 	proxyWTable->setWidget(tableView);
// 
// 	QGraphicsProxyWidget* proxyWPlay = new QGraphicsProxyWidget(nullptr, playWidget->windowFlags() | Qt::Window);
// 	proxyWPlay->setWindowTitle(QString::fromLocal8Bit("屏幕"));
// 	proxyWPlay->setWidget(playWidget);
// 
// 
// 	scene->addItem(proxyWVideo);
// 	scene->addItem(proxyWTable);
// 	scene->addItem(proxyWPlay);

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
