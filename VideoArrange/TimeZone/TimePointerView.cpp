#include "TimePointerView.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


TimePointerView::TimePointerView()
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	viewport()->setAttribute(Qt::WA_TranslucentBackground);
	viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);

	m_timePointer = new TimePointer(this);
	scene()->addItem(m_timePointer);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFrameShape(QFrame::NoFrame);

	setWindowFlags(Qt::FramelessWindowHint);
	viewport()->setWindowFlags(Qt::FramelessWindowHint);
}


TimePointerView::~TimePointerView()
{
}

void TimePointerView::onClickTimeBar(const unsigned int timepos)
{
	m_timePointer->click(timepos);
}

void TimePointerView::resizeEvent(QResizeEvent *event)
{
	TimeView::resizeEvent(event);

}

TimeZone* TimePointerView::timeZone()
{
	return m_timePointer;
}

TimePointer::TimePointer(TimePointerView* view):m_view(view)
{
	
}

void TimePointer::click(const unsigned int timepos)
{
	m_uiClickTimePos = timepos;
	update();
}

void TimePointer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
//	setBrush(QColor(0, 255, 0, 10));
	painter->fillRect(rect(), QBrush(QColor(0, 255, 0, 10)));
	TimeZone::paint(painter, option, widget);
//视图之外不绘制
	int x = m_uiClickTimePos / m_uiTimeSpace * m_dPixSpace;
	QPoint pos(x, 20);
	if (m_view->rect().contains(m_view->mapFromScene(pos)))
	{
		painter->drawLine(QPointF(x, 0), QPointF(x, rect().height()));
	}
	
}
