#pragma once

#include "TimeView.h"

class TimeBarView;
class TimeBar : public TimeZone
{
public:
	TimeBar(TimeBarView* view) :m_view(view)
	{
		setBrush(Qt::darkGray);
	}
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
public:
	
	TimeBarView* m_view = nullptr;
};

// ±º‰Ãı
class TimeBarView : public TimeView
{
	Q_OBJECT
public:
	TimeBarView();
	~TimeBarView();
signals:
	void sigTimebarClicked(qreal xPos);
protected:
	virtual TimeZone* timeZone()Q_DECL_OVERRIDE;
private:
	TimeBar* m_bar = nullptr;
};

