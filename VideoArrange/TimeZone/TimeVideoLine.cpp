#include "TimeVideoLine.h"
#include "TimeVideoItem.h"
#include "TimeView.h"
#include "../Document/Document.h"

#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include "Layer/RightLayer.h"


const char* const DOAG_DROP_TYPE = "LM-video-resource";
TimeVideoLine::TimeVideoLine()
{
	setAcceptDrops(true);
}


TimeVideoLine::~TimeVideoLine()
{
}

void TimeVideoLine::setOriginator(IOriginator* o)
{
	m_selectedOriginator = o;
	if (editor())
	{
		editor()->setOriginator(o);
	}
	update();
}

QVector<RightLayer*> TimeVideoLine::initData(const QDomElement& elem, QGraphicsAnchorLayout* layout)
{
	//初始化图层下面的操作图层
	QVector<RightLayer*> arrR({ new RightLayer ,new RightLayer ,new RightLayer ,new RightLayer ,new RightLayer});
	QVector<LayerBase*> fellows(arrR.size());
	for (int i = 0 ; i < arrR.size(); ++i)
	{
		fellows[i] = arrR[i];
	}
	//操作图层放入布局中
	addGroupToLayout(fellows, layout);

	m_dataElem = elem;
	//创建图层子对象
	QDomElement mediaList = elem.firstChildElement("medialist");
	for (QDomElement media = mediaList.firstChildElement("media"); !media.isNull(); media = media.nextSiblingElement("media"))
	{
		TimeVideoItem* pItem = new TimeVideoItem();
		pItem->setParentItem(this);
		if (!pItem->initData(media))
		{
			delete pItem;
		}
	}

	return arrR;
}

void TimeVideoLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
//	painter->fillRect(rect(), QColor(32, 32, 32));
	LayerLeader::paint(painter, option, widget);
}

void TimeVideoLine::setGeometry(const QRectF &rect) 
{
	QGraphicsWidget::setGeometry(rect);
	QList<QGraphicsItem*> children = childItems();
	for (QGraphicsItem* item : children)
	{
		if (TimeVideoItem* pItem = dynamic_cast<TimeVideoItem* >(item))
		{
			pItem->updatePos();
		}
	}
}

QVariant TimeVideoLine::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (ItemChildAddedChange == change)
	{
		QGraphicsItem* item = qvariant_cast<QGraphicsItem*>(value);
		QDomElement mediaList = m_dataElem.firstChildElement("medialist");
		if (TimeVideoItem* pItem = dynamic_cast<TimeVideoItem*>(item))
		{
			mediaList.appendChild(pItem->data());
		}

	}
	return QGraphicsWidget::itemChange(change, value);
}

void TimeVideoLine::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasFormat(DOAG_DROP_TYPE))
		event->accept();
	else
		event->ignore();
}

void TimeVideoLine::dragLeaveEvent(QGraphicsSceneDragDropEvent *event) 
{
	event->accept();
}

void TimeVideoLine::dragMoveEvent(QGraphicsSceneDragDropEvent *event) 
{

}

void TimeVideoLine::dropEvent(QGraphicsSceneDragDropEvent *event) 
{
	event->accept();
	TimeZone* timeZ = timeZone();
	if (!timeZ)
		return;

	const QMimeData* mimeData = event->mimeData();
	QString qsResId = mimeData->data(DOAG_DROP_TYPE);
	TimeVideoItem* pItem = new TimeVideoItem();
	pItem->setParentItem(this);

	QDomElement media = Document::instance()->document().createElement("media");
	media.setAttribute("resourceId", qsResId);
	media.setAttribute("timeStart", QString("%1").arg(timeZ->positionToTime(event->pos().x())));
	//todo:通过接口获取时长
	media.setAttribute("timeLength", "60000");

	QDomElement mediaList = m_dataElem.firstChildElement("medialist");
	mediaList.appendChild(media);

	pItem->initData(media);
}

TimeZone* TimeVideoLine::timeZone()
{
	for (QGraphicsItem * par = parentItem(); par; par = par->parentItem())
	{
		if (TimeZone* tz = dynamic_cast<TimeZone*>(par))
		{
			return tz;
		}
	}
	return nullptr;
}
