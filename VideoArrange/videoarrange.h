#pragma once
#include <QAbstractScrollArea>

class QGraphicsScene;
class QPushButton;
class QLabel;
class TimeBarView;
class TimeVideoView;
class TimePointerView;
class QGraphicsView;
//图层编辑
class VideoArrange : public QAbstractScrollArea
{
	Q_OBJECT
public:
	VideoArrange();
	~VideoArrange();
protected:
	virtual void resizeEvent(QResizeEvent *)Q_DECL_OVERRIDE;
private slots:
	void onScroll();
private:
	QGraphicsScene* m_sceneTopLeft = nullptr;
	QGraphicsScene* m_sceneBottomLeft = nullptr;

	QPushButton* m_btnPlay;//播放
	QPushButton* m_btnZoomOut;//缩小
	QPushButton* m_btnZoomIn;//放大

	TimeBarView* m_timeBarView;
	TimeVideoView* m_timeVideoView;
	TimePointerView* m_timePointerView;
	QGraphicsView* m_pViewEffectRight;

	QLabel* m_labelTime;
	QPushButton* m_btnTime;
};

