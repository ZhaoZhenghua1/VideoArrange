#pragma once
#include <QVector>

class IMagnet;

class MagnetManager
{
public:
	MagnetManager();
	~MagnetManager();

	static MagnetManager* instance();
public:
	//ÅĞ¶ÏÊÇ·ñÓĞÎü¸½
	bool attached(QPointF& scenePos, IMagnet* mag);
	void setMagnet(bool magnet) { m_bMagnetOn = magnet; }
private:
	void addMagnet(IMagnet* magnet);
	void removeMagnet(IMagnet* magnet);
	friend class IMagnet;
private:
	QVector<IMagnet*> m_magnets;
	bool m_bMagnetOn = false;
};
