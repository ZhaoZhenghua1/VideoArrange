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
//ͼ��༭
class TimeLineField : public QAbstractScrollArea, public Observer
{
	Q_OBJECT
public:
	TimeLineField();
	~TimeLineField();
public slots:
	void onClickTimeBar(qreal time);
	void onClickBtnTime();
	void onAddLayer();
	void onAddVideoLayer();
	void onAddCtrlLayer();
	void onTimeSetted(unsigned int time);
	void onTimeLengthSetted(unsigned int time);
	void onMagnetChanged(int before, int after);
	void onPlayChanged(int before, int after);
protected:
	virtual void resizeEvent(QResizeEvent *)Q_DECL_OVERRIDE;
	virtual void init()override;
private:
	StatusButton* m_btnPlay;//����
	StatusButton* m_btnMagnet;//����

	TimeBarView* m_timeBarView;
	TimeVideoView* m_timeVideoView;
	TimePointerView* m_timePointerView;

	LayerView* m_leftLayerView;

	QLabel* m_labelTime = nullptr;
	QLabel* m_labelTimeLen = nullptr;

	MediaPlayControl * m_playControl = nullptr;
};

//�����༭
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

//�����༭�����ؼ���ʧȥ��������ؿؼ�
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