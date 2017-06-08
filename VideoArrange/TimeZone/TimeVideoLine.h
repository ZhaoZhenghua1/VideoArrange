#pragma once
#include "Layer/GraphicsLayoutRectItem.h"

#include <QDomElement>
#include <QGraphicsWidget>
#include"../Layer/Layer.h"

class TimeZone;
// ±º‰œﬂ
class TimeVideoLine : public LayerLeader
{
public:
	TimeVideoLine();
	~TimeVideoLine();

	void initData(const QDomElement& elem);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect)Q_DECL_OVERRIDE;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
private:
	TimeZone* timeZone();
private:
//	TimeZone* m_timezone = nullptr;
	QDomElement m_dataElem;
};

