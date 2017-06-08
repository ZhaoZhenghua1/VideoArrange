#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//图层,可拖动改变大小,内容由用户填充
class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	virtual void addSpacing(const qreal spacing) = 0;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
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
	virtual void hideFellows(bool hide = true);
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
	void setPartner(LayerLeader* partner) { m_partner = partner; }
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void hideFellows(bool hide = true)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	LayerLeader* m_partner = nullptr;
};

class HandleLayerFellow : public LayerFellow, public LayerHandle
{
public:
	void setPartner(LayerFellow* partner) { m_partner = partner; }
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	LayerFellow* m_partner = nullptr;
};