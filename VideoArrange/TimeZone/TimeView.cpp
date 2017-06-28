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

//����ʱ�䳤�Ⱥ�ʱ���᳤�ȼ���ʱ��������ؼ��
//ʱ����Ϊ 1/2/5 * 10���ݴη�
std::tuple<int, qreal> timeBarRuleStrategy(const unsigned int timeLength, qreal distance)
{
	//ʱ������Ʋ���
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

//�Ŵ�
void TimeView::zoomIn()
{
	//��С���200ms
	if (isTimeMaximized())
	{
		return;
	}
	QRectF sceneRec = sceneRect();
	//������ȼӱ�
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

void TimeView::onAdjustWidth(int width)
{
	//�Ѿ���С��
	if (width < 0 && isTimeMinimized())
	{
		return;
	}
	if (width > 0 && isTimeMaximized())
	{
		return;
	}
	QRectF sceneRec = sceneRect();
	//��������СΪԭ����1/2������СΪ��ͼ��С
	if (sceneRec.width() + width <= viewport()->width())
	{
		sceneRec.setWidth(viewport()->width());
		m_bIsMinimized = true;
	}
	//��СΪԭ����1/2
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

//ʱ����Ϊ200msʱΪ���
bool TimeView::isTimeMaximized()
{
	return timeZone()->m_uiTimeSpace <= 100;
}

void TimeView::setSceneRect(const QRectF& rect)
{
	//������ȸı���޸�ʱ�������
	std::tuple<int, qreal> ret = timeBarRuleStrategy(timeZone()->m_iTimeLength, rect.width());
	timeZone()->m_uiTimeSpace = std::get<TIMESPACE>(ret);
	timeZone()->m_dPixSpace = std::get<PIXSPACE>(ret);

	scene()->setSceneRect(rect);
	timeZone()->setGeometry(rect);
	update();
}

void TimeView::resizeEvent(QResizeEvent *event)
{
	//����������ͼʱ
	if (sceneRect().width() < width())
	{
		m_bIsMinimized = true;
	}

	//��������ͼһ����Сʱ������ͼ��С
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
