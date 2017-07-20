#pragma once
#include "../Document/Observer.h"

class QGraphicsAnchorLayout;
class WhiteWidget;
class LayerBuilder : public Observer
{
public:
	LayerBuilder();
	~LayerBuilder();
	void setLeftLayout(QGraphicsAnchorLayout* l);
	void setRightLayout(QGraphicsAnchorLayout* r);
	void createPlayItemLayer(const QDomElement& data);
	void createPlayItemLayer();
	void createMarkerItemLayer(const QDomElement& data);
	void createMarkerItemLayer();
	void coverWithWhiteWidget();
	static LayerBuilder* instance();
protected:
	virtual void init() override;
private:
	QGraphicsAnchorLayout* m_leftLayout = nullptr;
	QGraphicsAnchorLayout* m_rightLayout = nullptr;
	WhiteWidget* m_leftWhiteWidget = nullptr;
	WhiteWidget* m_rightWhiteWidget = nullptr;
};

