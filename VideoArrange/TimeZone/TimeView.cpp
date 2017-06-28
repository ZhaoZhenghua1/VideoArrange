#include "TimeView.h"
#include <QResizeEvent>
#include <QDebug>

namespace
{
	enum
	{
		TIMESPACE,
		PIXSPACE
	};
}

TimeView::TimeView()
{
	QGraphicsScene* scene = new QGraphicsScene;
	setScene(scene);
	setFrameShape(QFrame::NoFrame);
}


TimeView::~TimeView()
{
}

//根据时间长度和时间轴长度计算时间间距和像素间距
//时间间距为 1/2/5 * 10的幂次方
std::tuple<int, qreal> timeBarRuleStrategy(const unsigned int timeLength, qreal distance)
{
	//时间轴绘制策略
	qreal timeSpace = timeLength * 100.0 / distance;
	unsigned int e = pow(10, (int)log10(timeSpace));
	timeSpace /= e;
	if (timeSpace > 1 && timeSpace <= 2)
	{
		timeSpace = 2;
	}
	else if (timeSpace > 2 && timeSpace <= 5)
	{
		timeSpace = 5;
	}
	else
	{
		timeSpace = 10;
	}
	timeSpace *= e;

	qreal pixSpace = distance * timeSpace / timeLength;

	return{ (int)timeSpace , pixSpace };
}

void TimeView::setTimeLength(unsigned int time)
{
	timeZone()->m_iTimeLength = time;

	std::tuple<int, qreal> ret = timeBarRuleStrategy(timeZone()->m_iTimeLength, sceneRect().width());
	timeZone()->m_uiTimeSpace = std::get<TIMESPACE>(ret);
	timeZone()->m_dPixSpace = std::get<PIXSPACE>(ret);

	update();
}

//放大
void TimeView::zoomIn()
{
	//最小间距200ms
	if (isTimeMaximized())
	{
		return;
	}
	QRectF sceneRec = sceneRect();
	//场景宽度加倍
	sceneRec.setWidth(sceneRec.width() *2);
	setSceneRect(sceneRec);
	m_bIsMinimized = false;
}
//缩小
void TimeView::zoomOut()
{
	//已经最小化
	if (isTimeMinimized())
	{
		return;
	}

	QRectF sceneRec = sceneRect();
	//不能再缩小为原来的1/2，则缩小为视图大小
	if (sceneRec.width() / 2 <= viewport()->width())
	{
		sceneRec.setWidth(viewport()->width());
		m_bIsMinimized = true;
	}
	//缩小为原来的1/2
	else
	{
		sceneRec.setWidth(sceneRec.width() / 2);
	}
	setSceneRect(sceneRec);
}

void TimeView::onAdjustWidth(int width)
{
	//已经最小化
	if (width < 0 && isTimeMinimized())
	{
		return;
	}
	if (width > 0 && isTimeMaximized())
	{
		return;
	}
	QRectF sceneRec = sceneRect();
	//不能再缩小为原来的1/2，则缩小为视图大小
	if (sceneRec.width() + width <= viewport()->width())
	{
		sceneRec.setWidth(viewport()->width());
		m_bIsMinimized = true;
	}
	//缩小为原来的1/2
	else
	{
		sceneRec.setWidth(sceneRec.width() + width);
		m_bIsMinimized = false;
	}
	setSceneRect(sceneRec);
}

bool TimeView::isTimeMinimized()
{
	return m_bIsMinimized;
}

//时间间隔为200ms时为最大化
bool TimeView::isTimeMaximized()
{
	return timeZone()->m_uiTimeSpace <= 100;
}

void TimeView::setSceneRect(const QRectF& rect)
{
	//场景宽度改变后，修改时间轴策略
	std::tuple<int, qreal> ret = timeBarRuleStrategy(timeZone()->m_iTimeLength, rect.width());
	timeZone()->m_uiTimeSpace = std::get<TIMESPACE>(ret);
	timeZone()->m_dPixSpace = std::get<PIXSPACE>(ret);

	scene()->setSceneRect(rect);
	timeZone()->setGeometry(rect);
	update();
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	//场景超出视图时
	if (sceneRect().width() < width())
	{
		m_bIsMinimized = true;
	}

	//场景和视图一样大小时，随视图大小
	if (event->oldSize().width() != event->size().width() && isTimeMinimized())
	{
		setSceneRect(QRectF(0, 0, event->size().width(), sceneHeight()));
	}

	if (event->oldSize().height() != event->size().height())
	{
		setSceneRect(QRectF(0, 0, sceneRect().width(), sceneHeight()));
	}

	QGraphicsView::resizeEvent(event);
}

qreal TimeView::sceneHeight()
{
	return viewport()->height();
}

qreal TimeZone::timeToPosition(qreal timeMS) const
{
	return timeMS * 1.0 / m_uiTimeSpace * m_dPixSpace;
}

qreal TimeZone::positionToTime(qreal pos) const
{
	return pos / m_dPixSpace * m_uiTimeSpace;
}
