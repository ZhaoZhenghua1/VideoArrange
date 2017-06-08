#include "LayerView.h"
#include "LayerItem.h"
#include "../Document/Document.h"

#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>

#include "LayerTitle.h"
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

static QGraphicsProxyWidget *createItem(const QSizeF &minimum = QSizeF(100.0, 100.0),
	const QSizeF &preferred = QSize(150.0, 100.0),
	const QSizeF &maximum = QSizeF(200.0, 100.0),
	const QString &name = "0")
{
	QGraphicsProxyWidget *w = new QGraphicsProxyWidget;
	w->setWidget(new QLabel(name));
	w->setData(0, name);
	w->setMinimumSize(minimum);
	w->setPreferredSize(preferred);
	w->setMaximumSize(maximum);

	w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	return w;
}
LayerView::LayerView()
{
	setFrameShape(QFrame::NoFrame);

	QGraphicsScene* scene = new QGraphicsScene;

	QSizeF minSize(200, 13);
	QSizeF prefSize(210, 100);
	QSizeF maxSize(300, 150);
	QGraphicsProxyWidget *a = createItem(minSize, prefSize, maxSize, "A");
	QGraphicsProxyWidget *b = createItem(minSize, prefSize, maxSize, "B");
	QGraphicsProxyWidget *c = createItem(minSize, prefSize, maxSize, "C");
	QGraphicsProxyWidget *d = createItem(minSize, prefSize, maxSize, "A");
	QGraphicsProxyWidget *e = createItem(minSize, prefSize, maxSize, "B");
	QGraphicsProxyWidget *f = createItem(minSize, prefSize, maxSize, "C");

	QGraphicsAnchorLayout* lay = new QGraphicsAnchorLayout;
	lay->setSpacing(0);
	lay->setContentsMargins(0, 0, 0, 0);
//	LayerGroup* lg1 = new LayerGroup({ new Layer(a), new Layer(b), new Layer(c) }, lay);
//	LayerGroup* lg2 = new LayerGroup({ new Layer(d), new Layer(e), new Layer(f) }, lay);
	

	scene->setBackgroundBrush(Qt::blue);//QColor(33, 33, 33));
	
	m_rootWidget = new RootWidget;
	
	m_rootWidget->setLayout(lay);

	scene->addItem(m_rootWidget);
	FellowLayer* arr[6] = {};
	for (int i = 0; i < 6; ++i)
	{
		arr[i] = new FellowLayer;
	}
	arr[0]->setWidget(a);
	arr[1]->setWidget(b);
	arr[2]->setWidget(c);
	arr[3]->setWidget(d);
	arr[4]->setWidget(e);
	arr[5]->setWidget(f);

	TitleLayer* title[2];
	title[0] = new TitleLayer;
	title[1] = new TitleLayer;
//	title[0]->addGroupToLayout({ arr[0],arr[1],arr[2] }, lay);
//	title[1]->addGroupToLayout({ arr[3],arr[4],arr[5] }, lay);

	title[0]->addGroupToLayout({ arr[0],arr[1],arr[2] }, lay);
	title[1]->addGroupToLayout({ arr[3],arr[4],arr[5] }, lay);
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

	QGraphicsAnchorLayout* layout = new QGraphicsAnchorLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(1);

	QDomElement d = Observer::data({ "project","track","layerlist" });
	int totop = 0;
	for (QDomElement elem = d.firstChildElement("layer"); !elem.isNull(); elem = elem.nextSiblingElement("layer"))
	{
		LayerItem* item = new LayerItem;
		layout->addAnchor(item, Qt::AnchorLeft, layout, Qt::AnchorLeft);
		layout->addAnchor(item, Qt::AnchorRight, layout, Qt::AnchorRight);
		QGraphicsAnchor* anchor = layout->addAnchor(item, Qt::AnchorTop, layout, Qt::AnchorTop);
		anchor->setSpacing(-totop);
		
		anchor = layout->addAnchor(item, Qt::AnchorBottom, layout, Qt::AnchorTop);
		anchor->setSpacing(-totop +46);
		totop += 100;
//		layout->addItem(item);
	}

	m_rootWidget->setLayout(layout);

	scene()->addItem(m_rootWidget);
}
