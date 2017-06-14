#pragma once

#include <QGraphicsWidget>
#include "Layer.h"

//Í¼²ãÍ·£¬°üº¬Í¼²ãÃû£¬ÉìËõ°´Å¥£¬ÏÔÊ¾£¬ÉùÒô£¬Ëø¶¨µÈ¿ØÖÆÍ¼²ãµÄ°´Å¥
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
