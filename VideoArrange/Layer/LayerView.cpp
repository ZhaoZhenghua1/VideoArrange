#include "LayerView.h"
#include "LayerItem.h"
#include "../Document/Document.h"
#include "uilogic/LayerFactory.h"
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>

#include "LeftHandleLayer.h"
#include "Layer.h"
#include <QGraphicsProxyWidget>
#include <QLabel>

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
	setFrameShape(QFrame::NoFrame);

	QGraphicsScene* scene = new QGraphicsScene;

	QGraphicsAnchorLayout* lay = new QGraphicsAnchorLayout;
	lay->setSpacing(0);
	lay->setContentsMargins(0, 0, 0, 0);
	LayerFactory::instance()->setLeftLayout(lay);	

	scene->setBackgroundBrush(Qt::blue);//QColor(33, 33, 33));
	
	m_rootWidget = new RootWidget;

	m_rootWidget->setLayout(lay);

	scene->addItem(m_rootWidget);
	setScene(scene);
//	Document::instance()->addObserver(this);
}


LayerView::~LayerView()
{
}

void LayerView::resizeEvent(QResizeEvent *event)
{
	QGraphicsView::resizeEvent(event);
	m_rootWidget->setGeometry(QRectF( QPointF(), event->size()));
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
// 	delete m_rootWidget;
// 	m_rootWidget = new RootWidget;
// 
// 	QGraphicsAnchorLayout* layout = new QGraphicsAnchorLayout();
// 	layout->setContentsMargins(0, 0, 0, 0);
// 	layout->setSpacing(1);
// 
// 	QDomElement d = Observer::data({ "project","track","layerlist" });
// 	int totop = 0;
// 	for (QDomElement elem = d.firstChildElement("layer"); !elem.isNull(); elem = elem.nextSiblingElement("layer"))
// 	{
// 		LayerItem* item = new LayerItem;
// 		layout->addAnchor(item, Qt::AnchorLeft, layout, Qt::AnchorLeft);
// 		layout->addAnchor(item, Qt::AnchorRight, layout, Qt::AnchorRight);
// 		QGraphicsAnchor* anchor = layout->addAnchor(item, Qt::AnchorTop, layout, Qt::AnchorTop);
// 		anchor->setSpacing(-totop);
// 		
// 		anchor = layout->addAnchor(item, Qt::AnchorBottom, layout, Qt::AnchorTop);
// 		anchor->setSpacing(-totop +46);
// 		totop += 100;
// //		layout->addItem(item);
// 	}
// 
// 	m_rootWidget->setLayout(layout);
// 
// 	scene()->addItem(m_rootWidget);
}
