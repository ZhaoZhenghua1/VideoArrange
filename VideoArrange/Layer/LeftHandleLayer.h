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
//ͼƬ����Ƶ����Ƶͼ�� ������ͼ������������ť����ʾ�������������ȿ���ͼ��İ�ť
class LeftMediaLeader : public HandleLayerLeader
{
public:
	LeftMediaLeader();
	//���ñ༭����
	void setWidget(MediaItemWidgetEditor* widget);
	//��ȡ�༭����
	IEditor* editor();
	QVector<LeftHandleFellow*> init(QGraphicsAnchorLayout* leftLayout);
public:
void onHideFellows(int before, int after);
private:
	IEditor* m_editor = nullptr;
};

//������ͼ��
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

//ͼ������Ĳ�����
class LeftHandleFellow : public HandleLayerFellow
{
public:
	LeftHandleFellow();
	//���ñ༭����
	void setWidget(EffectValueEditor* widget);
	//��ȡ�༭����
	IEditor* editor();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};


