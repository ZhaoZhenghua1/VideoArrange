#pragma once

#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
//时间轴相关
class TimeView;
class TimeZone : public QGraphicsRectItem
{
protected:
	//virtual TimeView* timeView() = 0;
protected:
	unsigned int m_iTimeLength = 30*60*1000;//时间长度
	qreal m_dTimeSpace = 1;//时间间隔
	qreal m_dPixSpace = 1;//像素间隔

	friend class TimeView;
};

class TimeView : public QGraphicsView
{
	Q_OBJECT
public:
	TimeView();
	~TimeView();
	//设置时长
	void setTimeLength(unsigned int time);
public slots:
	void zoomIn();//放大
	void zoomOut();//缩小
	void update();
public:
	bool isTimeMinimized();
	bool isTimeMaximized();
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;

	virtual TimeZone* timeZone() = 0;
private:
	bool m_bIsMinimized = true;
};

