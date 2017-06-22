#pragma once
#include <QGraphicsRectItem>
#include <QDomElement>
#include <QGraphicsWidget>

#include "Controls/EffectEditor.h"
#include "Controls/IOriginatorEditor.h"
//资源项
class SizeGripItem;
class TimeZone;
class EffectEditor;
class IEditor;
class TimeVideoItem : public QGraphicsRectItem, public IOriginator
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
	virtual void setQsData(const QString& data) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual QString toQsData()override;
private:
	//改变大小工具
	SizeGripItem* m_sizeGrip = nullptr;
	//数据
	QDomElement m_dataElem;
	//效果编辑窗口
	QVector<EffectEditor*> m_effectEdits;
};
