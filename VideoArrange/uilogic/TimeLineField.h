#pragma once
#include <QAbstractScrollArea>
#include <QLabel>
#include <QTimeEdit>
#include "Document/Observer.h"
#include "..\MediaPlayControl\mediaplaycontrol.h"

class QGraphicsScene;
class StatusButton;
class QLabel;
class TimeBarView;
class TimeVideoView;
class TimePointerView;
class LayerView;
class QGraphicsView;
class QTimeEdit;
//图层编辑
class TimeLineField : public QAbstractScrollArea, public Observer
{
	Q_OBJECT
public:
	TimeLineField();
	~TimeLineField();
public slots:
	void onClickTimeBar(qreal time);
	void onAddLayer();
	void onAddVideoLayer();
	void onAddCtrlLayer();
	void onTimeSetted(unsigned int time);
	void onTimeLengthSetted(unsigned int time);
	void onMagnetChanged(int before, int after);
	void onPlayChanged(int before, int after);
	//播放控制时间同步
	void onUpdateRunningTime(unsigned int time);
	//播放控制时间设置
	void locateTo(unsigned int time);
	QDomDocument onGetDocument();
protected:
	virtual void resizeEvent(QResizeEvent *)Q_DECL_OVERRIDE;
	virtual bool eventFilter(QObject *watched, QEvent *event)override;
	virtual void init()override;
private:
	StatusButton* m_btnPlay = nullptr;//播放
	StatusButton* m_btnMagnet = nullptr;//吸附

	TimeBarView* m_timeBarView = nullptr;
	TimeVideoView* m_timeVideoView = nullptr;
	TimePointerView* m_timePointerView = nullptr;

	LayerView* m_leftLayerView = nullptr;

	QLabel* m_labelTime = nullptr;
	QLabel* m_labelTimeLen = nullptr;

	MediaPlayControl * m_playControl = nullptr;
};

//点击后编辑
class ClickTimeEdit : public QLabel
{
	Q_OBJECT
public:
	ClickTimeEdit(QWidget* parent = nullptr);
	ClickTimeEdit(const QString &text, QWidget *parent = nullptr);
signals:
	void timeSetted(unsigned int timeMiniSec);
	public slots :
		void onEditFinished();
		void onFocusOut();
protected:
	virtual void mousePressEvent(QMouseEvent *ev);
private:
	QTimeEdit* m_timeEdit = nullptr;
};

//点击后编辑辅助控件，失去焦点后隐藏控件
class FocusSignalTimeEdit : public QTimeEdit
{
	Q_OBJECT
public:
	using QTimeEdit::QTimeEdit;
signals:
	void focusOut();
protected:
	virtual void focusOutEvent(QFocusEvent *event)override;
};