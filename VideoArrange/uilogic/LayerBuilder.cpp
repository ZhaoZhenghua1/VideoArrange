#include "LayerBuilder.h"
#include "Layer/LeftHandleLayer.h"
#include "Layer/RightLayer.h"
#include "TimeZone/TimeVideoLine.h"
#include "../Document/Document.h"
#include "Controls/EffectValueEditor.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>

LayerBuilder::LayerBuilder()
{
	Document::instance()->addObserver(this);
}


LayerBuilder::~LayerBuilder()
{
}

void LayerBuilder::setLeftLayout(QGraphicsAnchorLayout* l)
{
	m_leftLayout = l;
}

void LayerBuilder::setRightLayout(QGraphicsAnchorLayout* r)
{
	m_rightLayout = r;
}

void LayerBuilder::createPlayItemLayer(const QDomElement& data)
{
	if (!m_leftLayout || !m_rightLayout)
		return;
	//���
	LeftMediaLeader* titleLeft = new LeftMediaLeader;
	QVector<LeftHandleFellow*> leftFellows = titleLeft->init(m_leftLayout);
	titleLeft->setWidget(new MediaItemWidgetEditor);

	TimeVideoLine* titleR = new TimeVideoLine;
	titleR->setEditor(titleLeft->editor());

	//��ʼ�����ݣ�����������Ϳؼ�
	QVector<RightLayer*> rightLayers = titleR->initData(data, m_rightLayout);
	//����ͷ�Ķ�Ӧ��ϵ
	titleLeft->setPartner(titleR);

	if (leftFellows.size() != rightLayers.size())
	{
		return;
	}
	
	for (int i = 0; i < leftFellows.size(); ++i)
	{
		//��������Ķ�Ӧ��ϵ
		leftFellows[i]->setPartner(rightLayers[i]);
		//���ñ༭
		rightLayers[i]->setEditor(leftFellows[i]->editor());
	}
}

void LayerBuilder::createMarkerItemLayer(const QDomElement& data)
{

}

LayerBuilder* LayerBuilder::instance()
{
	static LayerBuilder* pThis = nullptr;
	if (!pThis)
	{
		pThis = new LayerBuilder;
	}
	return pThis;
}

void LayerBuilder::init()
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
		createPlayItemLayer(elem);
	}
}
