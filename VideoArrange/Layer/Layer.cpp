#include "Layer.h"
#include "LeftHandleLayer.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPainter>
#include <QGraphicsScene>

const unsigned int SPACING = 0;//2;
LayerBase::LayerBase()
{
	setMinimumHeight(26);
}

void LayerBase::addHeight(const qreal spacing)
{
	bool ret = false;
	if (QGraphicsAnchor* pAnchor = bottomAnchor(ret))
	{
		int sig = ret ? 1 : -1;
		qreal val = (pAnchor->spacing() - spacing * (-sig))*sig;
		if (val < minimumHeight() + SPACING)
			val = minimumHeight() + SPACING;
		else if (val > maxHeight())
			val = maxHeight();
		pAnchor->setSpacing(val*(sig));
	}
}

void LayerBase::addSpace(const qreal spacing)
{
	if (QGraphicsAnchor* pAnchor = topAnchor())
	{
		qreal val = pAnchor->spacing() + spacing;
		// 		if (val < minimumHeight())
		// 			val = minimumHeight();

		pAnchor->setSpacing(val);
	}
	bool ret = false;
	if (QGraphicsAnchor* pAnchor = bottomAnchor(ret))
	{
		int sig = ret ? 1 : -1;
		qreal val = pAnchor->spacing() + spacing*sig;
		// 		if (val < minimumHeight())
		// 			val = minimumHeight();

		pAnchor->setSpacing(val);
	}
}

void LayerBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), QColor(32, 32, 32));
	painter->fillRect(QRectF(0,rect().height()-2,rect().width(),2), QColor(49, 49, 49));
}

QGraphicsAnchor* LayerBase::bottomAnchor(bool& retIsFirst)
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

	if (index == layout->count())
		return nullptr;

	if (0 == index)
	{
		retIsFirst = true;
		return layout->anchor(this, Qt::AnchorBottom, layout, Qt::AnchorTop);
	}
	else
	{
		retIsFirst = false;
		QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);

		return layout->anchor(this, Qt::AnchorBottom, secondItem, Qt::AnchorBottom);
	}
}

QGraphicsAnchor* LayerBase::topAnchor()
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

	if (index == layout->count())
		return nullptr;

	if (0 == index)
	{
		return layout->anchor(this, Qt::AnchorTop, layout, Qt::AnchorTop);
	}
	else
	{
		QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);

		return layout->anchor(this, Qt::AnchorTop, secondItem, Qt::AnchorBottom);
	}
}

qreal LayerBase::maxHeight()
{
	return minimumHeight() + 200;
}

void LayerLeader::addGroupToLayout(const QVector<LayerBase*>& fellows, QGraphicsAnchorLayout* anchorLayout)
{
	QGraphicsLayoutItem* anchorTo = anchorLayout;
	if (anchorLayout->count() > 0)
		anchorTo = anchorLayout->itemAt(anchorLayout->count() - 1);

	bool top = anchorTo == anchorLayout;
	anchorLayout->addAnchor(this, Qt::AnchorTop, anchorTo, top ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing(SPACING);
	anchorLayout->addAnchor(this, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(this, Qt::AnchorRight, anchorTo, Qt::AnchorRight);
	anchorLayout->addAnchor(this, Qt::AnchorBottom, anchorTo, top ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing((minimumHeight() + SPACING)*(top ? 1 : -1));//*(top?1:-1)

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
			leader->LayerLeader::addSpace(spacing * (hide ? -1 : 1));
			break;
		}
		else if (LayerBase* fellow = dynamic_cast<LayerBase*>(fellowItem))
		{
			spacing += fellow->rect().height() + SPACING;
 			hide ? fellow->hide() : fellow->show();
		}
 	}
}

HandleLayerLeader::~HandleLayerLeader()
{

}

void HandleLayerLeader::addHeight(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addHeight(spacing);
	}
	LayerLeader::addHeight(spacing);
}

void HandleLayerLeader::addSpace(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addSpace(spacing);
	}
	LayerLeader::addSpace(spacing);
}

QRectF HandleLayerLeader::handleRect()
{
	QRectF rec = rect();
	rec.adjust(0, (rec.height() - 4), 0, 4);
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
//	painter->fillRect(rect(), QColor(38, 38, 38));
	LayerLeader::paint(painter, option, widget);
}

void HandleLayerFellow::addHeight(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addHeight(spacing);
	}
	LayerBase::addHeight(spacing);
}

QRectF HandleLayerFellow::handleRect()
{
	QRectF rec = rect();
	rec.adjust(0, (rec.height() - 4), 0, 4);
	return rec;
}

void HandleLayerFellow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
//	painter->fillRect(rect(), QColor(38, 38, 38));
	LayerHandle::paint(painter, option, widget);
}

LayerHandle::LayerHandle()
{
	setAcceptHoverEvents(true);
}


void LayerHandle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	//解决闪烁问题
	scene()->update(scene()->itemsBoundingRect());
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
	//解决闪烁问题
	scene()->update(scene()->itemsBoundingRect());
}
void LayerHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
	//解决闪烁问题
	scene()->update(scene()->itemsBoundingRect());
}
void LayerHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!m_bPressHandle)
		return;
	int delt = event->pos().y() - event->lastPos().y();
	addHeight(delt);
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

void LayerHandle::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int delt = event->delta();
	addHeight(delt > 0 ? -2 : 2);
}
