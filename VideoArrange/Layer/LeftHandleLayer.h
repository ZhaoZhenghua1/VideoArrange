#pragma once

#include <QGraphicsWidget>
#include <QDomElement>
#include "Layer.h"

class QLabel;
class QDomElement;
class QGraphicsAnchorLayout;
class IEditor;
class EffectValueEditor;
class LeftHandleFellow;
class MediaItemWidgetEditor;
class QLineEdit;
//ͼƬ����Ƶ����Ƶͼ�� ������ͼ������������ť����ʾ�������������ȿ���ͼ��İ�ť
class LeftMediaLeader : public HandleLayerLeader
{

public:
	LeftMediaLeader();
	~LeftMediaLeader();
	//���ñ༭����
	void setWidget(MediaItemWidgetEditor* widget);
	//��ȡ�༭����
	IEditor* editor();
	QVector<LeftHandleFellow*> init(const QDomElement& data,QGraphicsAnchorLayout* leftLayout);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
	void onEditFinished();
	void onHideFellows(int before, int after);
private:
	IEditor* m_editor = nullptr;
	QLineEdit* m_editLayerName = nullptr;
	QDomElement m_data;
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
	void onEditFinished()
	{
		m_help->onEditFinished();
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


