#include "TimePointerView.h"
#include <QPainter>


TimePointerView::TimePointerView()
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_TransparentForMouseEvents);
	viewport()->setAttribute(Qt::WA_TranslucentBackground);
	viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);

	m_timePointer = new TimePointer;
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

void TimePointerView::onClickTimeBar(const qreal xpos)
{
	m_timePointer->click(xpos);
}

TimeZone* TimePointerView::timeZone()
{
	return m_timePointer;
}

void TimePointer::click(qreal xpos)
{
	m_qrClickPos = xpos;
	update();
}

void TimePointer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
//	setBrush(QColor(0, 255, 0, 50));
	TimeZone::paint(painter, option, widget);
	painter->drawLine(QPointF(m_qrClickPos,0),QPointF(m_qrClickPos,rect().height()));
}
