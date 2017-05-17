#include "VideoArrange.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include <QTime>

#include "TimeZone/TimeBarView.h"
#include "TimeZone/TimePointerView.h"
#include "TimeZone/TimeVideoView.h"
#include "Layer/LayerView.h"

const int LEFT_WIDTH = 112;
VideoArrange::VideoArrange()
{
	resize(900, 600);
	m_sceneTopLeft = new QGraphicsScene;
	m_sceneBottomLeft = new QGraphicsScene;
	
	m_btnPlay = new QPushButton("play");
	m_btnZoomIn = new QPushButton("zin");
	m_btnZoomOut = new QPushButton("zout");

	m_labelTime = new QLabel("00:00:00.0000");

	QPalette palette = m_labelTime->palette();
	palette.setColor(QPalette::Window, Qt::gray);
	m_labelTime->setPalette(palette);
	m_labelTime->setAutoFillBackground(true);

	//m_labelTime->setStyleSheet("background-color: gray;");
	m_labelTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
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
	m_labelTime->setFixedWidth(LEFT_WIDTH);
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
	m_leftLayerView = new LayerView;
	m_leftLayerView->setFixedWidth(LEFT_WIDTH);
	layoutTop->addWidget(m_leftLayerView);
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
// 	QWidget* PWBottom = new QWidget;
// 	splitter->addWidget(PWBottom);
// 	QHBoxLayout* layoutBottom = new QHBoxLayout;
// 	layoutBottom->setContentsMargins(0, 0, 0, 0);
// 	layoutBottom->setSpacing(1);
// 	PWBottom->setLayout(layoutBottom);
// 	QGraphicsView* pViewEffectLeft = new QGraphicsView;
// 	pViewEffectLeft->setFixedWidth(LEFT_WIDTH);
// 	pViewEffectLeft->setScene(m_sceneBottomLeft);
// 	layoutBottom->addWidget(pViewEffectLeft);
// 	m_pViewEffectRight = new QGraphicsView;
// 	//pViewEffectRight->setScene(m_sceneBottomRight);
// 	layoutBottom->addWidget(m_pViewEffectRight);

	m_sceneTopLeft->setSceneRect(QRectF(0, 0, LEFT_WIDTH,300));
	m_sceneBottomLeft->setSceneRect(QRectF(0, 0, LEFT_WIDTH, 300));

	//滚动条设置
//	m_leftLayerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_leftLayerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// 	m_pViewEffectRight->setVerticalScrollBar(pViewEffectLeft->verticalScrollBar());
// 	pViewEffectLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	pViewEffectLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setHorizontalScrollBar(m_timeBarView->horizontalScrollBar());
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeVideoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeVideoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeBarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	m_pViewEffectRight->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	m_pViewEffectRight->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	//左下角播放，放大，缩小 按钮
	QWidget* pWBottomLeftCorner = new QWidget;
	QHBoxLayout* cornerLayout = new QHBoxLayout;
	cornerLayout->setSpacing(0);
	cornerLayout->setContentsMargins(0, 0, 0, 0);
	pWBottomLeftCorner->setLayout(cornerLayout);
	pWBottomLeftCorner->setFixedWidth(LEFT_WIDTH);
	cornerLayout->addWidget(m_btnPlay);
	cornerLayout->addWidget(m_btnZoomOut);
	cornerLayout->addWidget(m_btnZoomIn);

 	m_timePointerView = new TimePointerView;
 	m_timePointerView->setParent(this);

	connect(m_btnZoomIn, &QPushButton::clicked, m_timeVideoView, &TimeBarView::zoomIn);
	connect(m_btnZoomIn, &QPushButton::clicked, m_timeBarView, &TimeBarView::zoomIn);
	connect(m_btnZoomIn, &QPushButton::clicked, m_timePointerView, &TimeBarView::zoomIn);

	connect(m_btnZoomOut, &QPushButton::clicked, m_timeBarView, &TimeBarView::zoomOut);
	connect(m_btnZoomOut, &QPushButton::clicked, m_timePointerView, &TimeBarView::zoomOut);
	connect(m_btnZoomOut, &QPushButton::clicked, m_timeVideoView, &TimeBarView::zoomOut);


	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timeVideoView->horizontalScrollBar(), &QScrollBar::setValue);
	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timePointerView->horizontalScrollBar(), &QScrollBar::setValue);

	connect(m_timeVideoView->verticalScrollBar(), &QScrollBar::valueChanged, m_leftLayerView->verticalScrollBar(), &QScrollBar::setValue);

	connect(m_timeBarView, &TimeBarView::sigTimebarClicked, m_timePointerView, &TimePointerView::onClickTimeBar);
	connect(m_timeBarView, &TimeBarView::sigTimebarClicked, this, &VideoArrange::onClickTimeBar);

	connect(m_btnTime, SIGNAL(clicked()), this, SLOT(onClickBtnTime()));
	addScrollBarWidget(pWBottomLeftCorner, Qt::AlignLeft);
}

VideoArrange::~VideoArrange()
{
}

void VideoArrange::onClickTimeBar(qreal t)
{
	QTime time = QTime(0, 0).addMSecs(t + 0.5);
	m_labelTime->setText(time.toString("hh:mm:ss.zzz"));
}

void VideoArrange::onClickBtnTime()
{
	if (0 == m_timePointerView->currentTime())
	{
		m_timeBarView->horizontalScrollBar()->setValue(0);
	}
	else
	{
		emit m_timeBarView->sigTimebarClicked(0);
	}
}

void VideoArrange::resizeEvent(QResizeEvent * event)
{
	QAbstractScrollArea::resizeEvent(event);
	m_timePointerView->setGeometry(114, 1, m_timeVideoView->viewport()->width(), viewport()->height());
}

