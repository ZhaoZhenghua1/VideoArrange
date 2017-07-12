
#include <QPainter>

#include "TimeMarkerItem.h"
#include "TimeView.h"
#include "TimeVideoLine.h"
#include "Controls/MagnetManager.h"

TimeMarkerItem::TimeMarkerItem()
{
	m_markerMask = QPixmap(":/markers.png");
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
	painter->fillRect(rect(), QColor(146,146,146));
	painter->setPen(QColor(255,232,72));
	painter->drawLine(rect().left(),0, rect().left(),rect().height());
	QSizeF pixSize(24, 22);
	QString type = m_dataElem.attribute("type");
	static QStringList pixmapTypes = QStringList({ "Position", "Pause", "Stop","Reset", "Jump","PPT", "Trigger" });
	int top = 2;
	for (const QString& pixmapType : pixmapTypes)
	{
		if (pixmapType == type)
		{
			break;
		}
		top += pixSize.height();
	}
	QRectF pixRect(QPointF(0, top), pixSize);
	painter->drawPixmap(QPointF(rect().left(), (rect().height() - pixSize.height()) / 2), m_markerMask, pixRect);
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
				if (TimeMarkerLine* tvl = dynamic_cast<TimeMarkerLine*>(parentItem()))
				{
					tvl->setOriginator(nullptr);
				}
				setParentItem(localItem);
				break;
			}
		}

		QPointF newPos = value.toPointF() + rect().topLeft();
		QPointF scenepos = parentItem()->mapToScene(newPos);
		MagnetManager::instance()->attached(scenepos, this);
		newPos = parentItem()->mapFromScene(scenepos) - rect().topLeft();
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
		if (TimeMarkerLine* tvl = dynamic_cast<TimeMarkerLine*>(parentItem()))
		{
			tvl->setOriginator(nullptr);
		}
		delete this;
	}
}

void TimeMarkerItem::setQsData(const QString& data)
{
	QStringList datas = data.split(';');
	if (datas.size() != 3)
	{
		return;
	}
	m_dataElem.setAttribute("timeStart", datas[0]);
	m_dataElem.setAttribute("type", datas[1]);
	m_dataElem.setAttribute("param", datas[2]);
	updatePos();
	update();
}

void TimeMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);

	//选中当前时，设置编辑数据
	if (TimeMarkerLine* tvl = dynamic_cast<TimeMarkerLine*>(parentItem()))
	{
		tvl->setOriginator(this);
	}
}

QString TimeMarkerItem::toQsData()
{
	return m_dataElem.attribute("timeStart") + ';' + m_dataElem.attribute("type") + ';' + m_dataElem.attribute("param");
}

bool TimeMarkerItem::attached(QPointF& scenePos)
{
	qreal left = parentItem()->mapToScene(pos() + rect().topLeft()).x();
	if (qAbs(scenePos.x() - left) <= 10)
	{
		scenePos.setX(left);
		return true;
	}
	return false;
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
