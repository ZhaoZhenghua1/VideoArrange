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
	painter->fillRect(rect(), Qt::gray);
	TimeZone::paint(painter, option, widget);
	qreal w = rect().width();

	for (qreal x = 0; x <= w; x += m_dPixSpace)
	{
		QPoint pos(x, rect().height()  - 7);
		//视图之外不绘制
		if (!m_view->rect().contains(m_view->mapFromScene(pos)))
		{
			continue;
		}
		QString drawStr;
		QTime time = QTime(0, 0).addMSecs(x / m_dPixSpace*m_uiTimeSpace + 0.5);
		if (time.hour() > 0)
		{
			drawStr += QString("%1:").arg(time.hour());
		}
		if (!drawStr.isEmpty() || time.minute() > 0)
		{
			drawStr += QString("%1:").arg(time.minute());	
		}
		if (!drawStr.isEmpty() || time.second() > 0)
		{
			drawStr += QString("%1").arg(time.second());
		}
		if (m_uiTimeSpace < 1000)
		{
			QString deci = QString(".%1").arg(QTime(time.hour(), time.minute(), time.second()).msecsTo(time));
			while (deci.endsWith('0'))
			{
				deci = deci.left(deci.size() - 1);
			}
			drawStr += deci;
		}

		int fontW = painter->fontMetrics().width(drawStr);
		painter->drawText(pos -= QPoint(fontW/2,0), drawStr);

		painter->drawLine(QPointF(x, rect().height() - 5), QPointF(x, rect().height()));
	}
}

void TimeBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mousePressEvent(event);
	qreal posX = event->scenePos().x();
	unsigned int t = posX / m_dPixSpace*m_uiTimeSpace;
	emit m_view->sigTimebarClicked(t);
	event->setAccepted(true);
}

void TimeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mouseMoveEvent(event);
	emit m_view->sigTimebarClicked(event->scenePos().x() / m_dPixSpace*m_uiTimeSpace);
}
