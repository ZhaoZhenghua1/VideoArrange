#include "VideoArrange.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include "TimeZone/TimeBarView.h"
#include "TimeZone/TimePointerView.h"
#include "TimeZone/TimeVideoView.h"

VideoArrange::VideoArrange()
{
	m_sceneTopLeft = new QGraphicsScene;
	m_sceneBottomLeft = new QGraphicsScene;
	
	m_btnPlay = new QPushButton("play");
	m_btnZoomIn = new QPushButton("zout");
	m_btnZoomOut = new QPushButton("zin");

	m_labelTime = new QLabel("00:00:00.0000");
	m_timeBarView = new TimeBarView;
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
	timeLayout->addWidget(m_timeBarView);
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
	m_timeVideoView = new TimeVideoView;
	layoutTop->addWidget(m_timeVideoView);

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
	m_pViewEffectRight = new QGraphicsView;
	//pViewEffectRight->setScene(m_sceneBottomRight);
	layoutBottom->addWidget(m_pViewEffectRight);

	m_sceneTopLeft->setSceneRect(QRectF(0, 0, 118,300));
	m_sceneBottomLeft->setSceneRect(QRectF(0, 0, 118, 300));

	//滚动条设置
	m_timeVideoView->setVerticalScrollBar(pViewLayerLeft->verticalScrollBar());
	pViewLayerLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pViewLayerLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_pViewEffectRight->setVerticalScrollBar(pViewEffectLeft->verticalScrollBar());
	pViewEffectLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	pViewEffectLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setHorizontalScrollBar(m_timeBarView->horizontalScrollBar());
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeVideoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pViewEffectRight->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeVideoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pViewEffectRight->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeBarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

	m_timePointerView = new TimePointerView;
	m_timePointerView->setParent(this);

	connect(m_btnZoomIn, &QPushButton::clicked, m_timeBarView, &TimeBarView::zoomIn);
	connect(m_btnZoomIn, &QPushButton::clicked, m_timePointerView, &TimeBarView::zoomIn);
	connect(m_btnZoomIn, &QPushButton::clicked, m_timeVideoView, &TimeBarView::zoomIn);

	connect(m_btnZoomOut, &QPushButton::clicked, m_timeBarView, &TimeBarView::zoomOut);
	connect(m_btnZoomOut, &QPushButton::clicked, m_timePointerView, &TimeBarView::zoomOut);
	connect(m_btnZoomOut, &QPushButton::clicked, m_timeVideoView, &TimeBarView::zoomOut);


	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timeVideoView->horizontalScrollBar(), &QScrollBar::setValue);
	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timePointerView->horizontalScrollBar(), &QScrollBar::setValue);

// 	connect(m_timeBarView->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_timeVideoView, SLOT(update()));
// 	connect(m_timeBarView->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_timeBarView, SLOT(update()));
// 	connect(m_timeBarView->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_timePointerView, SLOT(update()));
	connect(m_timeBarView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScroll()));

	connect(m_timeBarView, &TimeBarView::sigTimebarClicked, m_timePointerView, &TimePointerView::onClickTimeBar);

	addScrollBarWidget(pWBottomLeftCorner, Qt::AlignLeft);
}


VideoArrange::~VideoArrange()
{
}

void VideoArrange::resizeEvent(QResizeEvent * event)
{
	QAbstractScrollArea::resizeEvent(event);
	QRect rectGeo(m_timeBarView->geometry().topLeft(), m_pViewEffectRight->geometry().bottomRight());
	m_timePointerView->setGeometry(rectGeo);
//	m_timePointerView->hide();
}

void VideoArrange::onScroll()
{
	resize(size() + QSize(0,1));
	resize(size() + QSize(0, -1));
}
