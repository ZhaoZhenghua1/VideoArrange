#pragma once
#include "../Document/Observer.h"

class QGraphicsAnchorLayout;
class LayerFactory : public Observer
{
public:
	LayerFactory();
	~LayerFactory();
	void setLeftLayout(QGraphicsAnchorLayout* l);
	void setRightLayout(QGraphicsAnchorLayout* r);
	void createLayerToLayout(QDomElement data);
	static LayerFactory* instance();
protected:
	virtual void init() override;
private:
	QGraphicsAnchorLayout* m_leftLayout = nullptr;
	QGraphicsAnchorLayout* m_rightLayout = nullptr;

};

