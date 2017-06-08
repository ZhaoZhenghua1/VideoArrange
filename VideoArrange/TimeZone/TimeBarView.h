#pragma once

#include "TimeView.h"

class TimeBarView;
class TimeBar : public TimeZone
{
public:
	TimeBar(TimeBarView* view) :m_view(view)
	{
//		setBrush(Qt::darkGray);
	}
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)Q_DECL_OVERRIDE;
public:
private:
	//精度范围内取整
	qreal timeToRound(qreal t);
private:
	TimeBarView* m_view = nullptr;
};

//时间条
class TimeBarView : public TimeView
{
	Q_OBJECT
public:
	TimeBarView();
	~TimeBarView();
signals:
	void sigTimebarClicked(qreal timePos);
protected:
	virtual TimeZone* timeZone()Q_DECL_OVERRIDE;
private:
	TimeBar* m_bar = nullptr;
};

