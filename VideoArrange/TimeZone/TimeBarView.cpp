#include "TimeBarView.h"
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <math.h>
#include <QPainter>
#include <QTime>
#include <QGraphicsSceneMouseEvent>
#include <qdebug>
#include <QTime>
#include <math.h>

TimeBarView::TimeBarView()
{
	m_bar = new TimeBar(this);
	scene()->addItem(m_bar);
	setFixedHeight(41);
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
	painter->fillRect(rect(), QColor(35, 35, 35));
	TimeZone::paint(painter, option, widget);
	qreal w = rect().width();
	bool painted = false;
	for (qreal x = 0; x <= w; x += pixSpace())
	{
		QPoint pos(x, rect().height()  - 18);
		//视图之外不绘制
		if (!m_view->rect().contains(m_view->mapFromScene(pos)))
		{
			//视图之内绘制完成，不需要继续绘制
			if (painted)
			{
				return;
			}
			continue;
		}
		painted = true;
		QString drawStr;
		QTime time = QTime(0, 0).addMSecs((int)(positionToTime(x) + 0.5));
		drawStr = time.toString("hh:mm:ss.zzz");
		drawStr = drawStr.left(drawStr.size() - 1);
// 		if (time.hour() > 0)
// 		{
// 			drawStr += QString("%1:").arg(time.hour());
// 		}
// 		if (!drawStr.isEmpty() || time.minute() > 0)
// 		{
// 			drawStr += QString("%1:").arg(time.minute());	
// 		}
// 		if (!drawStr.isEmpty() || time.second() > 0)
// 		{
// 			drawStr += QString("%1").arg(time.second());
// 		}
// 		if (timeSpace() < 1000)
// 		{
// 			QString deci = QString(".%1").arg(QTime(time.hour(), time.minute(), time.second()).msecsTo(time));
// 			while (deci.endsWith('0'))
// 			{
// 				deci = deci.left(deci.size() - 1);
// 			}
// 			drawStr += deci;
// 		}

		int fontW = painter->fontMetrics().width(drawStr);
		painter->setPen(QPen(QColor(163, 163, 163)));
		painter->drawText(pos -= QPoint(fontW/2,0), drawStr);

		painter->setPen(QPen(QColor(79, 79, 79)));
		painter->drawLine(QPointF(x - 1, rect().height() - 13), QPointF(x - 1, rect().height()));

		for (int i = 1; i < 5; ++i)
		{
			painter->drawLine(QPointF(x + pixSpace()/5 * i - 1, rect().height() - 6), QPointF(x + pixSpace() / 5 * i - 1, rect().height()));
		}
	}
}

void TimeBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mousePressEvent(event);
	qreal posX = event->scenePos().x();
	qreal t = positionToTime(posX);//posX / m_dPixSpace*m_uiTimeSpace;
	emit m_view->sigTimebarClicked(timeToRound(t));
	event->setAccepted(true);
}

void TimeBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	TimeZone::mouseMoveEvent(event);
	qreal posX = event->scenePos().x();
	if (event->scenePos().x() < 0 )
	{
		posX = 0;
	}
	else if (event->scenePos().x() > scene()->width())
	{
		posX = scene()->width();
	}
	emit m_view->sigTimebarClicked(timeToRound(positionToTime(posX)));//event->scenePos().x() / m_dPixSpace*m_uiTimeSpace);
}

qreal TimeBar::timeToRound(qreal t)
{
	qreal precision = timeSpace() / pixSpace();
	if (t < precision && t > -precision)
	{
		return 0;
	}
	qreal tmin = t - precision;
	qreal tmax = t + precision;
	int e = 1;
	while (std::ceil(tmin / 10) < (tmax / 10) && tmin > 10)
	{
		tmin = std::ceil(tmin / 10);
		tmax /= 10;
		e *= 10;
	}
	return tmin * e;
}
