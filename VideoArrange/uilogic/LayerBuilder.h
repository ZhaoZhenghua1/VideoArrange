#pragma once
#include "../Document/Observer.h"

class QGraphicsAnchorLayout;
class LayerBuilder : public Observer
{
public:
	LayerBuilder();
	~LayerBuilder();
	void setLeftLayout(QGraphicsAnchorLayout* l);
	void setRightLayout(QGraphicsAnchorLayout* r);
	void createPlayItemLayer(const QDomElement& data);
	void createMarkerItemLayer(const QDomElement& data);
	static LayerBuilder* instance();
protected:
	virtual void init() override;
private:
	QGraphicsAnchorLayout* m_leftLayout = nullptr;
	QGraphicsAnchorLayout* m_rightLayout = nullptr;

};

