#include "LayerView.h"
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include "LayerItem.h"

class RootWidget : public QGraphicsWidget
{
public:
	virtual void updateGeometry()Q_DECL_OVERRIDE
	{
		QGraphicsWidget::updateGeometry();
	}
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
	{
		painter->fillRect(boundingRect(), Qt::black);
	}
};
LayerView::LayerView()
{
	QGraphicsScene* scene = new QGraphicsScene;

	scene->setBackgroundBrush(Qt::blue);
	m_rootWidget = new RootWidget;

	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(1);
	for (int i = 0 ; i < 7; ++i)
	{
		LayerItem* item = new LayerItem;
		layout->addItem(item);
	}

 	m_rootWidget->setLayout(layout);

	scene->addItem(m_rootWidget);

	setScene(scene);
}


LayerView::~LayerView()
{
}

void LayerView::resizeEvent(QResizeEvent *event) 
{
	QGraphicsView::resizeEvent(event);
	if (m_rootWidget->rect().height() < viewport()->height())
	{
		scene()->setSceneRect(QRectF(QPointF(0, 0), QSizeF(viewport()->width(), viewport()->height())));
	}
	else
	{
		scene()->setSceneRect(QRectF(QPointF(0, 0), QSizeF(viewport()->width(), m_rootWidget->rect().height())));
	}
}

void LayerView::paintEvent(QPaintEvent *event)
{
	QGraphicsView::paintEvent(event);
}
