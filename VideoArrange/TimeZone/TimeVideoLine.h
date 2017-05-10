#pragma once
#include "Layer/GraphicsLayoutRectItem.h"
#include <QGraphicsWidget>

// ±º‰œﬂ
class TimeVideoLine : public QGraphicsWidget
{
public:
	TimeVideoLine();
	~TimeVideoLine();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect)Q_DECL_OVERRIDE;

	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
};

