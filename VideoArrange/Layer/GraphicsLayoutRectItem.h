#pragma once
#include <QGraphicsLayoutItem>
#include <QGraphicsRectItem>

//���ֹ��ܣ��ؼ�����
class GraphicsLayoutRectItem : public QGraphicsLayoutItem, public QGraphicsRectItem
{
public:
	GraphicsLayoutRectItem();
	~GraphicsLayoutRectItem();
public:
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect)Q_DECL_OVERRIDE;
};

