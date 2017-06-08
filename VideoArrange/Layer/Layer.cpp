#include "Layer.h"
#include "LayerTitle.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPainter>

const unsigned int MIN_SPACE = 14;
const unsigned int MAX_SPACE = 100;
Layer::Layer(QGraphicsWidget* widget)
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0, 0, 0, 10);
	setLayout(layout);
	layout->addItem(widget);
	setMinimumHeight(14);
	setAcceptHoverEvents(true);
}


Layer::~Layer()
{
}

void Layer::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int delt = event->delta();
	if (!m_anchorBottom)
		return;
	
	qreal space = m_anchorBottom->spacing();
	if (delt > 0)
	{
		space += ( 1);
	}
	else
	{
		space -= ( 1);
	}
	int notate = qAbs(space) / space;
	if (space / notate < MIN_SPACE)
	{
		space = MIN_SPACE;
	}
	if (space / notate > MAX_SPACE)
	{
		space = MAX_SPACE;
	}
	m_anchorBottom->setSpacing(qAbs(space)*notate);

	event->accept();
}

void Layer::hoverMoveEvent(QGraphicsSceneHoverEvent *event) 
{
	QRectF rec = rect();
	rec.adjust(0, (rec.height() - 4), 0, 4);
	QPointF hpos = event->pos();
	if (rec.contains(hpos))
	{
		setCursor(Qt::SizeVerCursor);
	}
}

void Layer::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
}

void Layer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(boundingRect(), Qt::black);
}

void Layer::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	int delt = event->pos().y() - event->lastPos().y();
	if (!m_anchorBottom)
		return;

	qreal space = m_anchorBottom->spacing();

	m_anchorBottom->setSpacing(space - delt);

}

void Layer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Layer::setAnchor(QGraphicsAnchor* top, QGraphicsAnchor* bottom)
{
	m_anchorTop = top;
	m_anchorBottom = bottom;
}

LayerGroup::LayerGroup(const QVector<Layer*>& layers, QGraphicsAnchorLayout* anchorLayout)
{
	QGraphicsLayoutItem* anchorTo = anchorLayout;
	if (anchorLayout->count() > 0)
		anchorTo = anchorLayout->itemAt(anchorLayout->count() - 1);

	LayerTitle* title = new LayerTitle;
	
	anchorLayout->addAnchor(title, Qt::AnchorTop, anchorTo, anchorLayout->count() > 0 ? Qt::AnchorBottom : Qt::AnchorTop);
	anchorLayout->addAnchor(title, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(title, Qt::AnchorRight, anchorTo, Qt::AnchorRight);

	anchorTo = title;

	for (auto ite = layers.begin(); ite != layers.end(); ++ite)
	{
		QGraphicsAnchor* anchorTop = anchorTop = anchorLayout->addAnchor(*ite, Qt::AnchorTop, anchorTo, Qt::AnchorBottom);
		anchorLayout->addAnchor(*ite, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft)->setSpacing(40);
		anchorLayout->addAnchor(*ite, Qt::AnchorRight, anchorLayout, Qt::AnchorRight);
		QGraphicsAnchor* anchorBottom = anchorLayout->addAnchor(*ite, Qt::AnchorBottom, anchorTo, Qt::AnchorBottom);
		anchorBottom->setSpacing(-21);
		(*ite)->setAnchor(anchorTop, anchorBottom);
		anchorTo = *ite;
	}
}



// void Layer::addChild(Layer* child)
// {
// 	QList<QGraphicsItem*> childs = childItems();
// 	if (childs.contains(child))
// 		return;
// 	
// 	//去除非Layer的内容
// 	childs.erase(std::remove_if(childs.begin(), childs.end(), [](QGraphicsItem* item) {return !!dynamic_cast<Layer*>(item);}), childs.end());
// 	//按y排序
// 	std::sort(childs.begin(), childs.end(), [](QGraphicsItem* l, QGraphicsItem* r) {return l->pos().y() < r->pos().y();});
// 
// 	QGraphicsAnchorLayout* anchorLayout = dynamic_cast<QGraphicsAnchorLayout*>(layout());
// 	if (!anchorLayout)
// 		return;
// 
// 	QGraphicsLayoutItem* anchorTo = anchorLayout;
// 	if (!childs.empty())
// 		anchorTo = dynamic_cast<Layer*>(childs.last());
// 		
// 	anchorLayout->addAnchor(child, Qt::AnchorTop, anchorTo, Qt::AnchorTop);
// 	anchorLayout->addAnchor(child, Qt::AnchorLeft, anchorTo, Qt::AnchorLeft);
// 	anchorLayout->addAnchor(child, Qt::AnchorRight, anchorTo, Qt::AnchorRight);
// 	QGraphicsAnchor* anchor = anchorLayout->addAnchor(child, Qt::AnchorBottom, anchorTo, Qt::AnchorBottom);
// 	anchor->setSpacing(-46);
// }

// void Layer::setWidget(QGraphicsWidget* widget)
// {
// 	delete m_thisWidget;
// 	m_thisWidget = widget;
// 	QGraphicsAnchorLayout* anchorLayout = dynamic_cast<QGraphicsAnchorLayout*>(layout());
// 	if (!anchorLayout)
// 		return;
// 
// 	anchorLayout->addAnchor(m_thisWidget, Qt::AnchorTop, anchorLayout, Qt::AnchorTop);
// 	anchorLayout->addAnchor(m_thisWidget, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
// 	anchorLayout->addAnchor(m_thisWidget, Qt::AnchorRight, anchorLayout, Qt::AnchorRight);
// 	QGraphicsAnchor* anchor = anchorLayout->addAnchor(m_thisWidget, Qt::AnchorBottom, anchorLayout, Qt::AnchorTop);
// 	anchor->setSpacing(46);
// }

const int LEFT_SPACING = 40;
LayerBase::LayerBase()
{
	setMinimumHeight(24);
}

QGraphicsAnchor* LayerLeader::anchor()
{
	QGraphicsWidget* par = parentWidget();
	if (!par)
		return  nullptr;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return nullptr;

	int index = 0;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == this)
			break;

	if (0 == index || index == layout->count())
		return nullptr;

	QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);

	return layout->anchor(this, Qt::AnchorTop, secondItem, Qt::AnchorBottom);
}

void LayerLeader::addGroupToLayout(const QVector<LayerFellow*>& fellows, QGraphicsAnchorLayout* anchorLayout)
{
	QGraphicsLayoutItem* anchorTo = anchorLayout;
	if (anchorLayout->count() > 0)
		anchorTo = anchorLayout->itemAt(anchorLayout->count() - 1);

	anchorLayout->addAnchor(this, Qt::AnchorTop, anchorTo, anchorLayout->count() > 0 ? Qt::AnchorBottom : Qt::AnchorTop);
	anchorLayout->addAnchor(this, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(this, Qt::AnchorRight, anchorTo, Qt::AnchorRight);

	anchorTo = this;

	for (auto ite = fellows.begin(); ite != fellows.end(); ++ite)
	{
		QGraphicsAnchor* anchorTop = anchorTop = anchorLayout->addAnchor(*ite, Qt::AnchorTop, anchorTo, Qt::AnchorBottom);
		anchorLayout->addAnchor(*ite, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft)->setSpacing(LEFT_SPACING);
		anchorLayout->addAnchor(*ite, Qt::AnchorRight, anchorLayout, Qt::AnchorRight);
		QGraphicsAnchor* anchorBottom = anchorLayout->addAnchor(*ite, Qt::AnchorBottom, anchorTo, Qt::AnchorBottom);
		anchorBottom->setSpacing(-minimumHeight());
		anchorTo = *ite;
	}
}

void LayerLeader::hideFellows(bool hide /*= true*/)
{
 	QGraphicsWidget* par = parentWidget();
 	if (!par)
 		return;
 
 	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
 	if (!layout)
 		return;
 
 	int index = 0;
 	for (; index < layout->count(); ++index)
 		if (layout->itemAt(index) == this)
 			break;
 
	qreal spacing = 0;
 	for (++index; index < layout->count(); ++index)
 	{
 		QGraphicsLayoutItem * fellowItem = layout->itemAt(index);
		if (LayerLeader* leader = dynamic_cast<LayerLeader*>(fellowItem))
		{
			leader->addSpacing(spacing * (hide ? -1 : 1));
			break;
		}
		else if (LayerFellow* fellow = dynamic_cast<LayerFellow*>(fellowItem))
		{
			spacing += fellow->rect().height();
 			hide ? fellow->hide() : fellow->show();
		}
 	}
}

void LayerLeader::addSpacing(const qreal spacing)
{
	if (QGraphicsAnchor* pAnchor = anchor())
	{
 		qreal val = pAnchor->spacing() + spacing;
// 		if (val < minimumHeight())
// 			val = minimumHeight();

		pAnchor->setSpacing(val);
	}
}

QGraphicsAnchor* LayerFellow::anchor()
{
	QGraphicsWidget* par = parentWidget();
	if (!par)
		return nullptr;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return nullptr;

	int index = 0;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == this)
			break;

	if (0 == index || index == layout->count())
		return nullptr;

	QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);

	return layout->anchor(this, Qt::AnchorBottom, secondItem, Qt::AnchorBottom);
}

void LayerFellow::addSpacing(const qreal spacing)
{
	if (QGraphicsAnchor* pAnchor = anchor())
	{
		qreal val = -(pAnchor->spacing() - spacing);
		if (val < minimumHeight())
			val = minimumHeight();
		pAnchor->setSpacing(-val);
	}
}

void HandleLayerLeader::addSpacing(const qreal spacing)
{
	if (m_partener)
	{
		m_partener->addSpacing(spacing);
	}
	LayerLeader::addSpacing(spacing);
}

QRectF HandleLayerLeader::handleRect()
{
	return QRectF();
	QRectF rec = rect();
	rec.adjust(0, 0, 0, -(rec.height() - 4));
	return rec;
}

void HandleLayerLeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), Qt::gray);
}

void HandleLayerFellow::addSpacing(const qreal spacing)
{
	if (m_partener)
	{
		m_partener->addSpacing(spacing);
	}
	LayerFellow::addSpacing(spacing);
}

QRectF HandleLayerFellow::handleRect()
{
	QRectF rec = rect();
	rec.adjust(0, (rec.height() - 4), 0, 4);
	return rec;
}

void HandleLayerFellow::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int delt = event->delta();
	addSpacing(delt > 0 ? -1 : 1);
}

void HandleLayerFellow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), Qt::darkGray);
}

LayerHandle::LayerHandle()
{
	setAcceptHoverEvents(true);
}

void LayerHandle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if (handleRect().contains(event->pos()))
	{
		setCursor(Qt::SizeVerCursor);
	}
}
void LayerHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
}
void LayerHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!m_bPressHandle)
		return;
	int delt = event->pos().y() - event->lastPos().y();
	addSpacing(delt);
}
void LayerHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (handleRect().contains(event->pos()))
	{
		m_bPressHandle = true;
	}
}
void LayerHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_bPressHandle = false;
}