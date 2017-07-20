#include "LeftHandleLayer.h"
#include "../Controls/StatusButton.h"
#include "Controls/EffectValueEditor.h"
#include "Controls/GraphicsButton.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsAnchorLayout>
#include <QLineEdit>
#include <QDomElement>
#include <QTimer>
enum EStatus
{
	eOff, eOn
};

//编辑时才创建，显示时不创建控件
template<class T>
class ClickProxyWidget : public QGraphicsProxyWidget
{
public:
	ClickProxyWidget(int width, QGraphicsItem* parent = nullptr) :QGraphicsProxyWidget(parent)
	{
		m_widget = new T;
		setAcceptHoverEvents(true);
		setGeometry(QRectF(0, 0, width, 20));
	}
	T* proxyWidget() { return m_widget; }

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		if (QGraphicsProxyWidget::widget())
		{
			QGraphicsProxyWidget::paint(painter, option, widget);
		}
		else
		{
			//	painter->fillRect(rect(), Qt::yellow);
			painter->setPen(QPen(QColor(45, 140, 235)));
			painter->drawText(QPointF(0, 13), m_widget->text());
		}
	}
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::PointingHandCursor);
		QGraphicsProxyWidget::hoverEnterEvent(event);
	}
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::ArrowCursor);
		QGraphicsProxyWidget::hoverLeaveEvent(event);
	}
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		QGraphicsProxyWidget::mousePressEvent(event);
		setWidget(m_widget);
		QGraphicsProxyWidget::setPos(m_pos);
	}
	virtual void focusOutEvent(QFocusEvent *event)
	{
		setWidget(nullptr);
		QGraphicsProxyWidget::setPos(m_pos);
		setAcceptHoverEvents(true);
		QGraphicsProxyWidget::focusOutEvent(event);
	}
	void setPos(qreal x, qreal y)
	{
		m_pos = QPointF(x, y);
		QGraphicsProxyWidget::setPos(m_pos);
	}
private:
	T* m_widget = nullptr;
	QPointF m_pos;
};

LeftMediaLeader::LeftMediaLeader()
{
	GraphicsStatusButton* pS = new GraphicsStatusButton(this);
	pS->setPreferredSize(33, 20);
	MediaHelper* pHelper = new MediaHelper(this);
	connect(pS, &GraphicsStatusButton::statusChanged, pHelper, &MediaHelper::onStatusChanged);
	pS->setPos(0, 2);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unfold_normal.png", ":/unfold_hover.png");
	pS->setStatusPixmap(eOff, ":/fold_normal.png", ":/fold_hover.png");

	auto proxylabelName = new ClickProxyWidget<QLineEdit>(60, this);
	m_editLayerName = proxylabelName->proxyWidget();//new QLabel(QString::fromLocal8Bit("图层"));
	m_editLayerName->setFixedSize(60, 20);
	m_editLayerName->setText(QString::fromLocal8Bit("图层"));
	connect(m_editLayerName, &QLineEdit::editingFinished, pHelper, &MediaHelper::onEditFinished);
// 	editorlName->setStyleSheet(R"(
// background-color: rgb(35, 35, 35);
// color: rgb(177, 177, 177);
// font: 10pt "Candara";)");
	proxylabelName->setPos(40, 5);

	pS = new GraphicsStatusButton(this);
	pS->setPreferredSize(33, 20);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/visible_normal.png", ":/visible_hover.png");
	pS->setStatusPixmap(eOff, ":/unvisible_normal.png", ":/unvisible_hover.png");
	pS->setPos(100, 2);

	pS = new GraphicsStatusButton(this);
	pS->setPreferredSize(33, 20);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/onvoice_normal.png", ":/onvoice_hover.png");
	pS->setStatusPixmap(eOff, ":/offvoice_normal.png", ":/offvoice_hover.png");
	pS->setPos(130, 2);

	pS = new GraphicsStatusButton(this);
	pS->setPreferredSize(33, 20);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unlock_normal.png", ":/unlock_hover.png");
	pS->setStatusPixmap(eOff, ":/lock_normal.png", ":/lock_hover.png");
	pS->setPos(160, 2);

	GraphicsButton* btnDelete = new GraphicsButton(this);
	btnDelete->setPreferredSize(20, 12);
	btnDelete->setPixmap(":/delete.png");
	btnDelete->setPos(190, 2);
	connect(btnDelete, &GraphicsButton::clicked, pHelper, &MediaHelper::onDelete);
}

//析构时删除所有图层
LeftMediaLeader::~LeftMediaLeader()
{

}

void LeftMediaLeader::setWidget(MediaItemWidgetEditor* widget)
{
	widget->setParentItem(this);
	widget->setPos(QPointF(0,30));
	m_editor = widget;
}

IEditor* LeftMediaLeader::editor()
{
	return m_editor;
}

QVector<LeftHandleFellow*> LeftMediaLeader::init(const QDomElement& data, QGraphicsAnchorLayout* leftLayout)
{
	QString name = data.attribute("layerName");
	if (name.isEmpty())
	{
		name = QString::fromLocal8Bit("图层");
	}
	m_data = data;
	m_editLayerName->setText(name);
	//左边下拉项
	QVector<LeftHandleFellow*> arrLeft = { new LeftHandleFellow, new LeftHandleFellow , new LeftHandleFellow , new LeftHandleFellow, new LeftHandleFellow };

	EffectValueEditor* proxyWidget[5] = { new TransparencyWidgetEditor, new PositionWidgetEditor, new RotateWidgetEditor, new ScailWidgetEditor, new VoiceWidgetEditor };

	for (int i = 0;i < arrLeft.size(); ++i)
	{
		arrLeft[i]->setWidget(proxyWidget[i]);
	}

	QVector<LayerBase*> arrOrig(arrLeft.size());
	for (int i = 0;i < arrLeft.size(); ++i)
		arrOrig[i] = arrLeft[i];
	addGroupToLayout(arrOrig, leftLayout);

	return arrLeft;
}

void LeftMediaLeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	HandleLayerLeader::paint(painter, option, widget);
	painter->fillRect(rect().adjusted(0, 0, 0, -2), QColor(38, 38, 38));
}

void LeftMediaLeader::onEditFinished()
{
	QString name = m_editLayerName->text();
	if (name.isEmpty())
	{
		name = QString::fromLocal8Bit("图层");
	}
	m_data.setAttribute("layerName",name);
}

void LeftMediaLeader::onHideFellows(int before, int after)
{
	hideFellows(after == eOff);
}

LeftHandleFellow::LeftHandleFellow()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0, 0, 0, 4);
	setLayout(layout);
}

void LeftHandleFellow::setWidget(EffectValueEditor* widget)
{
	if (QGraphicsLinearLayout* lay = dynamic_cast<QGraphicsLinearLayout* >(layout()))
	{
		lay->addItem(widget);
	}
}

IEditor* LeftHandleFellow::editor()
{
	return dynamic_cast<EffectValueEditor*>(layout()->itemAt(0));
}

void LeftHandleFellow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	HandleLayerFellow::paint(painter, option, widget);
	painter->fillRect(rect().adjusted(0, 0, 0, -2), QColor(38, 38, 38));
}

void MediaHelper::onDelete()
{
	QTimer::singleShot(0, [this]() {
		delete m_help;
	});
}

LeftMarkerLeader::LeftMarkerLeader()
{
	MarkerHelper* pHelper = new MarkerHelper(this);

	auto proxylabelName = new ClickProxyWidget<QLineEdit>(60, this);
	m_editLayerName = proxylabelName->proxyWidget();
	m_editLayerName->setFixedSize(60, 20);
	m_editLayerName->setText(QString::fromLocal8Bit("控制"));
	connect(m_editLayerName, &QLineEdit::editingFinished, pHelper, &MarkerHelper::onEditFinished);
	proxylabelName->setPos(40, 5);

	GraphicsStatusButton * pS = new GraphicsStatusButton(this);
	pS->setPreferredSize(33, 20);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unlock_normal.png", ":/unlock_hover.png");
	pS->setStatusPixmap(eOff, ":/lock_normal.png", ":/lock_hover.png");
	pS->setPos(160, 2);

	GraphicsButton* btnDelete = new GraphicsButton(this);
	btnDelete->setPreferredSize(20, 12);
	btnDelete->setPixmap(":/delete.png");
	btnDelete->setPos(190, 2);
	connect(btnDelete, &GraphicsButton::clicked, pHelper, &MarkerHelper::onDelete);
}

void LeftMarkerLeader::setWidget(MarkerWidetEditor* widget)
{
	widget->setParentItem(this);
	widget->setPos(QPointF(0, 30));
	m_editor = widget;
}

void LeftMarkerLeader::init(const QDomElement& data, QGraphicsAnchorLayout* leftLayout)
{
	QString name = data.attribute("layerName");
	if (name.isEmpty())
	{
		name = QString::fromLocal8Bit("控制");
	}
	m_data = data;
	m_editLayerName->setText(name);
	//左边下拉项
	

	QVector<LayerBase*> arrOrig;
	addGroupToLayout(arrOrig, leftLayout);

}

void LeftMarkerLeader::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	HandleLayerLeader::paint(painter, option, widget);
	painter->fillRect(rect().adjusted(0, 0, 0, -2), QColor(38, 38, 38));
}

void LeftMarkerLeader::onEditFinished()
{
	QString name = m_editLayerName->text();
	if (name.isEmpty())
	{
		name = QString::fromLocal8Bit("图层");
	}
	m_data.setAttribute("layerName", name);
}

void MarkerHelper::onDelete()
{
	QTimer::singleShot(0, [this]() {
		delete m_help;
	});
}

void WhiteWidget::init(QGraphicsAnchorLayout* anchorLayout)
{
	QGraphicsLayoutItem* anchorTo = anchorLayout; 

	qreal spacing = 0;
	for (int i = anchorLayout->count() - 1; i >= 0; --i)
	{
		QGraphicsLayoutItem* item = anchorLayout->itemAt(i);
		if (LayerLeader* leader = dynamic_cast<LayerLeader*>(item))
		{
			anchorTo = leader;
			break;
		}
		else if (LayerBase* fellow = dynamic_cast<LayerBase*>(item))
		{
			if (fellow->isVisible())
			{
				spacing += fellow->rect().height();
			}
		}
	}


	bool top = anchorTo == anchorLayout;
	anchorLayout->addAnchor(this, Qt::AnchorTop, anchorTo, top ? Qt::AnchorTop : Qt::AnchorBottom)->setSpacing(spacing);
	anchorLayout->addAnchor(this, Qt::AnchorLeft, anchorLayout, Qt::AnchorLeft);
	anchorLayout->addAnchor(this, Qt::AnchorRight, anchorTo, Qt::AnchorRight);
	anchorLayout->addAnchor(this, Qt::AnchorBottom, anchorLayout, Qt::AnchorBottom);
}

void WhiteWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->fillRect(rect(), QColor(49,49,49));
}
