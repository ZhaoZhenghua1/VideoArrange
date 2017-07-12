#include "IMagnet.h"
#include "MagnetManager.h"

IMagnet::IMagnet()
{
	MagnetManager::instance()->addMagnet(this);
}

IMagnet::~IMagnet()
{
	MagnetManager::instance()->removeMagnet(this);
}

bool IMagnet::attached(QPointF& scenePos)
{
	return false;
}
