#include "GraphicsLayoutTextItem.h"



GraphicsLayoutTextItem::GraphicsLayoutTextItem()
{
	m_textItem = new QGraphicsTextItem(this);
}


GraphicsLayoutTextItem::GraphicsLayoutTextItem(const QString& text):GraphicsLayoutTextItem()
{
	m_textItem->setPlainText(text);
}

GraphicsLayoutTextItem::~GraphicsLayoutTextItem()
{
}

void GraphicsLayoutTextItem::setText(const QString& text)
{
	return m_textItem->setPlainText(text);
}

void GraphicsLayoutTextItem::setGeometry(const QRectF &rect)
{
	GraphicsLayoutRectItem::setGeometry(rect);
	m_textItem->setPos(rect.topLeft());
}
