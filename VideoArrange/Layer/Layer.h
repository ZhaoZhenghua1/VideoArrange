#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//ͼ��,���϶��ı��С,�������û����
class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	//����ʱ�ı����λ��
	virtual void addSpacing(const qreal spacing) = 0;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	//��ǰ���ê��
	virtual QGraphicsAnchor* anchor() = 0;
};

//ͼ��ͷ����Ĳ�����
class LayerFellow : public virtual LayerBase
{
public:
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};

//ͼ��ͷ
class LayerLeader : public virtual LayerBase
{
public:
	LayerLeader(){}
	//ͨ��ͼ��ͷ��ͼ��ͷ��ͼ���������ӵ���Ӧ�Ĳ�����
	void addGroupToLayout(const QVector<LayerFellow*>& fellows, QGraphicsAnchorLayout* anchorLayout);
	//����ͼ��ͷ��Ŀ��ͼ��
	virtual void hideFellows(bool hide = true);
	virtual void addSpacing(const qreal spacing) override;
protected:
	virtual QGraphicsAnchor* anchor() override;
};

//�ɲ�����ͼ�㣬��Ӧ��ꡣ
class LayerHandle : virtual public LayerBase
{
public:
	LayerHandle();
protected:
	//�����Ϣ
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
protected:
	//��Ӧ�����Ϣ������
	virtual QRectF handleRect() = 0;
private:
	bool m_bPressHandle = false;
};

//��Ӧ����ͼ��ͷ
class HandleLayerLeader : public LayerLeader , public LayerHandle
{
public:
	//��ߺ��ұ�Ϊһ�������ߺ��ұ�λ�ù�ͬ�仯
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

//��Ӧ����ͼ�������
class HandleLayerFellow : public LayerFellow, public LayerHandle
{
public:
	//��ߺ��ұ�Ϊһ�������ߺ��ұ�λ�ù�ͬ�仯
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