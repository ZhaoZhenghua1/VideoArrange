#pragma once
#include <QPointF>

//�����������ܽӿ�
class IMagnet
{
public:
	IMagnet();
	virtual ~IMagnet() = 0;
	//��ǰ�ĵ�scenePos������Ϊ���صĵ�
	//����ʱ����TRUE��ͬʱ�޸Ĵ����scenePos��ֵ
	virtual bool attached(QPointF& scenePos);
};

