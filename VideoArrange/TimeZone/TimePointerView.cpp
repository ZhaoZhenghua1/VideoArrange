#include "TimePointerView.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>


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

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | windowFlags());
	viewport()->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | viewport()->windowFlags());

	setAutoFillBackground(false);
	viewport()->setAutoFillBackground(false);
}


TimePointerView::~TimePointerView()
{
}

unsigned int TimePointerView::currentTime()
{
	return m_timePointer->currentTime();
}

void TimePointerView::onClickTimeBar(qreal timepos)
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

void TimePointer::click(qreal timepos)
{
	m_qrClickTimePos = timepos;
	update();
}

unsigned int TimePointer::currentTime()
{
	return m_qrClickTimePos + 0.5;
}

void TimePointer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
//	setBrush(QColor(0, 255, 0, 10));
//	painter->fillRect(rect(), QBrush(QColor(0, 255, 0, 100)));
	TimeZone::paint(painter, option, widget);
//视图之外不绘制
	qreal x = timeToPosition(m_qrClickTimePos);
	QPointF pos(x, 20);
	if (m_view->rect().contains(m_view->mapFromScene(pos)))
	{
		painter->drawLine(QPointF(x, 0), QPointF(x, rect().height()));
	}
	
}
