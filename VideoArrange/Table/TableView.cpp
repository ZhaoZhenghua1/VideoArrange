#include "TableView.h"
#include "MediaResModel.h"
#include <QHeaderView>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

TableView::TableView()
{
	QString styles = "background-color: rgb(50, 50, 50); color:rgb(0,255,255);";
	header()->setStyleSheet(styles);
	MediaResModel* model = new MediaResModel(this);
	setModel(model);
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

// void TableView::mouseMoveEvent(QMouseEvent *event)
// {
// 	QTreeView::mouseMoveEvent(event);
// 	if (QLineF(event->screenPos(), m_posPressed)
// 		.length() < QApplication::startDragDistance()) {
// 		return;
// 	}
// 
// 	QDrag *drag = new QDrag(this);
// 	QMimeData *mime = new QMimeData;
// 	drag->setMimeData(mime);
// 	
// 	drag->setPixmap(model()->data());
// 	//	drag->setHotSpot(QPoint(15, 20));
// 
// 	drag->exec();
// }
// 
// void TableView::mousePressEvent(QMouseEvent *event)
// {
// 	QTreeView::mousePressEvent(event);
// 	m_posPressed = event->screenPos();
// }
