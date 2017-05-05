#pragma once

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
//ʱ�������
class TimeView;
class TimeZone : public QGraphicsRectItem
{
public:
	TimeZone(TimeView* timeView);
protected:
	unsigned int m_iTimeLength;//ʱ�䳤��
	qreal m_dTimeSpace = 1;//ʱ����
	qreal m_dPixSpace = 1;//���ؼ��

	TimeView* m_timeView;

	friend class TimeView;
};

class TimeView : public QGraphicsView
{
public:
	TimeView();
	~TimeView();
	void setTimeZone(TimeZone* timeZone);
	//����ʱ��
	void setTimeLength(unsigned int time);
	void zoomIn();//�Ŵ�
	void zoomOut();//��С
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
protected:
	TimeZone* m_timeZone = nullptr;
};

