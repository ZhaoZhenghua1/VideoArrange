#include "TimeVideoItem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../SizeGrib/SizeGripItem.h"
#include "TimeView.h"
#include "TimeVideoLine.h"
#include "Controls/EffectEditor.h"
#include <QGraphicsAnchorLayout>
#include "Document/Document.h"

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
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemSendsScenePositionChanges | ItemIsFocusable);
	setBrush(Qt::darkCyan);
}


TimeVideoItem::~TimeVideoItem()
{
// 	for (EffectEditor* item : m_effectEdits)
// 	{
// 		delete item;
// 	}
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
	TimeZone* tz = timeZone();
	if (!tz)
		return;
	
	int startTime = this->startTime();
	int timeLen = this->timeLen();
	QRectF resizeRect = QRectF(tz->timeToPosition(startTime) - pos().x(), 0, tz->timeToPosition(timeLen), parentItem()->boundingRect().height() - 2);
	setRect(resizeRect);

	updateEffectEditPos();
}

//根据位置更新时间
void TimeVideoItem::updateTime()
{
	TimeZone* tz = timeZone();
	if (!tz)
		return;

	int startTime = tz->positionToTime(pos().x() + rect().left()) + 0.5;
	int timeLen = tz->positionToTime(rect().width()) + 0.5;
	m_dataElem.setAttribute("timeStart", QString("%1").arg(startTime));
	m_dataElem.setAttribute("timeLength", QString("%1").arg(timeLen));

	//更新编辑器数据
	if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
	{
		tvl->setOriginator(this);
	}
}

bool TimeVideoItem::initData(const QDomElement& media)
{
	m_dataElem = media;

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
	m_effectEdits.push_back(new TransparencyEditor(timeZone()));
	m_effectEdits.push_back(new PositionEdiotr(timeZone()));
	m_effectEdits.push_back(new RotateEdiotr(timeZone()));
	m_effectEdits.push_back(new ScailEdiotr(timeZone()));
	m_effectEdits.push_back(new VoiceEditor(timeZone()));

	QDomElement parentElem = m_dataElem.firstChildElement("adjustmentlist");
	if (parentElem.isNull())
	{
		parentElem = Document::instance()->document().createElement("adjustmentlist");
		m_dataElem.appendChild(parentElem);
	}

	for (EffectEditor* editor : m_effectEdits)
	{
		editor->initDataFromParent(parentElem);
	}
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
	bool selected = false;
	if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
	{
		selected = tvl->originator() == this;
	}
	painter->fillRect(rect(), selected ? QColor(57,77,102) : QColor(114,154,204));
	painter->setPen(Qt::white);
	painter->drawText(rect(), resID());
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


void TimeVideoItem::keyPressEvent(QKeyEvent *event)
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

void TimeVideoItem::setQsData(const QString& data)
{
	QStringList datas = data.split(';');
	if (datas.size() != 3)
	{
		return;
	}
	m_dataElem.setAttribute("timeStart", datas.at(0));
	m_dataElem.setAttribute("timeLength", datas.at(1));
	m_dataElem.setAttribute("offset", datas[2]);
	updatePos();
}

void TimeVideoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);

	//选中当前时，设置编辑数据
	if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
	{
		tvl->setOriginator(this);
	}
}

QString TimeVideoItem::toQsData()
{
	return m_dataElem.attribute("timeStart") + ';' + m_dataElem.attribute("timeLength") + ';' + m_dataElem.attribute("offset");
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

int TimeVideoItem::startTime()
{
	return m_dataElem.attribute("timeStart").toInt();
}

int TimeVideoItem::timeLen()
{
	return m_dataElem.attribute("timeLength").toInt();
}

QString TimeVideoItem::resID() const
{
	return m_dataElem.attribute("resourceId");
}

IEditor* TimeVideoItem::editor()
{
	if (TimeVideoLine* tvl = dynamic_cast<TimeVideoLine*>(parentItem()))
	{
		return tvl->editor();
	}
	return nullptr;
}
