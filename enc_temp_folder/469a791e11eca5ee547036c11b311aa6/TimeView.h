#pragma once

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
//时间轴相关
class TimeView;
class TimeZone : public QGraphicsRectItem
{
public:
	TimeZone(TimeView* timeView);
protected:
	unsigned int m_iTimeLength;//时间长度
	qreal m_dTimeSpace = 1;//时间间隔
	qreal m_dPixSpace = 1;//像素间隔

	TimeView* m_timeView;

	friend class TimeView;
};

class TimeView : public QGraphicsView
{
public:
	TimeView();
	~TimeView();
	void setTimeZone(TimeZone* timeZone);
	//设置时长
	void setTimeLength(unsigned int time);
	void zoomIn();//放大
	void zoomOut();//缩小
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
protected:
	TimeZone* m_timeZone = nullptr;
};

