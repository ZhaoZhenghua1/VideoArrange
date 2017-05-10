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

void TimeView::setTimeLength(unsigned int time)
{
	timeZone()->m_iTimeLength = time;
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
	m_bIsMinimized = false;
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
		m_bIsMinimized = true;
	}
	//��СΪԭ����1/2
	else
	{
		sceneRec.setWidth(sceneRec.width() / 2);
	}
	setSceneRect(sceneRec);
}

#include <QApplication>
void TimeView::update()
{
// 	QEvent event(QEvent::UpdateRequest);
// 	QApplication::sendEvent(this, &event);
//	QGraphicsView::update();
}

bool TimeView::isTimeMinimized()
{
	return m_bIsMinimized;
}

bool TimeView::isTimeMaximized()
{
	return (int)timeZone()->m_uiTimeSpace <= 200;
}

void TimeView::setSceneRect(const QRectF& rect)
{
	scene()->setSceneRect(rect);
	timeZone()->setRect(rect);

	std::tuple<int, qreal> ret = timeBarRuleStrategy(timeZone()->m_iTimeLength, scene()->width());
	timeZone()->m_uiTimeSpace = std::get<TIMESPACE>(ret);
	timeZone()->m_dPixSpace = std::get<PIXSPACE>(ret);
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	//����������ͼʱ
	if (sceneRect().width() < width())
	{
		m_bIsMinimized = true;
	}
	if (sceneRect().height() < height())
	{
		scene()->setSceneRect(sceneRect().adjusted(0,0,event->size().width(), event->size().height()));//QRectF(0, 0, event->size().width(), event->size().height()));
		timeZone()->setRect(timeZone()->rect().adjusted(0, 0, 0, height()));
	}
	//��������ͼһ����Сʱ������ͼ��С
	if (isTimeMinimized())
	{
		setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
	}
	QGraphicsView::resizeEvent(event);
}
