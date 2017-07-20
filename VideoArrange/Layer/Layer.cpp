#include "Layer.h"
#include "LeftHandleLayer.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>

const unsigned int SPACING = 0;//2;
LayerBase::LayerBase()
{
	setMinimumHeight(26);
}

void LayerBase::addHeight(const qreal spacing)
{
	QGraphicsAnchor* pAnchor = bottomAnchor();

	if (pAnchor)
	{
		qreal val = pAnchor->spacing() - spacing;
		Q_ASSERT(val < 0);
		if (-val <  minimumHeight() + SPACING)
		{
			val = -(minimumHeight() + SPACING);
		}
		if (-val > maxHeight())
		{
			val = -maxHeight();
		}

		pAnchor->setSpacing(val);
	}

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

	for (--index; index >= 0; --index)
	{
		QGraphicsLayoutItem * fellowItem = layout->itemAt(index);
		if (LayerLeader* leader = dynamic_cast<LayerLeader*>(fellowItem))
		{
			leader->adjustLayout();
			break;
		}
	}
}

void LayerBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(rect(), QColor(32, 32, 32));
	painter->fillRect(QRectF(0,rect().height()-2,rect().width(),2), QColor(49, 49, 49));
}


QGraphicsAnchor* LayerBase::bottomAnchor()
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

	QGraphicsAnchor* pAnchor = nullptr;
	if (0 == index)
	{
		pAnchor = layout->anchor(this, Qt::AnchorBottom, layout, Qt::AnchorTop);
	}
	else
	{
		QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);
		pAnchor = layout->anchor(this, Qt::AnchorBottom, secondItem, Qt::AnchorBottom);
	}
	return pAnchor;
}

qreal LayerBase::maxHeight()
{
	return minimumHeight() + 200;
}

LayerLeader::~LayerLeader()
{
	QGraphicsWidget* par = parentWidget();
	if (!par)
		return ;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return ;

	int index = 0;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == this)
			break;

	QVector<QGraphicsLayoutItem*> subItems;
	QGraphicsLayoutItem* nextLeaderItem = nullptr;
	for (int ind = index + 1; ind < layout->count(); ++ind)
	{
		if (dynamic_cast<LayerLeader*>(layout->itemAt(ind)))
		{
			nextLeaderItem = layout->itemAt(ind);
			break;
		}
		subItems.push_back(layout->itemAt(ind));
	}

	QGraphicsLayoutItem* beforeLeaderItem = layout;
	qreal spacing = 0;
	for (int ind = index - 1; ind >= 0; --ind)
	{
		if (dynamic_cast<LayerLeader*>(layout->itemAt(ind)))
		{
			beforeLeaderItem = layout->itemAt(ind);
			//当前项到上一项的距离，当前项被删除后，将这个距离设置到下一项上
			if (QGraphicsAnchor* anchor = layout->anchor(this, Qt::AnchorTop, beforeLeaderItem, beforeLeaderItem == layout ? Qt::AnchorTop : Qt::AnchorBottom))
			{
				spacing = anchor->spacing();
			}
			break;
		}
	}
	for (QGraphicsLayoutItem* item : subItems)
	{
		delete item;
	}
	if (nextLeaderItem)
	{
		int sig = beforeLeaderItem == layout ? -1 : 1;
		layout->addAnchor(nextLeaderItem, Qt::AnchorTop, beforeLeaderItem, beforeLeaderItem == layout ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing(spacing*sig);
		qreal height = dynamic_cast<QGraphicsWidget*>(nextLeaderItem)->rect().height();
		layout->addAnchor(nextLeaderItem, Qt::AnchorBottom, beforeLeaderItem, beforeLeaderItem == layout ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing((-spacing - height)*sig);
		layout->addAnchor(nextLeaderItem, Qt::AnchorLeft, beforeLeaderItem, Qt::AnchorLeft);
		layout->addAnchor(nextLeaderItem, Qt::AnchorRight, beforeLeaderItem, Qt::AnchorRight);
	}
	adjustScene();
}

void LayerLeader::addGroupToLayout(const QVector<LayerBase*>& fellows, QGraphicsAnchorLayout* anchorLayout)
{
	QGraphicsLayoutItem* anchorTo = anchorLayout;

	qreal spacing = 0;
	for (int i = anchorLayout->count() - 1; i >= 0; --i )
	{
		QGraphicsLayoutItem* item = anchorLayout->itemAt(i);
		if (LayerLeader* leader = dynamic_cast<LayerLeader*>(item))
		{
			anchorTo = leader;
			break;
		}
		else if (LayerBase* fellow = dynamic_cast<LayerBase*>(item))
		{
			if (fellow->isVisible())
			{
				spacing += fellow->rect().height() + SPACING;
			}
		}
	}


	bool top = anchorTo == anchorLayout;
	anchorLayout->addAnchor(this, Qt::AnchorTop, anchorTo, top ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing(spacing);
	anchorLayout->addAnchor(this, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(this, Qt::AnchorRight, anchorTo, Qt::AnchorRight);
	anchorLayout->addAnchor(this, Qt::AnchorBottom, anchorTo, top ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing((minimumHeight() + spacing)*(top ? 1 : -1));//*(top?1:-1)

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

	//更新场景高度
	adjustScene();
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
			break;
		}
		else if (LayerBase* fellow = dynamic_cast<LayerBase*>(fellowItem))
		{
 			hide ? fellow->hide() : fellow->show();
		}
 	}
	//隐藏当前下拉项后，调整其他项的位置
	adjustLayout();
}

void LayerLeader::adjustLayout()
{
	QGraphicsWidget* par = parentWidget();
	if (!par)
		return;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return;

	//当前项的位置
	int index = 0;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == this)
			break;

	//从当前项开始，一行一行计算
	qreal spacing = 0;
	QGraphicsLayoutItem* pAnchorTo = this;
	for (++index; index < layout->count(); ++index)
	{
		QGraphicsLayoutItem * fellowItem = layout->itemAt(index);
		//找到下一行的头，直接调整
		if (LayerLeader* leader = dynamic_cast<LayerLeader*>(fellowItem))
		{
			if(QGraphicsAnchor* anchorTop = layout->anchor(fellowItem, Qt::AnchorTop, this, Qt::AnchorBottom))
				anchorTop->setSpacing(-spacing);
			if(QGraphicsAnchor* anchorBottom = layout->anchor(fellowItem, Qt::AnchorBottom, this, Qt::AnchorBottom))
				anchorBottom->setSpacing(spacing - leader->rect().height());
			break;
		}
		//是当前项的下拉项，则计算高度
		else if (LayerBase* fellow = dynamic_cast<LayerBase*>(fellowItem))
		{
			if (fellow->isVisible())
			{
				if (QGraphicsAnchor* anchor = layout->anchor(fellow, Qt::AnchorBottom, pAnchorTo, Qt::AnchorBottom))
				{
					spacing += anchor->spacing();
				}
			}
		}
		pAnchorTo = layout->itemAt(index);
	}

	//调整场景大小
	adjustScene();
}

void LayerLeader::adjustScene()
{
	//场景超出视图时
	int viewHeight = scene()->views().at(0)->viewport()->rect().height();

	QGraphicsWidget* par = parentWidget();
	if (!par)
		return;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return;

	qreal height = 0;
	for (int index = layout->count() - 2; index >= 0; --index)
	{
		if (QGraphicsWidget * pW = dynamic_cast<QGraphicsWidget*>(layout->itemAt(index)))
		{
			if (pW->isVisible())
			{
				height = pW->geometry().bottom();
				break;
			}
		}
	}
	height = (height > viewHeight ? height : viewHeight);
	height += 200;
	
	QRectF sceneRect = scene()->sceneRect();
	sceneRect.setHeight(height);
	scene()->setSceneRect(sceneRect);
}

QGraphicsAnchor* LayerLeader::bottomAnchor()
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
		return layout->anchor(this, Qt::AnchorBottom, layout, Qt::AnchorTop);
	}
	else
	{
		for (--index;index >= 0;--index)
		{
			QGraphicsLayoutItem * secondItem = layout->itemAt(index);
			if (dynamic_cast<LayerLeader*>(secondItem))
			{
				return layout->anchor(this, Qt::AnchorBottom, secondItem, Qt::AnchorBottom);
			}
		}
	}
	return nullptr;
}

void LayerLeader::addHeight(const qreal spacing)
{
	qreal baseHeight = 0;
	QGraphicsWidget* par = parentWidget();
	if (!par)
		return ;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return ;

	int index = 0;
	for (; index < layout->count(); ++index)
		if (layout->itemAt(index) == this)
			break;

	if (index == layout->count())
		return ;
	bool bTop = index == 0;
	for (--index;index >= 0;--index)
	{
		QGraphicsLayoutItem * secondItem = layout->itemAt(index);
		if (dynamic_cast<LayerLeader*>(secondItem))
		{
			break;
		}
		else if (LayerBase* b = dynamic_cast<LayerBase*>(secondItem))
		{
			if (b->isVisible())
			{
				baseHeight += b->rect().height();
			}
		}
	}

	QGraphicsAnchor* pAnchor = bottomAnchor();
	if (pAnchor)
	{
		int sig = bTop ? 1 : -1;
		qreal val = (pAnchor->spacing() + spacing*(sig))*sig;
		if (val < baseHeight + minimumHeight() + SPACING)
		{
			val = (baseHeight + minimumHeight() + SPACING);
		}
		if (val > maxHeight() + baseHeight)
		{
			val = maxHeight() + baseHeight;
		}

		pAnchor->setSpacing(val*sig);
	}
	adjustScene();
}

HandleLayerLeader::~HandleLayerLeader()
{
	delete m_partner;
}

void HandleLayerLeader::addHeight(const qreal spacing)
{
	if (m_partner)
	{
		m_partner->addHeight(spacing);
	}
	LayerLeader::addHeight(spacing);
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
//	scene()->update(scene()->itemsBoundingRect());
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

//鼠标滚动，调整大小
void LayerHandle::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	int delt = event->delta();
	addHeight(delt > 0 ? -2 : 2);
}
