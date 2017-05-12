#include "TimeVideoView.h"
#include "TimeVideoLine.h"

#include <QTime>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsWidget>
#include <QGraphicsAnchorLayout>

const char* const DOAG_DROP_TYPE = "LM-video-resource";
TimeVideo::TimeVideo(TimeVideoView* view):m_view(view)
{
	m_anchorLayout = new QGraphicsAnchorLayout;
	m_anchorLayout->setContentsMargins(0, 0, 0, 0);
	m_anchorLayout->setSpacing(1);
	TimeVideoLine* tls[7];
	for (int i = 0;i < 7;++i)
	{
		tls[i] = new TimeVideoLine;
	}
	m_anchorLayout->addAnchor(tls[0], Qt::AnchorTop,m_anchorLayout , Qt::AnchorTop);
 	m_anchorLayout->addAnchor(tls[0], Qt::AnchorLeft,m_anchorLayout , Qt::AnchorLeft);
 	m_anchorLayout->addAnchor(tls[0], Qt::AnchorRight, m_anchorLayout, Qt::AnchorRight);
	QGraphicsAnchor* anchor = m_anchorLayout->addAnchor(tls[0], Qt::AnchorBottom, m_anchorLayout, Qt::AnchorTop);
	anchor->setSpacing(46);

 	for (int i = 1 ; i < 2 ; ++i)
 	{
 		m_anchorLayout->addAnchor(tls[i], Qt::AnchorTop, tls[i-1], Qt::AnchorBottom);
 		m_anchorLayout->addAnchor(tls[i], Qt::AnchorLeft, m_anchorLayout, Qt::AnchorLeft);
 		m_anchorLayout->addAnchor(tls[i], Qt::AnchorRight, m_anchorLayout, Qt::AnchorRight);
 		QGraphicsAnchor* anchor = m_anchorLayout->addAnchor(tls[i], Qt::AnchorBottom, tls[i-1], Qt::AnchorBottom);
 		anchor->setSpacing(-46);
 	}

	setLayout(m_anchorLayout);
}

void TimeVideo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) 
{
	TimeZone::paint(painter, option, widget);
	qreal w = rect().width();

	painter->fillRect(rect(), Qt::yellow);
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

void TimeVideo::setGeometry(const QRectF &rect)
{
	TimeZone::setGeometry(rect);
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

qreal TimeVideoView::sceneHeight() 
{
	return viewport()->height() + 200;
}
