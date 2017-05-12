#include "TimeVideoItem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../SizeGrib/SizeGripItem.h"

namespace
{
	class RectResizer : public SizeGripItem::Resizer
	{
	public:
		virtual void operator()(QGraphicsItem* item, const QRectF& rect)
		{
			QGraphicsRectItem* rectItem =
				dynamic_cast<QGraphicsRectItem*>(item);

			if (rectItem)
			{
				rectItem->setRect(rect);
			}
		}
	};
}

TimeVideoItem::TimeVideoItem(const QString& res):m_qsRes(res)
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
		//¹ìµÀ¸Ä±ä
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
	return QGraphicsItem::itemChange(change, value);
}
