#pragma once

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
//ʱ�������
class TimeView;
class TimeZone : public QGraphicsWidget
{
public:
	qreal timeToPosition(unsigned int timeMS);
	unsigned int positionToTime(qreal pos);
protected:
	//virtual TimeView* timeView() = 0;
protected:
	unsigned int m_iTimeLength = 30*60*1000;//ʱ�䳤��
	unsigned int m_uiTimeSpace = 1;//ʱ����
	qreal m_dPixSpace = 1;//���ؼ��

	friend class TimeView;
};

class TimeView : public QGraphicsView
{
	Q_OBJECT
public:
	TimeView();
	~TimeView();
	//����ʱ��
	void setTimeLength(unsigned int time);
public slots:
	void zoomIn();//�Ŵ�
	void zoomOut();//��С
public:
	bool isTimeMinimized();
	bool isTimeMaximized();
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
	//ʱ����ͼ�������ͼ�������߶����������
	virtual qreal sceneHeight();
	virtual TimeZone* timeZone() = 0;
private:
	bool m_bIsMinimized = true;
};

