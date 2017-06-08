#include "TimeVideoItem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../SizeGrib/SizeGripItem.h"
#include "TimeView.h"

namespace
{
	class RectResizer : public SizeGripItem::Resizer
	{
	public:
		virtual void operator()(QGraphicsItem* item, const QRectF& rect)
		{
			TimeVideoItem* rectItem =
				dynamic_cast<TimeVideoItem*>(item);
			if (rect.width() < 1)
			{
				return;
			}
			if (rectItem)
			{
				rectItem->setRect(rect);
				rectItem->updateTime();
			}
		}
	};
}

TimeVideoItem::TimeVideoItem(TimeZone* timezone):m_timezone(timezone)
{
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
	setBrush(Qt::darkCyan);
}


TimeVideoItem::~TimeVideoItem()
{
}

void TimeVideoItem::setRect(const QRectF& rect)
{
	QGraphicsRectItem::setRect(rect);
	if (!m_sizeGrip)
	{
		m_sizeGrip = new SizeGripItem(new RectResizer, this);
	}
	m_sizeGrip->fixToParent();
}

//根据时间更新位置
void TimeVideoItem::updatePos()
{
	QRectF resizeRect = QRectF(m_timezone->timeToPosition(m_startTime) - pos().x(), 0, m_timezone->timeToPosition(m_timeLen), 35);
	setRect(resizeRect);
}

//根据位置更新时间
void TimeVideoItem::updateTime()
{
	m_startTime = m_timezone->positionToTime(pos().x() + rect().left()) + 0.5;
	m_timeLen = m_timezone->positionToTime(rect().width()) + 0.5;
	m_dataElem.setAttribute("timeStart", QString("%1").arg(m_startTime));
	m_dataElem.setAttribute("timeLength", QString("%1").arg(m_timeLen));
}

bool TimeVideoItem::initData(const QDomElement& media)
{
	QString qsResId = media.attribute("resourceId");
	bool okTimeStart, okTimeLength;
	unsigned int timeStart = media.attribute("timeStart").toUInt(&okTimeStart);
	unsigned int timeLen = media.attribute("timeLength").toUInt(&okTimeLength);
	if (!okTimeStart || !okTimeLength && qsResId.isEmpty())
	{
		Q_ASSERT(false);
		return false;
	}
	m_dataElem = media;
	m_startTime = timeStart;
	m_timeLen = timeLen;
	m_qsRes = qsResId;
	updatePos();

	show();
	return true;
}

QDomElement TimeVideoItem::data()
{
	return m_dataElem;
}

void TimeVideoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), Qt::gray);
	painter->setPen(Qt::white);
	painter->drawText(rect(), m_qsRes);
}

QVariant TimeVideoItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene())
	{
		//轨道改变
		QList<QGraphicsItem *> items = parentItem()->parentItem()->childItems();

		QPoint cursorPos(QCursor::pos());
		QPoint viewPos = scene()->views().first()->mapFromGlobal(QCursor::pos());
		QPointF scenePos = scene()->views().first()->mapToScene(viewPos);
		for (auto ite = items.begin() ; ite != items.end(); ++ite)
		{
			QGraphicsItem* localItem = *ite;
			if (parentItem() == localItem)
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
