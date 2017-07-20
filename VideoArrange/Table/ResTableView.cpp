#include "ResTableView.h"
#include "MediaResModel.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QScrollBar>
#include "../Document/Document.h"

ResTableView::ResTableView()
{
	header()->setStyleSheet("QHeaderView::section{background-color: rgb(29, 29, 29); color:rgb(189, 189, 189);padding-left: 4px;border: 1px solid #6c6c6c}");

	setStyleSheet("QTreeView{background-color: rgb(29, 29, 29);}");
	setAcceptDrops(true);
	setDragEnabled(true);
	setModel(new MediaResModel());

	horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 11px; }");
	verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 11px; }");

	Document::instance()->addObserver(this);
}

ResTableView::~ResTableView()
{
}

void ResTableView::addFiles(const QStringList& files)
{
	((MediaResModel*)model())->addFiles(files);

	doItemsLayout();

	emit addTaskFiles(files);
}

void ResTableView::startDrag(Qt::DropActions supportedActions)
{
	QModelIndexList indexes = selectedIndexes();
	if (indexes.count() > 5)
	{
		QMimeData *data = model()->mimeData(indexes);
		if (!data)
			return;

		QPixmap pixmap = qvariant_cast<QPixmap>(model()->data(indexes.at(1), Qt::DecorationRole));
		QDrag *drag = new QDrag(this);
		drag->setPixmap(pixmap);
		drag->setMimeData(data);
		drag->setHotSpot(QPoint(5,5));
		drag->exec();
	}
	
}

void ResTableView::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	QStringList formats = mimeData->formats();
	QList<QUrl> urlList = mimeData->urls();
	bool valid = false;
	//判断拖动的文件中是否有有效的媒体文件
	for (int i = 0; i < urlList.size(); ++i)
	{
		QString file = urlList.at(i).toLocalFile();
		if (!file.isEmpty())
		{
			if (Document::instance()->isValidMediaFile(file))
			{
				valid = true;
				break;
			}
		}
	}
	if (valid)
	{
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
	}
}

void ResTableView::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void ResTableView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void ResTableView::dropEvent(QDropEvent *event)
{
	QStringList fileLst;
	const QMimeData *mimeData = event->mimeData();
	QStringList formats = mimeData->formats();
	QList<QUrl> urlList = mimeData->urls();
	for (int i = 0; i < urlList.size(); ++i)
	{
		QString file = urlList.at(i).toLocalFile();
		if (!file.isEmpty())
		{
			fileLst.push_back(file);
		}
	}

	this->addFiles(fileLst);
}

void ResTableView::init()
{
	((MediaResModel*)model())->setData(Document::instance()->data({ "project" ,"resourcelist" }));
	doItemsLayout();
}

