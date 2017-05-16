#include "LayerView.h"
#include "LayerItem.h"
#include "../Document/Document.h"

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

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
	scene->addItem(m_rootWidget);

	setScene(scene);

	Document::instance()->addObserver(this);
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

void LayerView::init()
{
	delete m_rootWidget;
	m_rootWidget = new RootWidget;

	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(1);

	QDomElement d = Observer::data({ "project","track","layerlist" });
	for (QDomElement elem = d.firstChildElement("layer"); !elem.isNull(); elem = elem.nextSiblingElement("layer"))
	{
		LayerItem* item = new LayerItem;
		layout->addItem(item);
	}

	m_rootWidget->setLayout(layout);

	scene()->addItem(m_rootWidget);
}
