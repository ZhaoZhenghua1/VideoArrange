#include "TimeView.h"
#include <QResizeEvent>


TimeView::TimeView()
{
	QGraphicsScene* scene = new QGraphicsScene;
	setScene(scene);

	setTimeZone(new TimeZone(this));
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

void TimeView::setTimeZone(TimeZone* timeZone)
{
	delete m_timeZone;
	m_timeZone = timeZone;
	timeZone->m_timeView = this;
	scene()->addItem(timeZone);
	m_timeZone->setPos(0,0);
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
	//具有最大的时间间距
	return std::get<TIMESPACE>(timeBarRuleStrategy(m_timeZone->m_iTimeLength, viewport()->width())) == (int)m_timeZone->m_dTimeSpace;
}

bool TimeView::isTimeMaximized()
{
	return (int)m_timeZone->m_dTimeSpace <= 200;
}

void TimeView::setSceneRect(const QRectF& rect)
{
	scene()->setSceneRect(rect);
	m_timeZone->setRect(rect);
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	//场景和视图一样大小时，随视图大小
	if (isTimeMinimized())
	{
		setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
	}

}

TimeZone::TimeZone(TimeView* timeView) : m_timeView(timeView)
{

}
