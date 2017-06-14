#include "Layer.h"
#include "LeftHandleLayer.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPainter>

const unsigned int MIN_SPACE = 14;
const unsigned int MAX_SPACE = 100;
const int LEFT_SPACING = 40;
const unsigned int SPACING = 2;
LayerBase::LayerBase()
{
	setMinimumHeight(24);
}

void LayerBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), QColor(32, 32, 32));
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

	anchorLayout->addAnchor(this, Qt::AnchorTop, anchorTo, anchorLayout->count() > 0 ? Qt::AnchorBottom : Qt::AnchorTop)->setSpacing(SPACING);
	anchorLayout->addAnchor(this, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(this, Qt::AnchorRight, anchorTo, Qt::AnchorRight);

	anchorTo = this;

	for (auto ite = fellows.begin(); ite != fellows.end(); ++ite)
	{
		anchorLayout->addAnchor(*ite, Qt::AnchorTop, anchorTo, Qt::AnchorBottom)->setSpacing(SPACING);
		anchorLayout->addAnchor(*ite, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
		anchorLayout->addAnchor(*ite, Qt::AnchorRight, anchorLayout, Qt::AnchorRight);
		QGraphicsAnchor* anchorBottom = anchorLayout->addAnchor(*ite, Qt::AnchorBottom, anchorTo, Qt::AnchorBottom);
		anchorBottom->setSpacing(-minimumHeight() - SPACING);
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
			leader->LayerLeader::addSpacing(spacing * (hide ? -1 : 1));
			break;
		}
		else if (LayerFellow* fellow = dynamic_cast<LayerFellow*>(fellowItem))
		{
			spacing += fellow->rect().height() + SPACING;
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
		if (val < minimumHeight() + SPACING)
			val = minimumHeight() + SPACING;
		pAnchor->setSpacing(-val);
	}
}

void HandleLayerLeader::addSpacing(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addSpacing(spacing);
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

void HandleLayerLeader::hideFellows(bool hide /*= true*/)
{
	if (m_partner)
	{
		m_partner->hideFellows(hide);
	}
	LayerLeader::hideFellows(hide);
	update();
}

void HandleLayerLeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), QColor(38, 38, 38));
}

void HandleLayerFellow::addSpacing(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addSpacing(spacing);
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
	painter->fillRect(rect(), QColor(38, 38, 38));
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
	else
	{
		setCursor(Qt::ArrowCursor);
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