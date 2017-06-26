#include "EffectEditor.h"
#include <QGraphicsSceneHoverEvent>

#include "TimeZone/TimeView.h"
#include <Document/Document.h>
#include <QtMath>
#include <QPainter>
#include <QPolygon>
#include <QGraphicsAnchorLayout>
#include "Layer/RightLayer.h"

EffectEditor::EffectEditor(TimeZone* timeZone) :m_timeZone(timeZone)
{
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable);
}


EffectEditor::~EffectEditor()
{
}

void EffectEditor::initDataFromParent(QDomElement& parentElem)
{
	//从父节点中找到和当前节点类型一样的节点，从而获取数据
	QDomElement curElem = parentElem.firstChildElement("adjustment");
	for (; !curElem.isNull(); curElem = curElem.nextSiblingElement("adjustment"))
	{
		if (curElem.attribute("type") == type())
		{
			break;
		}
	}
	//如果没有，则创建
	if (curElem.isNull())
	{
		curElem = Document::instance()->document().createElement("adjustment");
		parentElem.appendChild(curElem);
		curElem.setAttribute("type", type());
	}

	m_data = curElem;
}

void EffectEditor::updatePos(qreal left, qreal width)
{
	QGraphicsWidget* par = parentWidget();
	if (!par)
	{
		Q_ASSERT(false);
		return;
	}

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
	{
		Q_ASSERT(false);
		return;
	}

	//调整左边位置
	QGraphicsAnchor* anchor = layout->anchor(this, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	if (anchor)
		anchor->setSpacing(left);
	else
		Q_ASSERT(false);

	//调整右边位置
	anchor = layout->anchor(this, Qt::AnchorRight, layout, Qt::AnchorLeft);
	if (anchor)
		anchor->setSpacing((left + width));
	else
		Q_ASSERT(false);
}

void EffectEditor::parentChanged(QGraphicsWidget* parent)
{
	setParentItem(nullptr);
	if (QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(parent->layout()))
	{
		layout->addAnchor(this, Qt::AnchorTop, layout, Qt::AnchorTop);
		layout->addAnchor(this, Qt::AnchorBottom, layout, Qt::AnchorBottom);
		layout->addAnchor(this, Qt::AnchorLeft, layout, Qt::AnchorLeft);
		layout->addAnchor(this, Qt::AnchorRight, layout, Qt::AnchorLeft);
	}
}

void EffectEditor::setValue(int index, const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		return;
	}
	if (index < 0)
		return;
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}

	curData.setAttribute("value", datas[0]);
	curData.setAttribute("name", datas[1]);
	update();
}

QString EffectEditor::value(int index)const
{
	if (index < 0)
		return "";
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}

	return curData.attribute("value") + ';' + curData.attribute("name");
}

QString EffectEditor::positionToValue(qreal xPos) const
{
	QDomElement before, after;
	QDomElement elemTable = m_data.firstChildElement("adjustmentpos");
	qreal curtime = m_timeZone->positionToTime(xPos);
	for (; !elemTable.isNull(); elemTable = elemTable.nextSiblingElement("adjustmentpos"))
	{
		qreal time = elemTable.attribute("name").toDouble();
		if (time > curtime)
		{
			after = elemTable;
			break;
		}
		else
		{
			before = elemTable;
		}
		if (elemTable.nextSiblingElement("adjustmentpos").isNull())
		{
			break;
		}
	}

	return average(before, after, curtime);
}

// IEditor* EffectEditor::editor()
// {
// 	//设置左边的编辑器
// 	if (RightLayer* rightLayer = dynamic_cast<RightLayer*>(parentItem()))
// 	{
// 		return rightLayer->editor();
// 	}
// 	return nullptr;
// }

RightLayer* EffectEditor::layer()
{
	return dynamic_cast<RightLayer*>(parentItem());
}

void EffectEditor::setQsData(const QString& data)
{
	int index = layer()->selectedIndex(this);
	setValue(index, data);
}

QString EffectEditor::toQsData()
{
	int index = layer()->selectedIndex(this);
	return value(index);
}

void EffectEditor::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	Line line = getLine();
	if (line.isOnLine(event->pos()))
	{
		if (line.selectIndex(event->pos()) < 0)
		{
			setCursor(Qt::CrossCursor);
		}
		else
		{
			setCursor(Qt::PointingHandCursor);
		}
	}
	else
	{
		setCursor(Qt::ArrowCursor);
	}
}

void EffectEditor::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
}

void EffectEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	qreal x = event->pos().x();
	if (x < 0)
	{
		x = 0;
	}
	if (x > rect().width())
	{
		x = rect().width();
	}

	Line line = getLine();
	if (m_pressIndex >= 0)
	{
		line.setPointTime(m_pressIndex, m_timeZone->positionToTime(x));

		layer()->setSelected(this, m_pressIndex);
		update();
	}
}

void EffectEditor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	scene()->setFocusItem(this, Qt::ShortcutFocusReason);
	int selectedPointIndex = -1; 
	m_pressIndex = -1;
	Line line = getLine();
	if (line.isOnLine(event->pos()))
	{
		//select a point
		if ((selectedPointIndex = line.selectIndex(event->pos())) < 0)
		{
			qreal x = event->pos().x();

			//no point selected , so create one
			selectedPointIndex = line.addAdjustPoint(m_timeZone->positionToTime(x), positionToValue(x));
		}
		m_pressIndex = selectedPointIndex;
		layer()->setSelected(this, selectedPointIndex);
		setCursor(Qt::PointingHandCursor);
		update();
	}
	
}

void EffectEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_pressIndex < 0)
	{
		return;
	}
	qreal x = event->pos().x();
	if (x < 0)
	{
		x = 0;
	}
	if (x > rect().width())
	{
		x = rect().width();
	}

	Line line = getLine();
	QString value = line.pointValue(m_pressIndex);
	line.deleteAdjustPoint(m_pressIndex);
	m_pressIndex = -1;

	int index = line.addAdjustPoint(m_timeZone->positionToTime(x), value);
	layer()->setSelected(this, index);

}

void EffectEditor::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier)
	{
		//delete selected point

		Line line = getLine();

		line.deleteAdjustPoint(layer() ? layer()->selectedIndex(this) : -1);
		layer()->setSelected(this, layer()->selectedIndex(this));
		update();
	}
}

void EffectEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
// 	if (focusItem() == this)
// 	{
// 		painter->fillRect(rect(), QColor(216, 210, 109));
// 	}

	Line line = getLine();
	line.paint(painter, layer() ? layer()->selectedIndex(this) : -1);
}

EffectEditor::Line EffectEditor::getLine()
{
	return Line(this, m_data);
}

QVector<qreal> EffectEditor::Line::points()const
{
	QVector<qreal> ret;
	for (QDomElement elemTable = m_data.firstChildElement("adjustmentpos"); !elemTable.isNull(); elemTable = elemTable.nextSiblingElement("adjustmentpos"))
	{
		qreal time = elemTable.attribute("name").toDouble();
		qreal x = m_owner->m_timeZone->timeToPosition(time);
		ret.push_back(x);
	}
	return ret;
}

void EffectEditor::Line::addHeadAndTail(QVector<qreal>& ret)
{
	ret.push_front(0);
	ret.push_back(m_owner->rect().width());
}

int EffectEditor::Line::addAdjustPoint(qreal time, const QString& value)
{
	int index = -1;
	QDomElement elemTable = m_data.firstChildElement("adjustmentpos");
	for (; !elemTable.isNull(); elemTable = elemTable.nextSiblingElement("adjustmentpos"))
	{
		++index;
		if (elemTable.attribute("name").toDouble() > time)
		{
			break;
		}
	}

	QDomElement elem = Document::instance()->document().createElement("adjustmentpos");
	elem.setAttribute("name", (unsigned int)(time + 0.5));
	elem.setAttribute("value", value);
	if (elemTable.isNull())
	{
		++index;
		m_data.appendChild(elem);
	}
	else
	{
		m_data.insertBefore(elem, elemTable);
	}
	return index;
}

// void EffectEditor::Line::adjustPointValue(int index, qreal time, const QString& value)
// {
// 	if (index < 0)
// 		return;
// 	QDomElement curData = m_data.firstChildElement("adjustmentpos");
// 	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
// 	{
// 		++i;
// 	}
// 
// 	curData.setAttribute("name", (unsigned int)(time));
// 	curData.setAttribute("value", value);
// 	return;
// }

void EffectEditor::Line::deleteAdjustPoint(int index)
{
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}
	m_data.removeChild(curData);
}

int EffectEditor::Line::selectIndex(const QPointF& pos)
{
	QVector<qreal> points = this->points();
	for (int i = 0; i < points.size(); ++i)
	{
		if (qAbs(pos.x() - points[i]) <= 3)
		{
			return i;
		}
	}

	return -1;
}

bool EffectEditor::Line::isOnLine(const QPointF& pos)
{
	QVector<qreal> ret = points();
	addHeadAndTail(ret);
	for (int i = 1; i < ret.size(); ++i)
	{
		const QPointF& begin = QPointF(ret[i - 1], m_owner->rect().height() / 2);
		const QPointF& end = QPointF(ret[i], m_owner->rect().height() / 2);;
		//line Ax+By+C=0
		qreal A = end.y() - begin.y();
		qreal B = begin.x() - end.x();
		qreal C = -B*begin.y() - A*begin.x();
		//d = |(Ax + By + C)/sqrt(A^2 + B^2)|
		qreal d = qAbs(A*pos.x() + B * pos.y() + C) / qSqrt(A*A + B*B);
		if (d <= 3)
		{
			return true;
		}
	}
	return false;
}

void EffectEditor::Line::paint(QPainter *painter, int selectIndex)
{
	QVector<qreal> ret = points();
	addHeadAndTail(ret);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QColor(89, 89, 89));
	painter->drawLine(0, m_owner->rect().height() / 2, m_owner->rect().width(), m_owner->rect().height() / 2);
	for (auto ite = ret.begin() + 1; ite != ret.end() - 1; ++ite)
	{
		QPointF pos(*ite, m_owner->rect().height() / 2);
		if (!m_owner->rect().contains(pos))
		{
			continue;
		}
		if (!(selectIndex--))
		{
			painter->setBrush(QColor(225, 225, 128));
		}
		else
		{
			painter->setBrush(Qt::NoBrush);
		}
		painter->drawEllipse(pos, 3, 3);
	}
}

void EffectEditor::Line::setPointTime(int index, unsigned int time)
{
	if (index < 0)
		return ;
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}

	return curData.setAttribute("name", time);
}

QString EffectEditor::Line::pointValue(int index)
{
	if (index < 0)
		return "";
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}

	return curData.attribute("value");
}

QString TransparencyEditor::average(QDomElement before, QDomElement after, qreal curtime) const
{
	if (before.isNull() && after.isNull())
	{
		return "100%";
	}
	else if (before.isNull())
	{
		return after.attribute("value");
	}
	else //if (after.isNull())
	{
		return before.attribute("value");
	}
// 	else
// 	{
// 		//求线性平均值
// 		unsigned int beforeTime = before.attribute("name").toUInt();
// 		unsigned int afterTime = after.attribute("name").toUInt();
// 		unsigned int beforeValue = before.attribute("value").remove("%").toUInt();
// 		unsigned int afterValue = after.attribute("value").remove("%").toUInt();
// 		return QString("%1%").arg(beforeValue + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue - beforeValue));
// 	}
}

QPoint stringToPoint(const QString& str)
{
	QStringList strlst = str.split(',');
	if (strlst.size() != 2)
	{
		return QPoint();
	}
	return QPoint(strlst.at(0).toInt(), strlst.at(1).toInt());
}

QPointF stringToPointF(const QString& str)
{
	QStringList strlst = str.split(',');
	if (strlst.size() != 2)
	{
		return QPoint();
	}
	return QPoint(strlst.at(0).toDouble(), strlst.at(1).toDouble());
}

QString PositionEdiotr::average(QDomElement before, QDomElement after, qreal curtime) const
{
	if (before.isNull() && after.isNull())
	{
		return "0,0";
	}
	else if (before.isNull())
	{
		return after.attribute("value");
	}
	else //if (after.isNull())
	{
		return before.attribute("value");
	}
// 	else
// 	{//求线性平均值
// 		unsigned int beforeTime = before.attribute("name").toUInt();
// 		unsigned int afterTime = after.attribute("name").toUInt();
// 		QPoint beforeValue = stringToPoint(before.attribute("value"));
// 		QPoint afterValue = stringToPoint(after.attribute("value"));
// 		return QString("%1,%2").arg(beforeValue.x() + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue.x() - beforeValue.x()))
// 			.arg(beforeValue.y() + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue.y() - beforeValue.y()));
// 	}
}

QString RotateEdiotr::average(QDomElement before, QDomElement after, qreal curtime) const
{
	if (before.isNull() && after.isNull())
	{
		return "0";
	}
	else if (before.isNull())
	{
		return after.attribute("value");
	}
	else //if (after.isNull())
	{
		return before.attribute("value");
	}
// 	else
// 	{//求线性平均值
// 		unsigned int beforeTime = before.attribute("name").toUInt();
// 		unsigned int afterTime = after.attribute("name").toUInt();
// 		qreal beforeValue = before.attribute("value").remove("%").toDouble();
// 		qreal afterValue = after.attribute("value").remove("%").toDouble();
// 		return QString("%1").arg(beforeValue + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue - beforeValue));
// 	}
}

QString ScailEdiotr::average(QDomElement before, QDomElement after, qreal curtime) const
{
	if (before.isNull() && after.isNull())
	{
		return "0%,0%";
	}
	else if (before.isNull())
	{
		return after.attribute("value");
	}
	else //if (after.isNull())
	{
		return before.attribute("value");
	}
// 	else
// 	{//求线性平均值
// 		qreal beforeTime = before.attribute("name").toDouble();
// 		qreal afterTime = after.attribute("name").toDouble();
// 		QPointF beforeValue = stringToPointF(before.attribute("value"));
// 		QPointF afterValue = stringToPointF(after.attribute("value"));
// 		return QString("%1%,%2%").arg(beforeValue.x() + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue.x() - beforeValue.x()))
// 			.arg(beforeValue.y() + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue.y() - beforeValue.y()));
// 	}
}

QString VoiceEditor::average(QDomElement before, QDomElement after, qreal curtime) const
{
	if (before.isNull() && after.isNull())
	{
		return "100%";
	}
	else if (before.isNull())
	{
		return after.attribute("value");
	}
	else if (after.isNull())
	{
		return before.attribute("value");
	}
	else
	{
		qreal beforeTime = before.attribute("name").toDouble();
		qreal afterTime = after.attribute("name").toDouble();
		qreal beforeValue = before.attribute("value").remove("%").toDouble();
		qreal afterValue = after.attribute("value").remove("%").toDouble();
		return QString("%1%").arg(beforeValue + (curtime - beforeTime) / (afterTime - beforeTime)*(afterValue - beforeValue));
	}
}
