#include "LayerBuilder.h"
#include "Layer/LeftHandleLayer.h"
#include "Layer/RightLayer.h"
#include "TimeZone/TimeVideoLine.h"
#include "../Document/Document.h"
#include "Controls/EffectValueEditor.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QTimer>

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
	//左边
	LeftMediaLeader* titleLeft = new LeftMediaLeader;
	QVector<LeftHandleFellow*> leftFellows = titleLeft->init(data, m_leftLayout);
	titleLeft->setWidget(new MediaItemWidgetEditor);

	TimeVideoLine* titleR = new TimeVideoLine;
	titleR->setEditor(titleLeft->editor());

	//初始化数据，并创建子项和控件
	QVector<RightLayer*> rightLayers = titleR->initData(data, m_rightLayout);
	//设置头的对应关系
	titleLeft->setPartner(titleR);

	if (leftFellows.size() != rightLayers.size())
	{
		return;
	}
	
	for (int i = 0; i < leftFellows.size(); ++i)
	{
		//设置子项的对应关系
		leftFellows[i]->setPartner(rightLayers[i]);
		//设置编辑
		rightLayers[i]->setEditor(leftFellows[i]->editor());
	}

	coverWithWhiteWidget();
}

void LayerBuilder::createPlayItemLayer()
{
	QDomElement d = Observer::data({ "project","track","layerlist" });
	if (d.isNull())
	{
		Q_ASSERT(false);
		return;
	}
	QDomElement createElem = Document::instance()->document().createElement("layer");
	createElem.setAttribute("layerName", QString::fromLocal8Bit("图层"));
	d.appendChild(createElem);
	QDomElement createElemMedialst = Document::instance()->document().createElement("medialist");
	createElem.appendChild(createElemMedialst);
	createPlayItemLayer(createElem);
}

void LayerBuilder::createMarkerItemLayer(const QDomElement& data)
{
	if (!m_leftLayout || !m_rightLayout)
		return;
// 	//左边
 	LeftMarkerLeader* titleLeft = new LeftMarkerLeader;
	titleLeft->init(data, m_leftLayout);
 	titleLeft->setWidget(new MarkerWidetEditor);
// 
 	TimeMarkerLine* titleR = new TimeMarkerLine;
	titleR->initData(data, m_rightLayout);
	titleR->setEditor(titleLeft->editor());
	//设置头的对应关系
	titleLeft->setPartner(titleR);

	coverWithWhiteWidget();
}

void LayerBuilder::createMarkerItemLayer()
{
	QDomElement d = Observer::data({ "project","track","layerlist" });
	if (d.isNull())
	{
		Q_ASSERT(false);
		return;
	}
	QDomElement createElem = Document::instance()->document().createElement("layer");
	createElem.setAttribute("layerName", QString::fromLocal8Bit("控制"));
	d.appendChild(createElem);
	QDomElement createElemMedialst = Document::instance()->document().createElement("marklist");
	createElem.appendChild(createElemMedialst);
	createMarkerItemLayer(createElem);
}

void LayerBuilder::coverWithWhiteWidget()
{
	delete m_leftWhiteWidget;
	delete m_rightWhiteWidget;
	
	m_leftWhiteWidget = new WhiteWidget;
	m_rightWhiteWidget = new WhiteWidget;

	m_leftWhiteWidget->init(m_leftLayout);
	m_rightWhiteWidget->init(m_rightLayout);

	QTimer::singleShot(0, [this]() {
		m_leftWhiteWidget->adjustScene();
		m_rightWhiteWidget->adjustScene();
	});
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
		if (elem.firstChildElement().nodeName() == "medialist")
		{
			createPlayItemLayer(elem);
		}
		else if (elem.firstChildElement().nodeName() == "marklist")
		{
			createMarkerItemLayer(elem);
		}
	}
}
