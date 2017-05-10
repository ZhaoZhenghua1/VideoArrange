#include "TimeVideoItem.h"
#include <QPainter>



TimeVideoItem::TimeVideoItem(const QString& res):m_qsRes(res)
{
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges);
}


TimeVideoItem::~TimeVideoItem()
{
}

void TimeVideoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), Qt::gray);
	painter->drawText(0, 0, m_qsRes);
}

QVariant TimeVideoItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange && scene()) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		newPos.setY(0);
		return newPos;
	}
	return QGraphicsItem::itemChange(change, value);
}
