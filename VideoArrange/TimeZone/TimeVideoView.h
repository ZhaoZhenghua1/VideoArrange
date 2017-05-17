#pragma once
#include "TimeView.h"
#include "../Document/Observer.h"

class QGraphicsWidget;
class TimeVideoView;
class QGraphicsAnchorLayout;
class TimeVideoLine;
class TimeVideo : public TimeZone , public Observer
{
public:
	TimeVideo(TimeVideoView* view);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect);
protected:
	virtual void init() override;
private:
	TimeVideoView* m_view;
	QGraphicsAnchorLayout* m_anchorLayout;
	QVector<TimeVideoLine*> m_layerLines;
};

class TimeVideoView : public TimeView
{
	Q_OBJECT
public:
	TimeVideoView();
	~TimeVideoView();
protected:
	virtual void paintEvent(QPaintEvent *event)Q_DECL_OVERRIDE;
protected:
	virtual TimeZone* timeZone()Q_DECL_OVERRIDE;
	virtual qreal sceneHeight()Q_DECL_OVERRIDE;
protected:
protected:

private:
	TimeVideo* m_timeVideo = nullptr;
};

