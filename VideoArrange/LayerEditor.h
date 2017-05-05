#pragma once
#include <QAbstractScrollArea>

class QGraphicsScene;
class QPushButton;
class QLabel;
class TimeBar;
//ͼ��༭
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

	QPushButton* m_btnPlay;//����
	QPushButton* m_btnZoomOut;//��С
	QPushButton* m_btnZoomIn;//�Ŵ�

	QLabel* m_labelTime;
	TimeBar* m_timeBar;
	QPushButton* m_btnTime;
};

