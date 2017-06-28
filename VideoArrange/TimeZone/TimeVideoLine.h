#pragma once
#include "Layer/GraphicsLayoutRectItem.h"

#include <QDomElement>
#include <QGraphicsWidget>
#include"../Layer/Layer.h"

class TimeZone;
class QGraphicsAnchorLayout;
class RightLayer;
class IEditor;
class IOriginator;
//播放对象轨道
class TimeVideoLine : public LayerLeader
{
public:
	TimeVideoLine();
	~TimeVideoLine();

	IEditor* editor() { return m_editor; }
	void setEditor(IEditor* editor) { m_editor = editor; }
	void setOriginator(IOriginator* o);
	IOriginator* originator() { return m_selectedOriginator; }
	QVector<RightLayer*> initData(const QDomElement& elem, QGraphicsAnchorLayout* layout);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect)Q_DECL_OVERRIDE;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

	virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
	virtual void dropEvent(QGraphicsSceneDragDropEvent *event)Q_DECL_OVERRIDE;
private:
	TimeZone* timeZone();
private:
	QDomElement m_dataElem;
	IEditor* m_editor = nullptr;
	IOriginator* m_selectedOriginator = nullptr;
};

//指令轨道
class TimeMarkerLineHelper;
class TimeMarkerLine : public LayerLeader
{
public:
	TimeMarkerLine();
	void initData(const QDomElement& elem, QGraphicsAnchorLayout* layout);
	void setOriginator(IOriginator* o);
public:
	void onAction();
protected:
	virtual void setGeometry(const QRectF &rect)override;
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
	TimeZone* timeZone();
private:
	QPointF m_createPos;
	QDomElement m_dataElem;
	IEditor* m_editor = nullptr;
	IOriginator* m_selectedOriginator = nullptr;
	TimeMarkerLineHelper* m_helper = nullptr;
};

class TimeMarkerLineHelper : public QObject
{
	Q_OBJECT
public:
	TimeMarkerLineHelper(TimeMarkerLine* marker) :QObject(marker), m_markerLine(marker){}
public slots:
	void onAction()
	{
	m_markerLine->onAction();
	}
private:
	TimeMarkerLine* m_markerLine = nullptr;
};