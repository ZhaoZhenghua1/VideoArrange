#include "TimeVideoView.h"
#include <QTime>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsWidget>

const char* const DOAG_DROP_TYPE = "LM-video-resource";
TimeVideo::TimeVideo(TimeVideoView* view):m_view(view)
{
	m_layoutWidget = new QGraphicsWidget(this);
	setRect(0, 0, 100, 100);
}

void TimeVideo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) 
{
	QGraphicsRectItem::paint(painter, option, widget);
	qreal w = rect().width();

	for (qreal x = 0; x <= w; x += m_dPixSpace)
	{
		QPoint pos(x, 20);
		//视图之外不绘制
		if (!m_view->rect().contains(m_view->mapFromScene(pos)))
		{
			continue;
		}
		if (m_uiTimeSpace < 1000)
		{
			painter->drawText(pos, QTime(0, 0).addMSecs(x / m_dPixSpace*m_uiTimeSpace).toString("hh:mm:ss.zzz"));
		}
		else
		{
			painter->drawText(pos, QTime(0, 0).addMSecs(x / m_dPixSpace*m_uiTimeSpace).toString("hh:mm:ss"));
		}
	}
}

QVariant TimeVideo::itemChange(GraphicsItemChange change, const QVariant& value) 
{
	return TimeZone::itemChange(change, value);
}

TimeVideoView::TimeVideoView()
{
	m_timeVideo = new TimeVideo(this);
	scene()->addItem(m_timeVideo);
}


TimeVideoView::~TimeVideoView()
{
}

TimeZone* TimeVideoView::timeZone()
{
	return m_timeVideo;
}

void TimeVideoView::dragEnterEvent(QDragEnterEvent *event) 
{
	if (event->mimeData()->hasFormat(DOAG_DROP_TYPE))
		event->accept();
	else
		event->ignore();
}

void TimeVideoView::dragLeaveEvent(QDragLeaveEvent *event)
{
	event->accept();
}

void TimeVideoView::dragMoveEvent(QDragMoveEvent *event) 
{
}

void TimeVideoView::dropEvent(QDropEvent *event) 
{
}