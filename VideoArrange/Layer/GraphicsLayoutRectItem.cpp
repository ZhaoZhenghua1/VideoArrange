#include "GraphicsLayoutRectItem.h"
#include <QPen>


GraphicsLayoutRectItem::GraphicsLayoutRectItem()
{
	setGraphicsItem(this);
	//û�б߿򣬱߿���þ��ε�boundingRect�����õĴ�
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
