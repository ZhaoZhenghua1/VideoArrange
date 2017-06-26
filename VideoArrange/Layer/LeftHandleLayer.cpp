#include "LeftHandleLayer.h"
#include "../Controls/StatusButton.h"
#include "Controls/EffectValueEditor.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsAnchorLayout>
#include <QLineEdit>
#include <QDomElement>
enum EStatus
{
	eOff, eOn
};

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
	T* widget() { return m_widget; }

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
	QGraphicsProxyWidget* btnUnfold = new NoHoverProxyWidget(this);
	StatusButton* pS = new StatusButton;
	Helper* pHelper = new Helper(this);
	connect(pS, &StatusButton::statusChanged, pHelper, &Helper::onStatusChanged);
	btnUnfold->setWidget(pS);
	btnUnfold->setPos(0, 2);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unfold_normal.png", ":/unfold_hover.png");
	pS->setStatusPixmap(eOff, ":/fold_normal.png", ":/fold_hover.png");

	auto proxylabelName = new ClickProxyWidget<QLineEdit>(60, this);
	m_editLayerName = proxylabelName->widget();//new QLabel(QString::fromLocal8Bit("Í¼²ã"));
	m_editLayerName->setFixedSize(60, 20);
	m_editLayerName->setText(QString::fromLocal8Bit("Í¼²ã"));
	connect(m_editLayerName, &QLineEdit::editingFinished, pHelper, &Helper::onEditFinished);
// 	editorlName->setStyleSheet(R"(
// background-color: rgb(35, 35, 35);
// color: rgb(177, 177, 177);
// font: 10pt "Candara";)");
	proxylabelName->setPos(40, 5);

	QGraphicsProxyWidget* btnHide = new NoHoverProxyWidget(this);
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/visible_normal.png", ":/visible_hover.png");
	pS->setStatusPixmap(eOff, ":/unvisible_normal.png", ":/unvisible_hover.png");
	btnHide->setWidget(pS);
	btnHide->setPos(100, 2);

	QGraphicsProxyWidget* btnVoice = new NoHoverProxyWidget(this);
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/onvoice_normal.png", ":/onvoice_hover.png");
	pS->setStatusPixmap(eOff, ":/offvoice_normal.png", ":/offvoice_hover.png");
	btnVoice->setWidget(pS);
	btnVoice->setPos(130, 2);

	QGraphicsProxyWidget* btnLock = new NoHoverProxyWidget(this);
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unlock_normal.png", ":/unlock_hover.png");
	pS->setStatusPixmap(eOff, ":/lock_normal.png", ":/lock_hover.png");
	btnLock->setWidget(pS);
	btnLock->setPos(160, 2);
}

//Îö¹¹Ê±É¾³ýËùÓÐÍ¼²ã
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
		name = QString::fromLocal8Bit("Í¼²ã");
	}
	m_data = data;
	m_editLayerName->setText(name);
	//×ó±ßÏÂÀ­Ïî
	QVector<LeftHandleFellow*> arrLeft = { new LeftHandleFellow, new LeftHandleFellow , new LeftHandleFellow , new LeftHandleFellow, new LeftHandleFellow };

	EffectValueEditor* proxyWidget[5] = {
		::createValueEditor<TransparencyWidgetEditor>(),
		::createValueEditor<PositionWidgetEditor>(),
		::createValueEditor<RotateWidgetEditor>(),
		::createValueEditor<ScailWidgetEditor>(),
		::createValueEditor<VoiceWidgetEditor>() };

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
		name = QString::fromLocal8Bit("Í¼²ã");
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
