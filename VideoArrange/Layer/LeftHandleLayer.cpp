#include "LeftHandleLayer.h"
#include "../Controls/StatusButton.h"
#include "Controls/EffectValueEditor.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsAnchorLayout>
enum EStatus
{
	eOff, eOn
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

	QGraphicsProxyWidget* labelName = new NoHoverProxyWidget(this);
	QLabel* m_labelName = new QLabel(QString::fromLocal8Bit("Í¼²ã"));
	m_labelName->setStyleSheet(R"(
background-color: rgb(35, 35, 35);
color: rgb(177, 177, 177);
font: 10pt "Candara";)");
	labelName->setWidget(m_labelName);
	labelName->setPos(40, 5);

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

QVector<LeftHandleFellow*> LeftMediaLeader::init(QGraphicsAnchorLayout* leftLayout)
{
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
