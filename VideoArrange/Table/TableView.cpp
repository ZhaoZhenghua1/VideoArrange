#include "TableView.h"
#include "MediaResModel.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "../Document/Document.h"

TableView::TableView()
{
	QString styles = "background-color: rgb(50, 50, 50); color:rgb(0,255,255);";
	header()->setStyleSheet(styles);

	setModel(Document::instance()->createMediaResModel());

	setStyleSheet("background-color: rgb(28, 58, 255);color: rgb(255, 255, 255);QHeaderView{background-color: rgb(50, 50, 50);}");
	setAcceptDrops(true);
	setDragEnabled(true);
}

TableView::~TableView()
{
}

void TableView::startDrag(Qt::DropActions supportedActions)
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

void TableView::dragEnterEvent(QDragEnterEvent *event)
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

void TableView::dragMoveEvent(QDragMoveEvent *event)
{
	event->acceptProposedAction();
}

void TableView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void TableView::dropEvent(QDropEvent *event)
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
	Document::instance()->addMediaResFiles(fileLst);
	doItemsLayout();
}

