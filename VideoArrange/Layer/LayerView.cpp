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
		//painter->fillRect(boundingRect(), Qt::black);
	}
};
LayerView::LayerView()
{
	QGraphicsScene* scene = new QGraphicsScene;

	scene->setBackgroundBrush(Qt::red);
	m_rootWidget = new RootWidget;
	m_rootWidget->setPreferredSize(100, 100);

	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(1);
	for (int i = 0 ; i < 7; ++i)
	{
		LayerItem* item = new LayerItem;
		connect(item, &LayerItem::sigLayerUnfold, this, &LayerView::onUnfoldRelayout);
		layout->addItem(item);
	}

// 
 	m_rootWidget->setLayout(layout);
// 
// 	layout->setContentsMargins(0, 0, 0, 0);
// 	layout->setSpacing(1);

	scene->addItem(m_rootWidget);
//	m_rootWidget->show();
	
	setScene(scene);
	scene->setBackgroundBrush(Qt::red);
	QRectF sr = sceneRect();
//	scene()->setSceneRect(0,0,116,100);
	
}


LayerView::~LayerView()
{
}

void LayerView::resizeEvent(QResizeEvent *event) 
{
	QGraphicsView::resizeEvent(event);
// 	if (sceneRect().height() < height())
// 	{
// 		scene()->setSceneRect(sceneRect().adjusted(0, 0, event->size().width()+200, event->size().height()+200));//QRectF(0, 0, event->size().width(), event->size().height()));
// 		m_rootWidget->setGeometry(m_rootWidget->geometry().adjusted(0, 0, width(), height()));
// 	}
// 	m_rootWidget->setGeometry(QRectF(m_rootWidget->pos(), event->size()));
 	QPointF pos = m_rootWidget->pos();
 	QRectF rect = m_rootWidget->rect();
 	bool b = m_rootWidget->isVisible();
 	QRectF sr = sceneRect();
	scene()->setSceneRect(QRectF(QPointF(0,0), QSizeF(112,1111)));
	static bool added = false;
	if (!added)
	{
		scene()->addItem(m_rootWidget);
		added = true;
	}
}

void LayerView::onUnfoldRelayout()
{
	//m_rootWidget->updateGeometry();
	m_rootWidget->layout()->invalidate();
}
