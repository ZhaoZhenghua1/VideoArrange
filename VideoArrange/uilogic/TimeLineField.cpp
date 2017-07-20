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
#include <QMenu>
#include <QTimeEdit>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>

#include "LayerBuilder.h"
#include "Document/Document.h"
#include "Controls/MagnetManager.h"

const int LEFT_WIDTH = 112;
unsigned int MIN_TIME_LEN = 10 * 1000;
TimeLineField::TimeLineField()
{
	Document::instance()->addObserver(this);
	QPalette pale = palette();
	pale.setColor(QPalette::Window, QColor(33,33,33));
	setPalette(pale);
	setAutoFillBackground(true);

	viewport()->setStyleSheet("background-color: rgb(33, 33, 33);");
	setFrameShape(QFrame::NoFrame);

	//label�������ؼ�
	QWidget* controls = new QWidget;
	controls->setFixedSize(QSize(236, 45));

	QLabel* labelTL = new QLabel(QString::fromLocal8Bit("ʱ��:"), controls);
	labelTL->setStyleSheet("color: rgb(163, 163, 163);");
	labelTL->setGeometry(2, 2, 33, 20);
	QLabel* labelT = new QLabel(QString::fromLocal8Bit("ʱ��:"), controls);
	labelT->setStyleSheet("color: rgb(163, 163, 163);");
	labelT->setGeometry(110, 2, 33, 20);

	ClickTimeEdit* timeLen = new ClickTimeEdit("00:30:00.00", controls);
	m_labelTimeLen = timeLen;
	timeLen->setGeometry(30, 2, 77, 20);
	timeLen->setStyleSheet(R"(color: rgb(45, 140, 235);font: 10pt "Cambria Math";)");
	connect(timeLen, &ClickTimeEdit::timeSetted, this, &TimeLineField::onTimeLengthSetted);

	ClickTimeEdit * labelTime = new ClickTimeEdit("00:00:00.00", controls);
	m_labelTime = labelTime;
	m_labelTime->setGeometry(140, 2, 77, 20);
	m_labelTime->setStyleSheet(R"(color: rgb(45, 140, 235);font: 10pt "Cambria Math";)");
	connect(labelTime, &ClickTimeEdit::timeSetted, this, &TimeLineField::onTimeSetted);

	//���Ű�ť����Ϊ���㴰��
	m_btnPlay = new StatusButton(controls);
	m_btnPlay->setShortcut(QKeySequence("Space"));
	
	m_btnPlay->setGeometry(2,25,33, 20);
	m_btnPlay->registerStatus({ 1,2 });
	m_btnPlay->setStatusPixmap(1, ":/play_off_hover.png", ":/play_off_hover.png");
	m_btnPlay->setStatusPixmap(2, ":/play_on_hover.png", ":/play_on_hover.png");
	connect(m_btnPlay, &StatusButton::statusChanged, this, &TimeLineField::onPlayChanged);
	
	m_btnMagnet = new StatusButton(controls);
	connect(m_btnMagnet, &StatusButton::statusChanged, this, &TimeLineField::onMagnetChanged);
	m_btnMagnet->registerStatus({ 1,2 });
	m_btnMagnet->setStatusPixmap(1, ":/magnet_off_normal.png", ":/magnet_off_hover.png");
	m_btnMagnet->setStatusPixmap(2, ":/magnet_on_normal.png", ":/magnet_on_hover.png");
	m_btnMagnet->setGeometry(37,25,33, 20);

	QPushButton* btnAddLayer = new QPushButton(controls);
	btnAddLayer->setGeometry(187,23,33, 20);
	
	QString stylesheet = R"(QPushButton{border-image:url(:/add_layer.png);}
QPushButton:hover{border-image:url(:/add_layer.png);}
QPushButton:pressed{border-image:url(:/add_layer.png);})";

	btnAddLayer->setStyleSheet(stylesheet);
	connect(btnAddLayer, &QPushButton::clicked, this, &TimeLineField::onAddLayer);

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
	m_timeVideoView->viewport()->installEventFilter(this);

	QWidget* rightW = new QWidget;
	QVBoxLayout* rightLay = new QVBoxLayout;
	rightLay->setSpacing(0);
	rightLay->setContentsMargins(0, 9, 0, 0);
	rightW->setLayout(rightLay);
	rightLay->addWidget(m_timeBarView);
	rightLay->addWidget(m_timeVideoView);

	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0 ,0 ,0);
	layout->setSpacing(0);
	setLayout(layout);
	layout->addWidget(leftW);
	layout->addWidget(rightW);

	//����������
	m_leftLayerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_leftLayerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	TimeLineScrollBar* timelineScrollBar = new TimeLineScrollBar;
	m_timeBarView->setHorizontalScrollBar(timelineScrollBar);
	m_timeBarView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 11px; }");
	setHorizontalScrollBar(m_timeBarView->horizontalScrollBar());
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_timeVideoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeVideoView->setHorScrollBar(timelineScrollBar);
	m_leftLayerView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 11px; }");
	setVerticalScrollBar(m_leftLayerView->verticalScrollBar());
	m_timeVideoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_timeBarView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_timeBarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//���ſ���
	m_playControl = new MediaPlayControl;
	connect(m_playControl, SIGNAL(updateTimePointer(unsigned int)), this, SLOT(onUpdateRunningTime(unsigned int)));
	connect(m_playControl, SIGNAL(sigGetDocument()), this, SLOT(onGetDocument()));

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
}

TimeLineField::~TimeLineField()
{
}

void TimeLineField::onClickTimeBar(qreal t)
{
	unsigned int utime = t + 0.5;
	QTime time = QTime(0, 0).addMSecs(utime);
	m_labelTime->setText(time.toString("hh:mm:ss.zzz"));

	locateTo(utime);
}

void TimeLineField::onAddLayer()
{
	QMenu menu;
	menu.setStyleSheet(R"(color: rgb(231, 231, 231);
background-color: rgb(49, 49, 49);)");
	QAction* addVideoLayer = menu.addAction(QString::fromLocal8Bit("���������"));
	connect(addVideoLayer, &QAction::triggered, this, &TimeLineField::onAddVideoLayer);
	QAction* addCtrLayer = menu.addAction(QString::fromLocal8Bit("��ӿ��Ʋ�"));
	connect(addCtrLayer, &QAction::triggered, this, &TimeLineField::onAddCtrlLayer);
	menu.exec(QCursor::pos());
}

void TimeLineField::onAddVideoLayer()
{
	LayerBuilder::instance()->createPlayItemLayer();
}

void TimeLineField::onAddCtrlLayer()
{
	LayerBuilder::instance()->createMarkerItemLayer();
}

void TimeLineField::onTimeSetted(unsigned int time)
{
	qreal pos = m_timeBarView->timeToPositon(time);
	m_timeBarView->centerOn(pos,0);
	m_timeVideoView->centerOn(pos, 0);
	m_timePointerView->centerOn(pos, 0);
	emit m_timeBarView->sigTimebarClicked(time);
}

void TimeLineField::onUpdateRunningTime(unsigned int time)
{
	qreal pos = m_timeBarView->timeToPositon(time);
	m_timeBarView->centerOn(pos, 0);
	m_timeVideoView->centerOn(pos, 0);
	m_timePointerView->centerOn(pos, 0);
	m_timePointerView->onClickTimeBar(time);
	QTime qtime = QTime(0, 0).addMSecs(time);
	m_labelTime->setText(qtime.toString("hh:mm:ss.zzz"));
}

void TimeLineField::onTimeLengthSetted(unsigned int time)
{
	if (time < MIN_TIME_LEN)
	{
		time = MIN_TIME_LEN;
		if (QLabel* label = qobject_cast<QLabel*>(sender()))
		{
			label->setText("00:00:10.00");
		}
	}
	QDomElement d = Observer::data({ "project","track" });
	d.setAttribute("timeLength", time);
	m_timeBarView->setTimeLength(time);
	m_timeVideoView->setTimeLength(time);
	m_timePointerView->setTimeLength(time);
}

void TimeLineField::onMagnetChanged(int before, int after)
{
	Q_UNUSED(before);
	MagnetManager::instance()->setMagnet(after == 2);
}

void TimeLineField::onPlayChanged(int before, int after)
{
	if (after == 2)
	{
		m_playControl->locateTo(m_timePointerView->currentTime());
		m_playControl->play();
	}
	else
	{
		m_playControl->pause();
	}
}

//���ſ���ʱ������
void TimeLineField::locateTo(unsigned int time)
{
	//��ͣ
	m_playControl->pause();
	m_btnPlay->toStatus(1);
	//��λ��ʱ��
	m_playControl->locateTo(time);
}

QDomDocument TimeLineField::onGetDocument()
{
	return Document::instance()->document();
}

void TimeLineField::resizeEvent(QResizeEvent * event)
{
	QAbstractScrollArea::resizeEvent(event);
	QTimer::singleShot(0, [this]() 
	{
		m_timePointerView->setGeometry(mapFromGlobal(m_timeVideoView->mapToGlobal(QPoint())).x(), 10, m_timeVideoView->width(), viewport()->height());
	});
}

bool TimeLineField::eventFilter(QObject *watched, QEvent *event)
{
	if (m_timeVideoView && watched == m_timeVideoView->viewport() && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove))
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)(event);
		// Store some of the event's button-down data.
		QPoint mousePressViewPoint = mouseEvent->pos();
		mousePressViewPoint.setY(20);
		QPointF mousePressScenePoint = m_timeBarView->mapToScene(mousePressViewPoint);
		QPoint mousePressScreenPoint = m_timeBarView->mapToGlobal(mousePressViewPoint);//mouseEvent->globalPos();
		QPointF lastMouseMoveScenePoint = mousePressScenePoint;
		QPoint lastMouseMoveScreenPoint = mousePressScreenPoint;


		QEvent::Type type = ((event->type() == QEvent::MouseButtonPress) ? QEvent::GraphicsSceneMousePress : QEvent::GraphicsSceneMouseMove);
		// Convert and deliver the mouse event to the scene.
		QGraphicsSceneMouseEvent scenemouseEvent(type);
		scenemouseEvent.setWidget(m_timeBarView->viewport());
		scenemouseEvent.setButtonDownScenePos(mouseEvent->button(), mousePressScenePoint);
		scenemouseEvent.setButtonDownScreenPos(mouseEvent->button(), mousePressScreenPoint);
		scenemouseEvent.setScenePos(mousePressScenePoint);
		scenemouseEvent.setScreenPos(mousePressScreenPoint);
		scenemouseEvent.setLastScenePos(lastMouseMoveScenePoint);
		scenemouseEvent.setLastScreenPos(lastMouseMoveScreenPoint);
		scenemouseEvent.setButtons(mouseEvent->buttons());
		scenemouseEvent.setButton(mouseEvent->button());
		scenemouseEvent.setModifiers(mouseEvent->modifiers());
		scenemouseEvent.setSource(mouseEvent->source());
		scenemouseEvent.setFlags(mouseEvent->flags());
		scenemouseEvent.setAccepted(false);

		QApplication::sendEvent(m_timeBarView->scene(), &scenemouseEvent);
	}
	return QObject::eventFilter(watched, event);
}

void TimeLineField::init()
{
	QDomElement d = Observer::data({ "project","track" });
	int timeLen = d.attribute("timeLength").toInt();
	if (timeLen < MIN_TIME_LEN)
	{
		timeLen = MIN_TIME_LEN;
	}
	m_labelTimeLen->setText(QTime::fromMSecsSinceStartOfDay(timeLen).toString("hh:mm:ss.zzz"));
	onTimeLengthSetted(timeLen);
}

ClickTimeEdit::ClickTimeEdit(QWidget* parent /*= nullptr*/) :QLabel(parent)
{
	QHBoxLayout* layout = new QHBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	FocusSignalTimeEdit * timeEdit = new FocusSignalTimeEdit;
	timeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
	timeEdit->setDisplayFormat("hh:mm:ss.zzz");
	m_timeEdit = timeEdit;
	connect(timeEdit, &QTimeEdit::editingFinished, this, &ClickTimeEdit::onEditFinished);
	connect(timeEdit, &FocusSignalTimeEdit::focusOut, this, &ClickTimeEdit::onFocusOut);
	layout->addWidget(m_timeEdit);
	m_timeEdit->hide();
}

ClickTimeEdit::ClickTimeEdit(const QString &text, QWidget *parent /*= nullptr*/) :ClickTimeEdit(parent)
{
	setText(text);
}

void ClickTimeEdit::onEditFinished()
{
	setText(m_timeEdit->text());
	m_timeEdit->hide();
	emit timeSetted(QTime::fromString(text()).msecsSinceStartOfDay());
}

void ClickTimeEdit::onFocusOut()
{
	m_timeEdit->hide();
}

void ClickTimeEdit::mousePressEvent(QMouseEvent *ev)
{
	m_timeEdit->setTime(QTime::fromString(text()));
	m_timeEdit->show();
	m_timeEdit->setFocus();
}

void FocusSignalTimeEdit::focusOutEvent(QFocusEvent *event)
{
	emit focusOut();
	QTimeEdit::focusOutEvent(event);
}
