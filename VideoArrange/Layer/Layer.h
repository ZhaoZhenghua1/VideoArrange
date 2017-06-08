#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//图层,可拖动改变大小,内容由用户填充
class Layer : public QGraphicsWidget
{
	Q_OBJECT
public:
	Layer(QGraphicsWidget* widget);
	~Layer();
protected:
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)Q_DECL_OVERRIDE;
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
private:
	void setAnchor(QGraphicsAnchor* top, QGraphicsAnchor* bottom);
private:
	QGraphicsAnchor* m_anchorTop = nullptr;
	QGraphicsAnchor* m_anchorBottom = nullptr;
	friend class LayerGroup;
};

class LayerGroup
{
public:
	LayerGroup(const QVector<Layer*>& layers, QGraphicsAnchorLayout* anchor);

};

class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	virtual void addSpacing(const qreal spacing) = 0;
protected:
	virtual QGraphicsAnchor* anchor() = 0;
};

class LayerFellow : public virtual LayerBase
{
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};

class LayerLeader : public virtual LayerBase
{
public:
	LayerLeader(){}
	void addGroupToLayout(const QVector<LayerFellow*>& fellows, QGraphicsAnchorLayout* anchorLayout);
	void hideFellows(bool hide = true);
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};


class LayerHandle : virtual public LayerBase
{
public:
	LayerHandle();
protected:
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
protected:
	virtual QRectF handleRect() = 0;
private:
	bool m_bPressHandle = false;
};

class HandleLayerLeader : public LayerLeader , public LayerHandle
{
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	LayerLeader* m_partener = nullptr;
};

class HandleLayerFellow : public LayerFellow, public LayerHandle
{
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	LayerFellow* m_partener = nullptr;
};