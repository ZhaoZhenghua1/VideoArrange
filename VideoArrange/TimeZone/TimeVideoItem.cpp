#include "TimeVideoItem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../SizeGrib/SizeGripItem.h"
#include "TimeView.h"
#include "TimeVideoLine.h"

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

TimeVideoItem::TimeVideoItem()
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

//����ʱ�����λ��
void TimeVideoItem::updatePos()
{
	TimeZone* tz = timeZone();
	if (!tz)
		return;

	QRectF resizeRect = QRectF(tz->timeToPosition(m_startTime) - pos().x(), 0, tz->timeToPosition(m_timeLen), 24);
	setRect(resizeRect);
}

//����λ�ø���ʱ��
void TimeVideoItem::updateTime()
{
	TimeZone* tz = timeZone();
	if (!tz)
		return;

	m_startTime = tz->positionToTime(pos().x() + rect().left()) + 0.5;
	m_timeLen = tz->positionToTime(rect().width()) + 0.5;
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
		//����ı�
		QList<QGraphicsItem *> items = parentItem()->parentItem()->childItems();

		QPoint cursorPos(QCursor::pos());
		QPoint viewPos = scene()->views().first()->mapFromGlobal(QCursor::pos());
		QPointF scenePos = scene()->views().first()->mapToScene(viewPos);
		for (auto ite = items.begin() ; ite != items.end(); ++ite)
		{
			QGraphicsItem* localItem = *ite;
			if (parentItem() == localItem || !dynamic_cast<TimeVideoLine*>(localItem))
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

TimeZone* TimeVideoItem::timeZone()
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