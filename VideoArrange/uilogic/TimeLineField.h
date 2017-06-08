#pragma once
#include <QAbstractScrollArea>

class QGraphicsScene;
class StatusButton;
class QLabel;
class TimeBarView;
class TimeVideoView;
class TimePointerView;
class LayerView;
class QGraphicsView;
//ͼ��༭
class TimeLineField : public QAbstractScrollArea
{
	Q_OBJECT
public:
	TimeLineField();
	~TimeLineField();
public slots:
	void onClickTimeBar(qreal time);
	void onClickBtnTime();
protected:
	virtual void resizeEvent(QResizeEvent *)Q_DECL_OVERRIDE;
private:
	StatusButton* m_btnPlay;//����
	StatusButton* m_btnMagnet;//����

	TimeBarView* m_timeBarView;
	TimeVideoView* m_timeVideoView;
	TimePointerView* m_timePointerView;

	LayerView* m_leftLayerView;

	QLabel* m_labelTime;
};

