#include "LeftHandleLayer.h"
#include "../Controls/StatusButton.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QPainter>
#include <QGraphicsAnchorLayout>
enum EStatus
{
	eOff, eOn
};

HandleTitle::HandleTitle()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	setLayout(layout);
	QGraphicsProxyWidget* btnUnfold = new QGraphicsProxyWidget;
	StatusButton* pS = new StatusButton;
	Helper* pHelper = new Helper(this);
	connect(pS, &StatusButton::statusChanged, pHelper, &Helper::onStatusChanged);
	btnUnfold->setWidget(pS);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unfold_normal.png", ":/unfold_hover.png");
	pS->setStatusPixmap(eOff, ":/fold_normal.png", ":/fold_hover.png");
	layout->addItem(btnUnfold);
	QGraphicsProxyWidget* labelName = new QGraphicsProxyWidget;
	QLabel* m_labelName = new QLabel(QString::fromLocal8Bit("Í¼²ã"));
	m_labelName->setMinimumWidth(50);
	m_labelName->setMaximumWidth(95);
	m_labelName->setStyleSheet(R"(
background-color: rgb(35, 35, 35);
color: rgb(177, 177, 177);
font: 10pt "Candara";)");
	labelName->setWidget(m_labelName);
	layout->addItem(labelName);
	QGraphicsProxyWidget* btnHide = new QGraphicsProxyWidget;
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/visible_normal.png", ":/visible_hover.png");
	pS->setStatusPixmap(eOff, ":/unvisible_normal.png", ":/unvisible_hover.png");
	btnHide->setWidget(pS);
	layout->addItem(btnHide);
	QGraphicsProxyWidget* btnVoice = new QGraphicsProxyWidget;
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/onvoice_normal.png", ":/onvoice_hover.png");
	pS->setStatusPixmap(eOff, ":/offvoice_normal.png", ":/offvoice_hover.png");
	btnVoice->setWidget(pS);
	layout->addItem(btnVoice);
	QGraphicsProxyWidget* btnLock = new QGraphicsProxyWidget;
	pS = new StatusButton;
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unlock_normal.png", ":/unlock_hover.png");
	pS->setStatusPixmap(eOff, ":/lock_normal.png", ":/lock_hover.png");
	btnLock->setWidget(pS);
	layout->addItem(btnLock);
}

void HandleTitle::onStatusChanged(int before, int after)
{
	hideFellows(after == eOff);
}

HandleFellow::HandleFellow()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	layout->setContentsMargins(0, 0, 0, 4);
	setLayout(layout);
}

void HandleFellow::setWidget(QGraphicsWidget* widget)
{
	if (QGraphicsLinearLayout* lay = dynamic_cast<QGraphicsLinearLayout* >(layout()))
	{
		lay->addItem(widget);
	}
}
