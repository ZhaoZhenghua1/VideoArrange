#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//图层,可拖动改变大小,内容由用户填充
class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	//增加高度
	virtual void addHeight(const qreal spacing);
	//增加距离
//	virtual void addSpace(const qreal spacing);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	//底部锚点，用来改变高度 、、retIsFirst 判断是不是第一个锚点，第一个锚点相对于布局，与其他锚点不同
//	QGraphicsAnchor* bottomAnchor(bool& retIsFirst);
	//顶部锚点，用来改变和上一锚点的相对位置
//	QGraphicsAnchor* topAnchor();
	virtual QGraphicsAnchor* bottomAnchor();
	virtual qreal maxHeight();
};

//图层头
class LayerLeader : public virtual LayerBase
{
public:
	LayerLeader(){}
	~LayerLeader();
	
	//通过图层头将图层头和图层操作项添加到对应的布局中
	void addGroupToLayout(const QVector<LayerBase*>& fellows, QGraphicsAnchorLayout* anchorLayout);
	//隐藏图层头项目的图层
	virtual void hideFellows(bool hide = true);
	//调整布局
	void adjustLayout();

	void adjustScene();
	virtual void addHeight(const qreal spacing);
	virtual QGraphicsAnchor* bottomAnchor();
};

//可操作的图层，响应鼠标。
class LayerHandle : public virtual LayerBase
{
public:
	LayerHandle();
protected:
	//鼠标消息
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)override;
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
	~HandleLayerLeader();
	//左边和右边为一个搭档，左边和右边位置共同变化
	void setPartner(LayerLeader* partner) { m_partner = partner; }
public:
	virtual void addHeight(const qreal spacing) override;
//	virtual void addSpace(const qreal spacing)override;
protected:
	virtual QRectF handleRect() override;
	virtual void hideFellows(bool hide = true)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	LayerLeader* m_partner = nullptr;
};

//响应鼠标的图层操作项
class HandleLayerFellow : public LayerHandle
{
public:
	//左边和右边为一个搭档，左边和右边位置共同变化
	void setPartner(LayerBase* partner) { m_partner = partner; }
public:
	virtual void addHeight(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	LayerBase* m_partner = nullptr;
};