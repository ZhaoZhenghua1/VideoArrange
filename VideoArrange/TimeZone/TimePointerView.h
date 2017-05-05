#pragma once
#include "TimeView.h"

class TimePointer : public TimeZone
{
public:
	void click(qreal xpos);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
private:
	qreal m_qrClickPos = 0;
};

//跨越窗口上下的时间指针
class TimePointerView : public TimeView
{
	Q_OBJECT
public:
	TimePointerView();
	~TimePointerView();
public slots:
void onClickTimeBar(const qreal xpos);
protected:
	virtual TimeZone* timeZone() Q_DECL_OVERRIDE;
private:
	TimePointer* m_timePointer;
};

