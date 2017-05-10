#pragma once
#include "TimeView.h"

class TimePointerView;
class TimePointer : public TimeZone
{
public:
	TimePointer(TimePointerView* view);
	void click(const unsigned int timepos);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
private:
	unsigned int m_uiClickTimePos = 0;
	TimePointerView* m_view;
};

//跨越窗口上下的时间指针
class TimePointerView : public TimeView
{
	Q_OBJECT
public:
	TimePointerView();
	~TimePointerView();
public slots:
void onClickTimeBar(const unsigned int timepos);
protected:
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
	virtual TimeZone* timeZone() Q_DECL_OVERRIDE;
private:
	TimePointer* m_timePointer;
};

