#include "MagnetManager.h"
#include "IMagnet.h"


MagnetManager::MagnetManager()
{
}


MagnetManager::~MagnetManager()
{
}

MagnetManager* MagnetManager::instance()
{
	static MagnetManager * ins = nullptr;
	if (!ins)
	{
		ins = new MagnetManager;
	}
	return ins;
}

void MagnetManager::addMagnet(IMagnet* magnet)
{
	m_magnets.push_back(magnet);
}

void MagnetManager::removeMagnet(IMagnet* magnet)
{
	m_magnets.removeAll(magnet);
}

bool MagnetManager::attached(QPointF& scenePos, IMagnet* origmag)
{
	//是否启用吸附功能
	if (!m_bMagnetOn)
	{
		return false;
	}

	for (IMagnet* mag : m_magnets)
	{
		if (mag && origmag!= mag && mag->attached(scenePos))
		{
			return true;
		}
	}
	return false;
}
