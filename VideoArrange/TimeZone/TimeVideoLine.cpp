#include "TimeVideoLine.h"
#include "TimeVideoItem.h"
#include "TimeMarkerItem.h"
#include "TimeView.h"
#include "../Document/Document.h"

#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QMenu>
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
	media.setAttribute("timeStart", QString("%1").arg((int)(timeZ->positionToTime(event->pos().x()))));
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

TimeMarkerLine::TimeMarkerLine()
{
	m_helper = new TimeMarkerLineHelper(this);
}

void TimeMarkerLine::initData(const QDomElement& elem, QGraphicsAnchorLayout* layout)
{

	QVector<LayerBase*> fellows;

	//操作图层放入布局中
	addGroupToLayout(fellows, layout);

	m_dataElem = elem;
	//创建图层子对象
	QDomElement mediaList = elem.firstChildElement("marklist");
	for (QDomElement media = mediaList.firstChildElement("marker"); !media.isNull(); media = media.nextSiblingElement("marker"))
	{
		TimeMarkerItem* pItem = new TimeMarkerItem();
		pItem->setParentItem(this);
		pItem->initData(media);
	}
}

void TimeMarkerLine::setOriginator(IOriginator* o)
{
	m_selectedOriginator = o;
	if (editor())
	{
		editor()->setOriginator(o);
	}
	update();
}

void TimeMarkerLine::onAction(QObject* sender)
{
	QAction* pAction = qobject_cast<QAction*>(sender);
	if (!pAction)
	{
		return;
	}

	QString text = pAction->text();
	text.remove("Insert ");
	text.remove(" Marker");
	if (text.isEmpty())
	{
		return;
	}
	TimeMarkerItem* pItem = new TimeMarkerItem();
	pItem->setParentItem(this);

	QDomElement media = Document::instance()->document().createElement("marker");
	media.setAttribute("timeStart", QString("%1").arg((int)(timeZone()->positionToTime(m_createPos.x()))));
	media.setAttribute("type", text);

	QDomElement mediaList = m_dataElem.firstChildElement("marklist");
	mediaList.appendChild(media);

	pItem->initData(media);
}

void TimeMarkerLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		QMenu menu;
		QAction* pAction1 = menu.addAction("Insert Position Marker");
		connect(pAction1, &QAction::triggered, m_helper, &TimeMarkerLineHelper::onAction);
		QAction* pAction2 = menu.addAction("Insert Pause Marker");
		connect(pAction2, &QAction::triggered, m_helper, &TimeMarkerLineHelper::onAction);
// 		QAction* pAction3 = menu.addAction("Insert Stop Marker");
// 		connect(pAction3, &QAction::triggered, m_helper, &TimeMarkerLineHelper::onAction);
		QAction* pAction4 = menu.addAction("Insert Jump Marker");
		connect(pAction4, &QAction::triggered, m_helper, &TimeMarkerLineHelper::onAction);
		QAction* pAction5 = menu.addAction("Insert Trigger Marker");
		connect(pAction5, &QAction::triggered, m_helper, &TimeMarkerLineHelper::onAction);
		m_createPos = event->pos();
		menu.exec(QCursor::pos());
	}
	LayerLeader::mousePressEvent(event);
}

TimeZone* TimeMarkerLine::timeZone()
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

void TimeMarkerLine::setGeometry(const QRectF &rect)
{
	QGraphicsWidget::setGeometry(rect);
	QList<QGraphicsItem*> children = childItems();
	for (QGraphicsItem* item : children)
	{
		if (TimeMarkerItem* pItem = dynamic_cast<TimeMarkerItem*>(item))
		{
			pItem->updatePos();
		}
	}
}

QVariant TimeMarkerLine::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (ItemChildAddedChange == change)
	{
		QGraphicsItem* item = qvariant_cast<QGraphicsItem*>(value);
		QDomElement mediaList = m_dataElem.firstChildElement("marklist");
		if (TimeMarkerItem* pItem = dynamic_cast<TimeMarkerItem*>(item))
		{
			mediaList.appendChild(pItem->data());
		}

	}
	return QGraphicsWidget::itemChange(change, value);
}