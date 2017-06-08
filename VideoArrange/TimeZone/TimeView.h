#pragma once

#include <QGraphicsView>
#include <QGraphicsWidget>
//时间轴相关
class TimeView;

//时间区域，控制时间刻度
class TimeZone : public QGraphicsWidget
{
public:
	//当前刻度下，时间到像素位置的换算
	qreal timeToPosition(qreal timeMS) const;
	//当前刻度下，像素位置到时间的换算
	qreal positionToTime(qreal pos) const;
	//刻度间的像素间隔
	qreal pixSpace() const{ return m_dPixSpace; }
	//刻度间的时间间隔
	unsigned int timeSpace()const { return m_uiTimeSpace; }
private:
	unsigned int m_iTimeLength = 30*60*1000;//时间长度
	unsigned int m_uiTimeSpace = 1;//时间刻度间隔
	qreal m_dPixSpace = 1;//像素刻度间隔

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
	void onAdjustWidth(int width);
public:
	bool isTimeMinimized();
	bool isTimeMaximized();
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
	//时间视图宽度由视图决定，高度由子类决定
	virtual qreal sceneHeight();
	virtual TimeZone* timeZone() = 0;
private:
	bool m_bIsMinimized = true;
};

