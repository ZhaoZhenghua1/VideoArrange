#pragma once
#include "TimeView.h"

class TimeVideoView;
class TimeVideo : public TimeZone
{
public:
	TimeVideo(TimeVideoView* view);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
private:
	TimeVideoView* m_view;
};

class TimeVideoView : public TimeView
{
public:
	TimeVideoView();
	~TimeVideoView();
protected:
	virtual TimeZone* timeZone()Q_DECL_OVERRIDE;
private:
	TimeVideo* m_timeVideo = nullptr;
};

