#include "TimeBar.h"
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <math.h>
#include <QPainter>
#include <QTime>
#include <QGraphicsSceneMouseEvent>
#include <qdebug>

class Bar : public QGraphicsRectItem
{
public:
	Bar(TimeBar* view):m_view(view)
	{
		setBrush(Qt::darkGray);
		//test
		m_iTimeLength = 30 * 60 * 1000;
	}
	//设置时长
	void setTimeLength(unsigned int time) { m_iTimeLength = time; }
	//场景宽度改变时，更新时间间隔和像素间隔
	void sceneWidthChanged();

	unsigned int timeLength() { return m_iTimeLength; }
	qreal timeSpace() { return m_dTimeSpace; }
	qreal pixSpace() { return m_dPixSpace; }
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */) Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
public:
	unsigned int m_iTimeLength;

	qreal m_dTimeSpace = 1;//时间间隔
	qreal m_dPixSpace = 1;//像素间隔

	TimeBar* m_view = nullptr;
};

TimeBar::TimeBar()
{
	QGraphicsScene* scene = new QGraphicsScene;
	setScene(scene);

	m_rectBar = new Bar(this);
	scene->addItem(m_rectBar);
}


TimeBar::~TimeBar()
{
}

void TimeBar::setTimeLength(unsigned int time)
{
	m_rectBar->setTimeLength(time);
}

void TimeBar::timebarClicked(const QPointF& scenePos)
{
	
}

void TimeBar::resizeEvent(QResizeEvent *event)
{
	int sw = scene()->width();
	int vw = viewport()->width();
	int vvw = width();
	int temp = viewport()->width() / m_rectBar->m_dPixSpace*m_rectBar->m_dTimeSpace;
	if ((int)(viewport()->width() / m_rectBar->m_dPixSpace*m_rectBar->m_dTimeSpace) == m_rectBar->m_iTimeLength)
	{
		setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
	}
	scene()->setSceneRect(0, 0, event->size().width(), event->size().height());
	
	
	m_rectBar->sceneWidthChanged();
	QGraphicsView::resizeEvent(event);
}

//时间间距为 1/2/5 * 10的幂次方
void Bar::sceneWidthChanged()
{
	setRect(scene()->sceneRect());
	qreal w = rect().width();

	//时间轴绘制策略
	qreal timeSpace = m_iTimeLength * 80 / w;
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

	qreal pixSpace = w * timeSpace / m_iTimeLength;
	
	m_dTimeSpace = timeSpace;
	m_dPixSpace = pixSpace;
}

void Bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	QGraphicsRectItem::paint(painter, option, widget);
	qreal w = rect().width();

	for (qreal x = 0; x <= w; x += m_dPixSpace)
	{
		if (m_dTimeSpace < 1000)
		{
			painter->drawText(x,20,QTime(0,0).addMSecs(x/ m_dPixSpace*m_dTimeSpace).toString("hh:mm:ss.zzz"));
		}
		else
		{
			painter->drawText(x, 20, QTime(0, 0).addMSecs(x / m_dPixSpace*m_dTimeSpace).toString("hh:mm:ss"));
		}
	}
}

void Bar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);
	m_view->timebarClicked(event->scenePos());
}

// 
// 	qreal w = scene()->width();
// 	
// 	qreal timeSpace = m_iTimeLength * 80 / w;
// 	unsigned int e = pow(10, (int)log10(timeSpace));
// 	timeSpace /= e;
// 	if (timeSpace >1 && timeSpace <= 2)
// 	{
// 		timeSpace = 2;
// 	}
// 	else if (timeSpace > 2 && timeSpace <= 5)
// 	{
// 		timeSpace = 5;
// 	}
// 	else
// 	{
// 		timeSpace = 10;
// 	}
// 	timeSpace *= e;
// 
// 	qreal pixSpace = w * timeSpace / m_iTimeLength;
// 
// 	qDebug() << timeSpace;
// 	qDebug() << pixSpace;
//qDebug() << space;