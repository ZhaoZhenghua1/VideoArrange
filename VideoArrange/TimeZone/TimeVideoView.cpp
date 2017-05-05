#include "TimeVideoView.h"
#include <QTime>
#include <QPainter>

TimeVideo::TimeVideo(TimeVideoView* view):m_view(view)
{

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
		if (m_dTimeSpace < 1000)
		{
			painter->drawText(pos, QTime(0, 0).addMSecs(x / m_dPixSpace*m_dTimeSpace).toString("hh:mm:ss.zzz"));
		}
		else
		{
			painter->drawText(pos, QTime(0, 0).addMSecs(x / m_dPixSpace*m_dTimeSpace).toString("hh:mm:ss"));
		}
	}
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

