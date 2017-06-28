#pragma once
#include <QGraphicsWidget>

class QGraphicsAnchor;
class QGraphicsAnchorLayout;
//ͼ��,���϶��ı��С,�������û����
class LayerBase : public virtual QGraphicsWidget
{
public:
	LayerBase();
	//���Ӹ߶�
	virtual void addHeight(const qreal spacing);
	//���Ӿ���
//	virtual void addSpace(const qreal spacing);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	//�ײ�ê�㣬�����ı�߶� ����retIsFirst �ж��ǲ��ǵ�һ��ê�㣬��һ��ê������ڲ��֣�������ê�㲻ͬ
//	QGraphicsAnchor* bottomAnchor(bool& retIsFirst);
	//����ê�㣬�����ı����һê������λ��
//	QGraphicsAnchor* topAnchor();
	virtual QGraphicsAnchor* bottomAnchor();
	virtual qreal maxHeight();
};

//ͼ��ͷ
class LayerLeader : public virtual LayerBase
{
public:
	LayerLeader(){}
	~LayerLeader();
	
	//ͨ��ͼ��ͷ��ͼ��ͷ��ͼ���������ӵ���Ӧ�Ĳ�����
	void addGroupToLayout(const QVector<LayerBase*>& fellows, QGraphicsAnchorLayout* anchorLayout);
	//����ͼ��ͷ��Ŀ��ͼ��
	virtual void hideFellows(bool hide = true);
	//��������
	void adjustLayout();

	void adjustScene();
	virtual void addHeight(const qreal spacing);
	virtual QGraphicsAnchor* bottomAnchor();
};

//�ɲ�����ͼ�㣬��Ӧ��ꡣ
class LayerHandle : public virtual LayerBase
{
public:
	LayerHandle();
protected:
	//�����Ϣ
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void wheelEvent(QGraphicsSceneWheelEvent *event)override;
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
	~HandleLayerLeader();
	//��ߺ��ұ�Ϊһ�������ߺ��ұ�λ�ù�ͬ�仯
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

//��Ӧ����ͼ�������
class HandleLayerFellow : public LayerHandle
{
public:
	//��ߺ��ұ�Ϊһ�������ߺ��ұ�λ�ù�ͬ�仯
	void setPartner(LayerBase* partner) { m_partner = partner; }
public:
	virtual void addHeight(const qreal spacing) override;
protected:
	virtual QRectF handleRect() override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
protected:
	LayerBase* m_partner = nullptr;
};