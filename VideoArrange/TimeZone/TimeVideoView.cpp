#include "TimeVideoView.h"
#include "TimeVideoLine.h"
#include "../Document/Document.h"
#include "uilogic/LayerBuilder.h"
#include "TimeVideoItem.h"

#include <QTime>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsWidget>
#include <QGraphicsAnchorLayout>
#include <QCoreApplication>
#include <QScrollBar>

const char* const DOAG_DROP_TYPE = "LM-video-resource";
TimeVideo::TimeVideo(TimeVideoView* view):m_view(view)
{
	m_anchorLayout = new QGraphicsAnchorLayout;
	m_anchorLayout->setContentsMargins(0, 0, 0, 0);
	m_anchorLayout->setSpacing(0);

	setLayout(m_anchorLayout);
	LayerBuilder::instance()->setRightLayout(m_anchorLayout);
//	Document::instance()->addObserver(this);
}

void TimeVideo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) 
{
	TimeZone::paint(painter, option, widget);
	qreal w = rect().width();

	painter->fillRect(rect(), QColor(49, 49, 49));
}

void TimeVideo::setGeometry(const QRectF &rect)
{
	TimeZone::setGeometry(rect);
}

void TimeVideo::init()
{

}

TimeVideoView::TimeVideoView()
{
	m_timeVideo = new TimeVideo(this);
	scene()->addItem(m_timeVideo);
}


TimeVideoView::~TimeVideoView()
{
}

void TimeVideoView::paintEvent(QPaintEvent *event)
{
	TimeView::paintEvent(event);
}

//Ë®Æ½¹ö¶¯
void TimeVideoView::wheelEvent(QWheelEvent *event)
{
	QCoreApplication::sendEvent(m_horScrollBar, event);
}

TimeZone* TimeVideoView::timeZone()
{
	return m_timeVideo;
}

qreal TimeVideoView::sceneHeight() 
{
	qreal height = 0;
	QGraphicsLayout* layout = m_timeVideo->layout();
	if (layout->count() > 0)
		height = layout->itemAt(layout->count() - 1)->geometry().bottom();
	height = (height > viewport()->height() ? height : viewport()->height()) + 200;
	return height;
}

void TimeVideoView::setTimeLength(unsigned int time)
{
	TimeView::setTimeLength(time);
	QList<QGraphicsItem*> items = scene()->items();
	for (QGraphicsItem* item : items)
	{
		if (TimeVideoItem* videoItem = dynamic_cast<TimeVideoItem*>(item))
		{
			videoItem->updatePos();
		}
	}
}
