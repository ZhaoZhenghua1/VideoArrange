#include "LayerView.h"
#include "../Document/Document.h"
#include "uilogic/LayerBuilder.h"
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
		painter->fillRect(boundingRect(), QColor(49, 49, 49));
	}
};

LayerView::LayerView()
{
	setFrameShape(QFrame::NoFrame);

	QGraphicsScene* scene = new QGraphicsScene;
	scene->setBackgroundBrush(QColor(49, 49, 49));
	QGraphicsAnchorLayout* lay = new QGraphicsAnchorLayout;
	lay->setSpacing(0);
	lay->setContentsMargins(0, 0, 0, 10);
	LayerBuilder::instance()->setLeftLayout(lay);	
	
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

	qreal height = 0;
	QGraphicsLayout* layout = m_rootWidget->layout();
	//计算视图高度
	if (layout->count() > 0)
		height = layout->itemAt(layout->count() - 1)->geometry().bottom();
	height = (height > event->size().height() ? height : event->size().height()) + 200;
	QRectF rect(QPointF(), QSizeF(event->size().width(), height));
	//-2产生一个隔离边
	m_rootWidget->setGeometry(rect.adjusted(0, 0, -2, 0));
	scene()->setSceneRect(rect);
}

void LayerView::paintEvent(QPaintEvent *event)
{
	QGraphicsView::paintEvent(event);
}

void LayerView::mouseMoveEvent(QMouseEvent *event)
{
	QGraphicsView::mouseMoveEvent(event);
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
