#include "TimeLineField.h"

#include "../TimeZone/TimeBarView.h"
#include "../TimeZone/TimePointerView.h"
#include "../TimeZone/TimeVideoView.h"
#include "../Layer/LayerView.h"
#include "../Controls/TimeLineScrollBar.h"
#include "../Controls/StatusButton.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include <QTime>
#include <QTimer>


const int LEFT_WIDTH = 112;
TimeLineField::TimeLineField()
{
	QPalette pale = palette();
	pale.setColor(QPalette::Window, QColor(33,33,33));
	setPalette(pale);
	setAutoFillBackground(true);

	viewport()->setStyleSheet("background-color: rgb(33, 33, 33);");
	setFrameShape(QFrame::NoFrame);

	//label和其他控件
	QWidget* controls = new QWidget;
	m_labelTime = new QLabel("00:00:00.00");
	m_labelTime->setStyleSheet(R"(color: rgb(44, 140, 180);font: 12pt "Cambria Math";)");

	m_btnPlay = new StatusButton;
	m_btnPlay->setFixedSize(33, 20);
	m_btnPlay->registerStatus({ 1,2 });
	m_btnPlay->setStatusPixmap(1, ":/play_off_hover.png", ":/play_off_hover.png");
	m_btnPlay->setStatusPixmap(2, ":/play_on_hover.png", ":/play_on_hover.png");
	m_btnMagnet = new StatusButton;
	m_btnMagnet->registerStatus({ 1,2 });
	m_btnMagnet->setStatusPixmap(1, ":/magnet_off_normal.png", ":/magnet_off_hover.png");
	m_btnMagnet->setStatusPixmap(2, ":/magnet_on_normal.png", ":/magnet_on_hover.png");
	m_btnMagnet->setFixedSize(33, 20);
	QVBoxLayout* vLayout = new QVBoxLayout;
	controls->setLayout(vLayout);
	vLayout->addWidget(m_labelTime);
	QHBoxLayout* hLayout = new QHBoxLayout;
	vLayout->addLayout(hLayout);
	hLayout->addWidget(m_btnMagnet);
	hLayout->addWidget(m_btnPlay);
	controls->setFixedSize(QSize(236, 45));

	m_leftLayerView = new LayerView;

	QWidget* leftW = new QWidget;
	leftW->setFixedWidth(226);
	QVBoxLayout* leftLay = new QVBoxLayout;
	leftLay->setSpacing(0);
	leftLay->setContentsMargins(0, 5, 0, 0);
	leftW->setLayout(leftLay);
	leftLay->addWidget(controls);
	leftLay->addWidget(m_leftLayerView);

	m_timeBarView = new TimeBarView;
	m_timeVideoView = new TimeVideoView;

	QWidget* rightW = new QWidget;
	QVBoxLayout* rightLay = new QVBoxLayout;
	rightLay->setSpacing(0);
	rightLay->setContentsMargins(0, 9, 0, 0);
	rightW->setLayout(rightLay);
	rightLay->addWidget(m_timeBarView);
	rightLay->addWidget(m_timeVideoView);
	

	//m_labelTime->setStyleSheet("background-color: gray;");
	//m_labelTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	

	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0 ,0 ,0);
	layout->setSpacing(0);
	setLayout(layout);
	layout->addWidget(leftW);
	layout->addWidget(rightW);

	//滚动条设置
	m_leftLayerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_leftLayerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// 	m_pViewEffectRight->setVerticalScrollBar(pViewEffectLeft->verticalScrollBar());
// 	pViewEffectLeft->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	pViewEffectLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	TimeLineScrollBar* timelineScrollBar = new TimeLineScrollBar;
	m_timeBarView->setHorizontalScrollBar(timelineScrollBar);
	m_timeBarView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 11px; }");
	setHorizontalScrollBar(m_timeBarView->horizontalScrollBar());
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeVideoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_leftLayerView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 11px; }");
	setVerticalScrollBar(m_leftLayerView->verticalScrollBar());
	m_timeVideoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_timeBarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	m_pViewEffectRight->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	m_pViewEffectRight->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


 	m_timePointerView = new TimePointerView;
 	m_timePointerView->setParent(this);

	connect(timelineScrollBar, &TimeLineScrollBar::adjustWidth, m_timeVideoView, &TimeBarView::onAdjustWidth);
	connect(timelineScrollBar, &TimeLineScrollBar::adjustWidth, m_timeBarView, &TimeBarView::onAdjustWidth);
	connect(timelineScrollBar, &TimeLineScrollBar::adjustWidth, m_timePointerView, &TimeBarView::onAdjustWidth);

	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timeVideoView->horizontalScrollBar(), &QScrollBar::setValue);
	connect(m_timeBarView->horizontalScrollBar(), &QScrollBar::valueChanged, m_timePointerView->horizontalScrollBar(), &QScrollBar::setValue);

	connect(m_leftLayerView->verticalScrollBar(), &QScrollBar::valueChanged, m_timeVideoView->verticalScrollBar(), &QScrollBar::setValue);

	connect(m_timeBarView, &TimeBarView::sigTimebarClicked, m_timePointerView, &TimePointerView::onClickTimeBar);
	connect(m_timeBarView, &TimeBarView::sigTimebarClicked, this, &TimeLineField::onClickTimeBar);

	//connect(m_btnTime, SIGNAL(clicked()), this, SLOT(onClickBtnTime()));
	//addScrollBarWidget(pWBottomLeftCorner, Qt::AlignLeft);
}

TimeLineField::~TimeLineField()
{
}

void TimeLineField::onClickTimeBar(qreal t)
{
	QTime time = QTime(0, 0).addMSecs(t + 0.5);
	m_labelTime->setText(time.toString("hh:mm:ss.zzz"));
}

void TimeLineField::onClickBtnTime()
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

void TimeLineField::resizeEvent(QResizeEvent * event)
{
	QAbstractScrollArea::resizeEvent(event);
	QTimer::singleShot(0, [this]() 
	{
		m_timePointerView->setGeometry(mapFromGlobal(m_timeVideoView->mapToGlobal(QPoint())).x(), 10, m_timeVideoView->width(), viewport()->height());
	});
}

