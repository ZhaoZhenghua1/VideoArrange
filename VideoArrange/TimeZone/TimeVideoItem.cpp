#include "TimeVideoItem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../SizeGrib/SizeGripItem.h"
#include "TimeView.h"
#include "TimeVideoLine.h"
#include "Controls/EffectEdit.h"
#include <QGraphicsAnchorLayout>
#include "Document/Document.h"

enum EFFECT_TYPE
{

};
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
				rectItem->updateEffectEditPos();
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
	for (EffectEdit* item : m_effectEdits)
	{
		delete item;
	}
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

	updateEffectEditPos();
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
	createEffectEdit();
	effectEditParentChanged();
	updateEffectEditPos();

	show();
	return true;
}

QDomElement TimeVideoItem::data()
{
	return m_dataElem;
}

void TimeVideoItem::createEffectEdit()
{
	m_effectEdits.push_back(new TransparencyEdit(timeZone()));
	if (m_dataElem.firstChildElement("adjustmentlist").isNull())
	{
		QDomElement elem = Document::instance()->document().createElement("adjustmentlist");
		m_dataElem.appendChild(elem);

		QDomElement elem2 = Document::instance()->document().createElement("adjustment");
		elem.appendChild(elem2);
	}
	m_effectEdits[0]->initData(m_dataElem.firstChildElement("adjustmentlist").firstChildElement("adjustment"));
}

void TimeVideoItem::effectEditParentChanged()
{
	QGraphicsWidget* par = dynamic_cast<QGraphicsWidget*>(timeZone());
	if (!par)
		return;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return;

	int index = -1;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == dynamic_cast<QGraphicsWidget*>(parentItem()))
			break;

	if (index < 0 || index == layout->count())
		return;

	for (int i = 0; i < m_effectEdits.size(); ++i)
	{
		QGraphicsLayoutItem * secondItem = layout->itemAt(index + i + 1);
		if (QGraphicsWidget* par = dynamic_cast<QGraphicsWidget*>(secondItem))
		{
			m_effectEdits[i]->parentChanged(par);
		}
	}
}

void TimeVideoItem::updateEffectEditPos()
{
	QRectF posRect(rect());
	for (int i = 0; i < m_effectEdits.size(); ++i)
	{
		m_effectEdits[i]->updatePos(posRect.left() + pos().x(), posRect.width());	
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
				effectEditParentChanged();
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
		updateEffectEditPos();
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
