#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//图层,可拖动改变大小,内容由用户填充
class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	//操作时改变相对位置
	virtual void addSpacing(const qreal spacing) = 0;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	//当前项的锚点
	virtual QGraphicsAnchor* anchor() = 0;
};

//图层头下面的操作项
class LayerFellow : public virtual LayerBase
{
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};

//图层头
class LayerLeader : public virtual LayerBase
{
public:
	LayerLeader(){}
	//通过图层头将图层头和图层操作项添加到对应的布局中
	void addGroupToLayout(const QVector<LayerFellow*>& fellows, QGraphicsAnchorLayout* anchorLayout);
	//隐藏图层头项目的图层
	virtual void hideFellows(bool hide = true);
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};

//可操作的图层，响应鼠标。
class LayerHandle : virtual public LayerBase
{
public:
	LayerHandle();
protected:
	//鼠标消息
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
protected:
	//响应鼠标消息的区域
	virtual QRectF handleRect() = 0;
private:
	bool m_bPressHandle = false;
};

//响应鼠标的图层头
class HandleLayerLeader : public LayerLeader , public LayerHandle
{
public:
	//左边和右边为一个搭档，左边和右边位置共同变化
	void setPartner(LayerLeader* partner) { m_partner = partner; }
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void hideFellows(bool hide = true)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	LayerLeader* m_partner = nullptr;
};

//响应鼠标的图层操作项
class HandleLayerFellow : public LayerFellow, public LayerHandle
{
public:
	//左边和右边为一个搭档，左边和右边位置共同变化
	void setPartner(LayerFellow* partner) { m_partner = partner; }
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	LayerFellow* m_partner = nullptr;
};