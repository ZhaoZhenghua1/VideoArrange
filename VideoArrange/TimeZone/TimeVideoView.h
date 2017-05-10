#pragma once
#include "TimeView.h"
class QGraphicsWidget;
class TimeVideoView;
class QGraphicsAnchorLayout;
class TimeVideo : public TimeZone
{
public:
	TimeVideo(TimeVideoView* view);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual void setGeometry(const QRectF &rect);
private:
	TimeVideoView* m_view;
	QGraphicsAnchorLayout* m_anchorLayout;
};

class TimeVideoView : public TimeView
{
	Q_OBJECT
public:
	TimeVideoView();
	~TimeVideoView();
protected:
	virtual TimeZone* timeZone()Q_DECL_OVERRIDE;
protected:
protected:
// 	virtual void dragEnterEvent(QDragEnterEvent *event) override;
// 	virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
// 	virtual void dragMoveEvent(QDragMoveEvent *event) override;
// 	virtual void dropEvent(QDropEvent *event) override;
private:
	TimeVideo* m_timeVideo = nullptr;
};

