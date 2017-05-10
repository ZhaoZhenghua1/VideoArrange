#pragma once
#include "GraphicsLayoutRectItem.h"
#include <QGraphicsTextItem>

class GraphicsLayoutTextItem : public GraphicsLayoutRectItem
{
public:
	GraphicsLayoutTextItem();
	GraphicsLayoutTextItem(const QString& text);
	~GraphicsLayoutTextItem();
public:
	void setText(const QString& text);
protected:
	virtual void setGeometry(const QRectF &rect)Q_DECL_OVERRIDE;
private:
	QGraphicsTextItem* m_textItem;
};

