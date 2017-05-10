#pragma once
#include "GraphicsLayoutRectItem.h"
#include <QObject>

class SwitchButton: public QObject, public GraphicsLayoutRectItem
{
	Q_OBJECT
public:
	SwitchButton();
	~SwitchButton();
public:
	void setSwitchPixmap(const QString& map1, const QString& map2);
protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
signals:
bool sigStatusAboutToChange(bool status);
private:
	QPixmap m_qsMapStatus0;
	QPixmap m_qsMapStatus1;
	bool m_bStatus = true;
};

