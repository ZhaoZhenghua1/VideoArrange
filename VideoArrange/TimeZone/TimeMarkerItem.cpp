
#include <QPainter>

#include "TimeMarkerItem.h"
#include "TimeView.h"
#include "TimeVideoLine.h"

TimeMarkerItem::TimeMarkerItem()
{
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsFocusable);
}

TimeMarkerItem::~TimeMarkerItem()
{
}

void TimeMarkerItem::updatePos()
{
	TimeZone* tz = timeZone();
	if (!tz)
		return;

	int startTime = this->startTime();
	QRectF resizeRect = QRectF(tz->timeToPosition(startTime) - pos().x(), 0, 24, parentItem()->boundingRect().height() - 2);
	setRect(resizeRect);
}

void TimeMarkerItem::updateTime()
{
	TimeZone* tz = timeZone();
	if (!tz)
		return;

	int startTime = tz->positionToTime(pos().x() + rect().left()) + 0.5;
	m_dataElem.setAttribute("timeStart", QString("%1").arg(startTime));

	//更新编辑器数据
	if (TimeMarkerLine* tvl = dynamic_cast<TimeMarkerLine*>(parentItem()))
	{
		tvl->setOriginator(this);
	}
}

void TimeMarkerItem::initData(const QDomElement& media)
{
	m_dataElem = media;

	updatePos();
}

void TimeMarkerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) 
{
	painter->fillRect(rect(), Qt::black);
}

QVariant TimeMarkerItem::itemChange(GraphicsItemChange change, const QVariant &value) 
{
	if (change == ItemPositionChange && scene())
	{
		//轨道改变
		QList<QGraphicsItem *> items = parentItem()->parentItem()->childItems();

		QPoint cursorPos(QCursor::pos());
		QPoint viewPos = scene()->views().first()->mapFromGlobal(QCursor::pos());
		QPointF scenePos = scene()->views().first()->mapToScene(viewPos);
		for (auto ite = items.begin(); ite != items.end(); ++ite)
		{
			QGraphicsItem* localItem = *ite;
			if (parentItem() == localItem || !dynamic_cast<TimeMarkerLine*>(localItem))
			{
				continue;
			}

			QPointF localItemPos = localItem->mapFromScene(scenePos);
			QRectF rect(QPointF(), localItem->boundingRect().size());

			if (rect.contains(localItemPos))
			{
				setParentItem(localItem);
				break;
			}
		}

		QPointF newPos = value.toPointF();
		newPos.setY(0);
		return newPos;
	}
	else if (change == ItemPositionHasChanged && scene())
	{
		updateTime();
	}
	return QGraphicsItem::itemChange(change, value);
}

void TimeMarkerItem::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier)
	{
		//删除时，清空编辑数据
		if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
		{
			tvl->setOriginator(nullptr);
		}
		delete this;
	}
}

void TimeMarkerItem::setQsData(const QString& data)
{

}

void TimeMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);

	//选中当前时，设置编辑数据
	if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
	{
		tvl->setOriginator(this);
	}
}

QString TimeMarkerItem::toQsData()
{
	return QString();
}

TimeZone* TimeMarkerItem::timeZone()
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

int TimeMarkerItem::startTime()
{
	return m_dataElem.attribute("timeStart").toInt();
}
