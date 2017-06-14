#pragma once
#include <QGraphicsRectItem>
#include <QDomElement>
#include <QGraphicsWidget>

#include "Controls/EffectEdit.h"
//×ÊÔ´Ïî
class SizeGripItem;
class TimeZone;
class EffectEdit;
class TimeVideoItem : public QGraphicsRectItem
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
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual  QVariant itemChange(GraphicsItemChange change, const QVariant &value)Q_DECL_OVERRIDE;
private:
	QString m_qsRes;
	unsigned int m_startTime = 0;
	unsigned int m_timeLen = 0;
	SizeGripItem* m_sizeGrip = nullptr;
	QDomElement m_dataElem;
	QVector<EffectEdit*> m_effectEdits;
};
