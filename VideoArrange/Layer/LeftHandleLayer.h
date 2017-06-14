#pragma once

#include <QGraphicsWidget>
#include "Layer.h"

//ͼ��ͷ������ͼ������������ť����ʾ�������������ȿ���ͼ��İ�ť
class QLabel;

class HandleTitle : public HandleLayerLeader
{
public:
	HandleTitle();
public:
void onStatusChanged(int before, int after);
};

class Helper : public QObject
{
	Q_OBJECT
public:
	Helper(HandleTitle* parent) :QObject(parent), m_help(parent) {}
	public slots :
		void onStatusChanged(int before, int after)
	{
		m_help->onStatusChanged(before, after);
	}
private:
	HandleTitle* m_help;
};

class HandleFellow : public HandleLayerFellow
{
public:
	HandleFellow();
	void setWidget(QGraphicsWidget* widget);
};
