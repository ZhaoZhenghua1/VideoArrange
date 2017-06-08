#pragma once

#include <QGraphicsWidget>
#include "Layer.h"

//Í¼²ãÍ·£¬°üº¬Í¼²ãÃû£¬ÉìËõ°´Å¥£¬ÏÔÊ¾£¬ÉùÒô£¬Ëø¶¨µÈ¿ØÖÆÍ¼²ãµÄ°´Å¥
class QLabel;
class LayerTitle : public QGraphicsWidget
{
public:
	LayerTitle();
	~LayerTitle();
public:
	void setName(const QString& name);
protected:
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)Q_DECL_OVERRIDE;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
private:
	QLabel* m_labelName = nullptr;
private:
	bool m_bIsPressTitle = false;
};


class TitleLayer : public HandleLayerLeader
{
public:
	TitleLayer();
public:
void onStatusChanged(int before, int after);
};

class Helper : public QObject
{
	Q_OBJECT
public:
	Helper(TitleLayer* parent) :QObject(parent), m_help(parent) {}
	public slots :
		void onStatusChanged(int before, int after)
	{
		m_help->onStatusChanged(before, after);
	}
private:
	TitleLayer* m_help;
};

class FellowLayer : public HandleLayerFellow
{
public:
	FellowLayer();
	void setWidget(QGraphicsWidget* widget);
};