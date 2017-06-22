#pragma once

#include <QGraphicsWidget>
#include "Layer.h"

class QLabel;
class QDomElement;
class QGraphicsAnchorLayout;
class IEditor;
class EffectValueEditor;
class LeftHandleFellow;
class MediaItemWidgetEditor;
//图片，视频，音频图层 ，包含图层名，伸缩按钮，显示，声音，锁定等控制图层的按钮
class LeftMediaLeader : public HandleLayerLeader
{
public:
	LeftMediaLeader();
	//设置编辑窗口
	void setWidget(MediaItemWidgetEditor* widget);
	//获取编辑窗口
	IEditor* editor();
	QVector<LeftHandleFellow*> init(QGraphicsAnchorLayout* leftLayout);
public:
void onHideFellows(int before, int after);
private:
	IEditor* m_editor = nullptr;
};

//控制器图层
class LeftMarkerLeader : public HandleLayerLeader
{
public:
	LeftMarkerLeader();
	void init(const QDomElement& data);
public:

};

class Helper : public QObject
{
	Q_OBJECT
public:
	Helper(LeftMediaLeader* parent) :QObject(parent), m_help(parent) {}
public slots :
	void onStatusChanged(int before, int after)
	{
		m_help->onHideFellows(before, after);
	}
private:
	LeftMediaLeader* m_help;
};

//图层下面的操作项
class LeftHandleFellow : public HandleLayerFellow
{
public:
	LeftHandleFellow();
	//设置编辑窗口
	void setWidget(EffectValueEditor* widget);
	//获取编辑窗口
	IEditor* editor();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};


