#include "GraphicsLayoutRectItem.h"
#include <QPen>


GraphicsLayoutRectItem::GraphicsLayoutRectItem()
{
	setGraphicsItem(this);
	//没有边框，边框会让矩形的boundingRect比设置的大
	setPen(QPen(Qt::NoPen));
}


GraphicsLayoutRectItem::~GraphicsLayoutRectItem()
{
}

QSizeF GraphicsLayoutRectItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint /*= QSizeF()*/) const 
{
	Q_UNUSED(constraint);
	Q_UNUSED(which);

	return rect().size();
}

void GraphicsLayoutRectItem::setGeometry(const QRectF &rect) 
{
	QGraphicsLayoutItem::setGeometry(rect);
	setRect(rect);
}
