#include "TimeVideoView.h"
#include "TimeVideoLine.h"
#include "../Document/Document.h"
#include "uilogic/LayerFactory.h"

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
	m_anchorLayout->setSpacing(0);

	setLayout(m_anchorLayout);
	LayerFactory::instance()->setRightLayout(m_anchorLayout);
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
// 	for (TimeVideoLine* item : m_layerLines)
// 	{
// 		delete item;
// 	}
// 	m_layerLines.clear();
// 
// 
// 	QDomElement d = Observer::data({ "project","track","layerlist"});
// 	QDomElement elem = d.firstChildElement("layer");
// 	if (elem.isNull())
// 	{
// 		return;
// 	}
// 	TimeVideoLine* first = new TimeVideoLine();
// 	m_layerLines.push_back(first);
// 	m_anchorLayout->addAnchor(m_layerLines[0], Qt::AnchorTop, m_anchorLayout, Qt::AnchorTop);
// 	m_anchorLayout->addAnchor(m_layerLines[0], Qt::AnchorLeft, m_anchorLayout, Qt::AnchorLeft);
// 	m_anchorLayout->addAnchor(m_layerLines[0], Qt::AnchorRight, m_anchorLayout, Qt::AnchorRight);
// 	QGraphicsAnchor* anchor = m_anchorLayout->addAnchor(m_layerLines[0], Qt::AnchorBottom, m_anchorLayout, Qt::AnchorTop);
// 	anchor->setSpacing(46);
// 	first->initData(elem);
// 
// 	elem = elem.nextSiblingElement("layer");
// 	for (int i = 1; !elem.isNull() ; elem = elem.nextSiblingElement("layer"), ++i)
// 	{
// 		TimeVideoLine* pLine = new TimeVideoLine();
// 		m_layerLines.push_back(pLine);
// 		pLine->initData(elem);
// 		m_anchorLayout->addAnchor(m_layerLines[i], Qt::AnchorTop, m_layerLines[i - 1], Qt::AnchorBottom);
// 		m_anchorLayout->addAnchor(m_layerLines[i], Qt::AnchorLeft, m_anchorLayout, Qt::AnchorLeft);
// 		m_anchorLayout->addAnchor(m_layerLines[i], Qt::AnchorRight, m_anchorLayout, Qt::AnchorRight);
// 		QGraphicsAnchor* anchor = m_anchorLayout->addAnchor(m_layerLines[i], Qt::AnchorBottom, m_layerLines[i - 1], Qt::AnchorBottom);
// 		anchor->setSpacing(-46);
// 	}
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

void TimeVideoView::wheelEvent(QWheelEvent *event)
{

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
