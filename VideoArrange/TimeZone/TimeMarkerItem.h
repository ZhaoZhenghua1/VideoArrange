#pragma once

#include <QGraphicsRectItem>
#include <QDomElement>
#include <QGraphicsWidget>

#include "Controls/IOriginatorEditor.h"
#include "Controls/IMagnet.h"

class TimeZone;
class TimeMarkerItem : public QGraphicsRectItem, public IOriginator, public IMagnet
{
public:
	TimeMarkerItem();
	~TimeMarkerItem();
public:
	void updatePos();
	void updateTime();
	void initData(const QDomElement& media);
	QDomElement data() { return m_dataElem; }
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual  QVariant itemChange(GraphicsItemChange change, const QVariant &value)Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *event)override;
	virtual void setQsData(const QString& data) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual QString toQsData()override;
	//Îü¸½ÅÐ¶Ï
	virtual bool attached(QPointF& scenePos);
private:
	TimeZone* timeZone();
	int startTime();
private:
	//Êý¾Ý
	QDomElement m_dataElem;
	QPixmap m_markerMask;
};

class PositionMarker
{

};

class PauseMarker
{

};

class JumpMarker
{

};
