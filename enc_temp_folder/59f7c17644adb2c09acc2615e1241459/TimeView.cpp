#include "TimeView.h"
#include <QResizeEvent>


TimeView::TimeView()
{
	QGraphicsScene* scene = new QGraphicsScene;
	setScene(scene);

	setTimeZone(new TimeZone(this));
}


TimeView::~TimeView()
{
}

void TimeView::setTimeZone(TimeZone* timeZone)
{
	delete m_timeZone;
	m_timeZone = timeZone;
	timeZone->m_timeView = this;
	scene()->addItem(timeZone);
	m_timeZone->setPos(0,0);
}

//�Ŵ�
void TimeView::zoomIn()
{
	//��С���200ms
	if ((int)m_timeZone->m_dTimeSpace <= 200)
	{
		return;
	}
}
//��С
void TimeView::zoomOut()
{

}

void TimeView::setSceneRect(const QRectF& rect)
{
	scene()->setSceneRect(rect);
	m_timeZone->setRect(rect);
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	if ((int)(viewport()->width()/m_timeZone->m_dPixSpace*m_timeZone->m_dTimeSpace) == m_timeZone->m_iTimeLength)
	{
		setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
	}
}

TimeZone::TimeZone(TimeView* timeView) : m_timeView(timeView)
{

}
