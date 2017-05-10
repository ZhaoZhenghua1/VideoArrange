#include "TimeBarView.h"
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <math.h>
#include <QPainter>
#include <QTime>
#include <QGraphicsSceneMouseEvent>
#include <qdebug>
#include <QTime>

TimeBarView::TimeBarView()
{
	m_bar = new TimeBar(this);
	scene()->addItem(m_bar);
}


TimeBarView::~TimeBarView()
{
}

TimeZone* TimeBarView::timeZone()
{
	return m_bar;
}

void TimeBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	QTime t;
	t.start();
	
	painter->fillRect(rect(), Qt::gray);
	TimeZone::paint(painter, option, widget);
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
			painter->drawText(pos,QTime(0,0).addMSecs(x/ m_dPixSpace*m_uiTimeSpace + 0.5).toString("hh:mm:ss.zzz"));
		}
		else
		{
			painter->drawText(pos, QTime(0, 0).addMSecs(x / m_dPixSpace*m_uiTimeSpace + 0.5).toString("hh:mm:ss"));
		}
	}
}

void TimeBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mousePressEvent(event);
	emit m_view->sigTimebarClicked(event->scenePos().x()/m_dPixSpace*m_uiTimeSpace);
	event->setAccepted(true);
}

void TimeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mouseMoveEvent(event);
	emit m_view->sigTimebarClicked(event->scenePos().x() / m_dPixSpace*m_uiTimeSpace);
}
