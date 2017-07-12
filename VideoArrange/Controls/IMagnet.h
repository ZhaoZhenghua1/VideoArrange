#pragma once
#include <QPointF>

//磁铁吸附功能接口
class IMagnet
{
public:
	IMagnet();
	virtual ~IMagnet() = 0;
	//当前的点scenePos被吸附为返回的点
	//吸附时返回TRUE，同时修改传入的scenePos的值
	virtual bool attached(QPointF& scenePos);
};

