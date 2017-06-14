#include "EffectEdit.h"
#include <QGraphicsSceneHoverEvent>

#include "TimeZone/TimeView.h"
#include <Document/Document.h>
#include <QtMath>
#include <QPainter>
#include <QPolygon>
#include <QGraphicsAnchorLayout>

EffectEdit::EffectEdit(TimeZone* timeZone):m_timeZone(timeZone)
{
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable);
}


EffectEdit::~EffectEdit()
{
}

void EffectEdit::initData(const QDomElement& elem)
{
	m_data = elem;
}

void EffectEdit::updatePos(qreal left, qreal width)
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
		return ;
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

void EffectEdit::parentChanged(QGraphicsWidget* parent)
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

void EffectEdit::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	for (int i = 0; i < lineCount() ; ++i)
	{
		Line line = lineAt(i);
		if (line.isOnLine(event->pos()))
		{
			if (line.selectIndex(event->pos()) < 0)
			{
				setCursor(Qt::CrossCursor);
			}
			else
			{
				setCursor(Qt::ClosedHandCursor);
			}
		}
		else
		{
			setCursor(Qt::ArrowCursor);
		}
	}
}

void EffectEdit::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
}

void EffectEdit::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!rect().contains(event->pos()))
	{
		return;
	}
	if (m_selectedLineIndex >= 0 && m_selectedLineIndex < lineCount())
	{
		Line line = lineAt(m_selectedLineIndex);

		line.adjustPointValue(m_selectedPointIndex, m_timeZone->positionToTime(event->pos().x()), positionToValue(event->pos().y()));
	}
	update();
}

void EffectEdit::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	scene()->setFocusItem(this, Qt::ShortcutFocusReason);
	m_selectedLineIndex = m_selectedPointIndex = -1;
	for (int i = 0; i < lineCount(); ++i)
	{
		Line line = lineAt(i);
		if (line.isOnLine(event->pos()))
		{
			//select a point
			m_selectedLineIndex = i;
			if ((m_selectedPointIndex = line.selectIndex(event->pos())) >= 0)
			{

			}
			else
			{
				//no point selected , so create one
				m_selectedPointIndex = line.addAdjustPoint(m_timeZone->positionToTime(event->pos().x()), positionToValue(event->pos().y()));
			}
			setCursor(Qt::ClosedHandCursor);
			update();
			return;
		}
	}
}

void EffectEdit::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void EffectEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier)
	{
		//delete selected point
		if (m_selectedLineIndex >= 0 && m_selectedLineIndex < lineCount())
		{
			Line line = lineAt(m_selectedLineIndex);

			line.deleteAdjustPoint(m_selectedPointIndex);
		}
		update();
	}
}


void EffectEdit::focusInEvent(QFocusEvent *event)
{
	update();
}

void EffectEdit::focusOutEvent(QFocusEvent *event)
{
	m_selectedPointIndex = m_selectedLineIndex = -1;
	update();
}

void EffectEdit::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	if (focusItem() == this)
	{
		painter->fillRect(rect(), QColor(35, 35, 35));
	}
	for (int i = 0; i < lineCount(); ++i)
	{
		Line line = lineAt(i);
		line.paint(painter, m_selectedLineIndex == i ? m_selectedPointIndex : -1);
	}
}

QVector<QPointF> EffectEdit::Line::points()const
{
	QVector<QPointF> ret;
	for (QDomElement elemTable = m_data.firstChildElement("adjustmentpos"); !elemTable.isNull(); elemTable = elemTable.nextSiblingElement("adjustmentpos"))
	{
		qreal time = elemTable.attribute("name").toDouble();
		qreal value = elemTable.attribute("value").toDouble();
		ret.push_back(QPointF(m_owner->m_timeZone->timeToPosition(time), m_owner->valueToPosition(value)));
	}
	return ret;
}

void EffectEdit::Line::addHeadAndTail(QVector<QPointF>& ret)
{
	QPointF begin(0, m_owner->rect().height() / 2);
	QPointF end(m_owner->rect().width(), m_owner->rect().height() / 2);
	if (!ret.empty())
	{
		begin.setY(ret.first().y());
		end.setY(ret.last().y());
	}
	ret.push_front(begin);
	ret.push_back(end);
}

int EffectEdit::Line::addAdjustPoint(qreal time, qreal value)
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
	elem.setAttribute("name", time);
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

void EffectEdit::Line::adjustPointValue(int index, qreal time, qreal value)
{
	if (index < 0)
		return;
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index ; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}

	curData.setAttribute("name", time);
	curData.setAttribute("value", value);
	return;
	QDomElement elemTable = m_data.firstChildElement("adjustmentpos");
	for (; !elemTable.isNull(); elemTable = elemTable.nextSiblingElement("adjustmentpos"))
	{
		if (elemTable.attribute("name").toDouble() > time)
		{
			break;
		}
	}

	m_data.insertBefore(curData, elemTable);
}

void EffectEdit::Line::deleteAdjustPoint(int index)
{
	QDomElement curData = m_data.firstChildElement("adjustmentpos");
	for (int i = 0; i < index; curData = curData.nextSiblingElement("adjustmentpos"))
	{
		++i;
	}
	m_data.removeChild(curData);
}

int EffectEdit::Line::selectIndex(const QPointF& pos)
{
	QVector<QPointF> points = this->points();
	for (int i = 0; i < points.size(); ++i)
	{
		if ((pos - points[i]).manhattanLength() <= 3)
		{
			return i;
		}
	}

	return -1;
}

bool EffectEdit::Line::isOnLine(const QPointF& pos)
{
	QVector<QPointF> ret = points();
	addHeadAndTail(ret);
	for (int i = 1; i < ret.size(); ++i)
	{
		const QPointF& begin = ret[i - 1];
		const QPointF& end = ret[i];
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

void EffectEdit::Line::paint(QPainter *painter, int selectIndex)
{
	QVector<QPointF> ret = points();
	addHeadAndTail(ret);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QColor(89, 89, 89));
	painter->drawPolyline(QPolygonF(ret));
	for (auto ite = ret.begin() + 1; ite != ret.end() - 1; ++ite)
	{
		if (!(selectIndex--))
		{
			painter->setBrush(QColor(225, 225, 128));
		}
		else
		{
			painter->setBrush(Qt::NoBrush);
		}
		painter->drawEllipse(*ite, 3, 3);
	}
}

unsigned int TransparencyEdit::lineCount() const
{
	return 1;
}

EffectEdit::Line TransparencyEdit::lineAt(unsigned int index) 
{
	if (index == 0)
	{
		return EffectEdit::Line(this, m_data);
	}
	return EffectEdit::Line(this, QDomElement());
}

qreal TransparencyEdit::valueToPosition(qreal val) const
{
	if (val < 0)
	{
		val = 0;
	}
	if (val > 1)
	{
		val = 1;
	}
	return 3 + val * (rect().height() - 6);
}

qreal TransparencyEdit::positionToValue(qreal yPos) const
{
	if (yPos < 3)
	{
		yPos = 3;
	}
	if (yPos > rect().height() - 3)
	{
		yPos = rect().height() - 3;
	}
	return yPos / (rect().height() - 6);
}
