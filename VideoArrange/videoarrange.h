#pragma once
#include <QAbstractScrollArea>

class QGraphicsScene;
class QPushButton;
class QLabel;
class TimeBarView;
class TimeVideoView;
class TimePointerView;
class QGraphicsView;
//ͼ��༭
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

	QPushButton* m_btnPlay;//����
	QPushButton* m_btnZoomOut;//��С
	QPushButton* m_btnZoomIn;//�Ŵ�

	TimeBarView* m_timeBarView;
	TimeVideoView* m_timeVideoView;
	TimePointerView* m_timePointerView;
	QGraphicsView* m_pViewEffectRight;

	QLabel* m_labelTime;
	QPushButton* m_btnTime;
};

