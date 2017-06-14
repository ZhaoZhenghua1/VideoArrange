#include "RightLayer.h"

#include <QGraphicsAnchorLayout>

RightLayer::RightLayer()
{
	QGraphicsAnchorLayout* lay = new QGraphicsAnchorLayout;
	lay->setContentsMargins(0, 0, 0, 0);
	lay->setSpacing(0);
	setLayout(lay);
}


RightLayer::~RightLayer()
{
}
