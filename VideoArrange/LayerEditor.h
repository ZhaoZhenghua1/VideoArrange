#pragma once
#include <QAbstractScrollArea>

class QGraphicsScene;
class QPushButton;
class QLabel;
class TimeBar;
//图层编辑
class LayerEditor : public QAbstractScrollArea
{
public:
	LayerEditor();
	~LayerEditor();
private:
	QGraphicsScene* m_sceneTopLeft = nullptr;
	QGraphicsScene* m_sceneTopRight = nullptr;
	QGraphicsScene* m_sceneBottomLeft = nullptr;
	QGraphicsScene* m_sceneBottomRight = nullptr;

	QPushButton* m_btnPlay;//播放
	QPushButton* m_btnZoomOut;//缩小
	QPushButton* m_btnZoomIn;//放大

	QLabel* m_labelTime;
	TimeBar* m_timeBar;
	QPushButton* m_btnTime;
};

