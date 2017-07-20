#pragma once
#include <QGraphicsRectItem>
#include <QDomElement>
#include <QGraphicsWidget>
#include <QPointer> 

#include "Controls/EffectEditor.h"
#include "Controls/IOriginatorEditor.h"
#include "Controls/IMagnet.h"
//��Դ��
class SizeGripItem;
class TimeZone;
class EffectEditor;
class IEditor;
class TimeVideoItem : public QGraphicsRectItem, public IOriginator, public IMagnet
{
public:
	TimeVideoItem();
	~TimeVideoItem();
	void setRect(const QRectF& rect);
	void updatePos();
	void updateTime();
	bool initData(const QDomElement& elem);
	QDomElement data();
	void createEffectEdit();
	void effectEditParentChanged();
	void updateEffectEditPos();
private:
	TimeZone* timeZone();
	int startTime();
	int timeLen();
	QString resID()const;
	IEditor* editor();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual  QVariant itemChange(GraphicsItemChange change, const QVariant &value)Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	//���ݱ༭����
	virtual void setQsData(const QString& data) override;
	virtual QString toQsData()override;
	//�����ж�
	virtual bool attached(QPointF& scenePos);
private:
	//�ı��С����
	SizeGripItem* m_sizeGrip = nullptr;
	//����
	QDomElement m_dataElem;
	//Ч���༭����
	QVector<QPointer<EffectEditor> > m_effectEdits;
};
