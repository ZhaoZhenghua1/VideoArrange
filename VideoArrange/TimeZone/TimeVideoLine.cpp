#include "TimeVideoLine.h"
#include "TimeVideoItem.h"
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

const char* const DOAG_DROP_TYPE = "LM-video-resource";
TimeVideoLine::TimeVideoLine()
{
	setAcceptDrops(true);
}


TimeVideoLine::~TimeVideoLine()
{
}

void TimeVideoLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), Qt::black);
}

void TimeVideoLine::setGeometry(const QRectF &rect) 
{
	QGraphicsWidget::setGeometry(rect);
}

void TimeVideoLine::dragEnterEvent(QGraphicsSceneDragDropEvent *event) 
{
	if (event->mimeData()->hasFormat(DOAG_DROP_TYPE))
		event->accept();
	else
		event->ignore();
}

void TimeVideoLine::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) 
{
	event->accept();
}

void TimeVideoLine::dragMoveEvent(QGraphicsSceneDragDropEvent *event) 
{

}

void TimeVideoLine::dropEvent(QGraphicsSceneDragDropEvent *event) 
{
	const QMimeData* mimeData = event->mimeData();
	QString qsResId = mimeData->data(DOAG_DROP_TYPE);
	TimeVideoItem* pItem = new TimeVideoItem(qsResId);
	pItem->setParentItem(this);
	pItem->setRect(QRectF(QPointF(event->pos().x(), 0), QSizeF(200, 35)));
	pItem->show();
	update();
	pItem->update();

}
