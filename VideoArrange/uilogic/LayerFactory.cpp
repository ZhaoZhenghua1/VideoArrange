#include "LayerFactory.h"
#include "Layer/LeftHandleLayer.h"
#include "TimeZone/TimeVideoLine.h"
#include "../Document/Document.h"

#include <QGraphicsAnchorLayout>

LayerFactory::LayerFactory()
{
	Document::instance()->addObserver(this);
}


LayerFactory::~LayerFactory()
{
}

void LayerFactory::setLeftLayout(QGraphicsAnchorLayout* l)
{
	m_leftLayout = l;
}

void LayerFactory::setRightLayout(QGraphicsAnchorLayout* r)
{
	m_rightLayout = r;
}

void LayerFactory::createLayerToLayout(QDomElement data)
{
	if (!m_leftLayout || !m_rightLayout)
		return;
	
	QVector<HandleFellow*> arrLeft(4);
	std::generate(arrLeft.begin(), arrLeft.end(), []() {return new HandleFellow;});

	QVector<LayerFellow*> arrOrig(arrLeft.size());
	for (int i = 0;i < arrLeft.size(); ++i)
		arrOrig[i] = arrLeft[i];

	HandleTitle* titleLeft = new HandleTitle;
	titleLeft->addGroupToLayout(arrOrig, m_leftLayout);

	QVector<LayerFellow*> arrR(arrLeft.size());
	std::generate(arrR.begin(), arrR.end(), []() {return new LayerFellow;});

	TimeVideoLine* titleR = new TimeVideoLine;
	titleLeft->setPartner(titleR);
	titleR->initData(data);
	for (int i = 0; i < arrLeft.size(); ++i)
	{
		arrLeft[i]->setPartner(arrR[i]);
	}

	titleR->addGroupToLayout(arrR, m_rightLayout);
}

LayerFactory* LayerFactory::instance()
{
	static LayerFactory* pThis = nullptr;
	if (!pThis)
	{
		pThis = new LayerFactory;
	}
	return pThis;
}

void LayerFactory::init()
{
	if (!m_leftLayout || !m_rightLayout)
		return;
	while (m_leftLayout->count() > 0)
		delete m_leftLayout->itemAt(0);
	while (m_rightLayout->count() > 0)
		delete m_rightLayout->itemAt(0);

	QDomElement d = Observer::data({ "project","track","layerlist" });
	for (QDomElement elem = d.firstChildElement("layer");!elem.isNull(); elem = elem.nextSiblingElement("layer"))
	{
		createLayerToLayout(elem);
	}
}
