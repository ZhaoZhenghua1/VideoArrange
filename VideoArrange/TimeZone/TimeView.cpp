#include "TimeView.h"
#include <QResizeEvent>
#include <QDebug>

TimeView::TimeView()
{
	QGraphicsScene* scene = new QGraphicsScene;
	setScene(scene);
}


TimeView::~TimeView()
{
}

enum
{
	TIMESPACE,
	PIXSPACE
};
//根据时间长度和时间轴长度计算时间间距和像素间距
//时间间距为 1/2/5 * 10的幂次方
std::tuple<int, qreal> timeBarRuleStrategy(const unsigned int timeLength, qreal distance)
{
	//时间轴绘制策略
	qreal timeSpace = timeLength * 80 / distance;
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

bool TimeView::isTimeMinimized()
{
	return m_bIsMinimized;
}

//时间间隔为200ms时为最大化
bool TimeView::isTimeMaximized()
{
	return (int)timeZone()->m_uiTimeSpace <= 200;
}

void TimeView::setSceneRect(const QRectF& rect)
{
	scene()->setSceneRect(rect);
	timeZone()->setGeometry(rect);

	std::tuple<int, qreal> ret = timeBarRuleStrategy(timeZone()->m_iTimeLength, scene()->width());
	timeZone()->m_uiTimeSpace = std::get<TIMESPACE>(ret);
	timeZone()->m_dPixSpace = std::get<PIXSPACE>(ret);
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	//场景超出视图时
	if (sceneRect().width() < width())
	{
		m_bIsMinimized = true;
	}

	//场景和视图一样大小时，随视图大小
	if (isTimeMinimized())
	{
		setSceneRect(QRectF(0, 0, event->size().width(), sceneHeight()));
	}
	QGraphicsView::resizeEvent(event);
}

qreal TimeView::sceneHeight()
{
	return viewport()->height();
}

