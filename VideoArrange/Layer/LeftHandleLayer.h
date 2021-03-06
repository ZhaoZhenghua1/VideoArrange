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
class MarkerWidetEditor;
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
	//���ñ༭����
	void setWidget(MarkerWidetEditor* widget);
	void init(const QDomElement& data, QGraphicsAnchorLayout* leftLayout);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	//��ȡ�༭����
	IEditor* editor() { return m_editor; }
public:
public:
	void onEditFinished();
private:
	IEditor* m_editor = nullptr;
	QLineEdit* m_editLayerName = nullptr;
	QDomElement m_data;
};

class MediaHelper : public QObject
{
	Q_OBJECT
public:
	MediaHelper(LeftMediaLeader* parent) :QObject(parent), m_help(parent) {}
public slots :
	void onStatusChanged(int before, int after)
	{
		m_help->onHideFellows(before, after);
	}
	void onEditFinished()
	{
		m_help->onEditFinished();
	}
	void onDelete();
private:
	LeftMediaLeader* m_help;
};
class MarkerHelper : public QObject
{
	Q_OBJECT
public:
	MarkerHelper(LeftMarkerLeader* parent) :QObject(parent), m_help(parent) {}
	public slots :
		void onStatusChanged(int before, int after)
	{
		
	}
	void onEditFinished()
	{
		m_help->onEditFinished();
	}
	void onDelete();
private:
	LeftMarkerLeader* m_help;
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

//���Ϊ�˸�ס�������ڵĲ��ֵĿհ״���
class WhiteWidget : public LayerLeader
{
public:
	void init(QGraphicsAnchorLayout* layout);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
