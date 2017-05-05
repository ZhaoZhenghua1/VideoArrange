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
//����ʱ�䳤�Ⱥ�ʱ���᳤�ȼ���ʱ��������ؼ��
//ʱ����Ϊ 1/2/5 * 10���ݴη�
std::tuple<int, qreal> timeBarRuleStrategy(const unsigned int timeLength, qreal distance)
{
	//ʱ������Ʋ���
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

//�Ŵ�
void TimeView::zoomIn()
{
	//��С���200ms
	if (isTimeMaximized())
	{
		return;
	}
	QRectF sceneRec = sceneRect();
	sceneRec.setWidth(sceneRec.width() *2);
	setSceneRect(sceneRec);
}
//��С
void TimeView::zoomOut()
{
	//�Ѿ���С��
	if (isTimeMinimized())
	{
		return;
	}

	QRectF sceneRec = sceneRect();
	//��������СΪԭ����1/2������СΪ��ͼ��С
	if (sceneRec.width() / 2 <= viewport()->width())
	{
		sceneRec.setWidth(viewport()->width());
	}
	//��СΪԭ����1/2
	else
	{
		sceneRec.setWidth(sceneRec.width() / 2);
	}
	setSceneRect(sceneRec);
}

bool TimeView::isTimeMinimized()
{
	//��������ʱ����
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
	//��������ͼһ����Сʱ������ͼ��С
	if (isTimeMinimized())
	{
		setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
	}

}

TimeZone::TimeZone(TimeView* timeView) : m_timeView(timeView)
{

}
