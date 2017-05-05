#include "LayerEditor.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include "TimeBar.h"


LayerEditor::LayerEditor()
{
	m_sceneTopLeft = new QGraphicsScene;
	m_sceneTopRight = new QGraphicsScene;
	m_sceneBottomLeft = new QGraphicsScene;
	m_sceneBottomRight = new QGraphicsScene;
	m_btnPlay = new QPushButton("play");
	m_btnZoomIn = new QPushButton("zout");
	m_btnZoomOut = new QPushButton("zin");

	m_labelTime = new QLabel("00:00:00.0000");
	m_timeBar = new TimeBar;
	m_btnTime = new QPushButton("t");

	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0 ,0 ,0);
	QSplitter* splitter = new QSplitter(Qt::Vertical);
	layout->addWidget(splitter);
	setLayout(layout);

	//上方时间条
	QWidget* pWTime = new QWidget;
	pWTime->setFixedHeight(25);
	QHBoxLayout* timeLayout = new QHBoxLayout;
	timeLayout->setSpacing(1);
	timeLayout->setContentsMargins(0, 0, 0, 0);
	pWTime->setLayout(timeLayout);
	m_labelTime->setFixedWidth(118);
	m_btnTime->setFixedWidth(16);
	timeLayout->addWidget(m_labelTime);
	timeLayout->addWidget(m_timeBar);
	timeLayout->addWidget(m_btnTime);



	//中间图层编辑
	QWidget* pWTop = new QWidget;
	QHBoxLayout* layoutTop = new QHBoxLayout;
	layoutTop->setContentsMargins(0, 0, 0, 0);
	layoutTop->setSpacing(1);
	pWTop->setLayout(layoutTop);
	QGraphicsView* pViewLayerLeft = new QGraphicsView;
	pViewLayerLeft->setScene(m_sceneTopLeft);
	pViewLayerLeft->setFixedWidth(118);
	layoutTop->addWidget(pViewLayerLeft);
	QGraphicsView* pViewLayerRight = new QGraphicsView;
	pViewLayerRight->setScene(m_sceneTopRight);
	layoutTop->addWidget(pViewLayerRight);

	QWidget* pWTimeLayer = new QWidget;
	QVBoxLayout* layoutTimeLayer = new QVBoxLayout;
	layoutTimeLayer->setSpacing(0);
	layoutTimeLayer->setContentsMargins(0, 0, 0, 0);
	pWTimeLayer->setLayout(layoutTimeLayer);
	layoutTimeLayer->addWidget(pWTime);
	layoutTimeLayer->addWidget(pWTop);
	splitter->addWidget(pWTimeLayer);

	//下方效果编辑
	QWidget* PWBottom = new QWidget;
	splitter->addWidget(PWBottom);
	QHBoxLayout* layoutBottom = new QHBoxLayout;
	layoutBottom->setContentsMargins(0, 0, 0, 0);
	layoutBottom->setSpacing(1);
	PWBottom->setLayout(layoutBottom);
	QGraphicsView* pViewEffectLeft = new QGraphicsView;
	pViewEffectLeft->setFixedWidth(118);
	pViewEffectLeft->setScene(m_sceneBottomLeft);
	layoutBottom->addWidget(pViewEffectLeft);
	QGraphicsView* pViewEffectRight = new QGraphicsView;
	pViewEffectRight->setScene(m_sceneBottomRight);
	layoutBottom->addWidget(pViewEffectRight);

	m_sceneTopLeft->setSceneRect(QRectF(0, 0, 118,300));
	m_sceneBottomLeft->setSceneRect(QRectF(0, 0, 118, 300));
	m_sceneTopRight->setSceneRect(QRectF(0, 0, 1180, 300));
	m_sceneBottomRight->setSceneRect(QRectF(0, 0, 1180, 300));

	//滚动条设置
	pViewLayerRight->setVerticalScrollBar(pViewLayerLeft->verticalScrollBar());
	pViewLayerLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pViewLayerLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	pViewEffectRight->setVerticalScrollBar(pViewEffectLeft->verticalScrollBar());
	pViewEffectLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pViewEffectLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setHorizontalScrollBar(m_timeBar->horizontalScrollBar());
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	pViewLayerRight->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pViewEffectRight->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//左下角播放，放大，缩小 按钮
	QWidget* pWBottomLeftCorner = new QWidget;
	QHBoxLayout* cornerLayout = new QHBoxLayout;
	cornerLayout->setSpacing(0);
	cornerLayout->setContentsMargins(0, 0, 0, 0);
	pWBottomLeftCorner->setLayout(cornerLayout);
	pWBottomLeftCorner->setFixedWidth(118);
	cornerLayout->addWidget(m_btnPlay);
	cornerLayout->addWidget(m_btnZoomOut);
	cornerLayout->addWidget(m_btnZoomIn);

	addScrollBarWidget(pWBottomLeftCorner, Qt::AlignLeft);
}


LayerEditor::~LayerEditor()
{
}
