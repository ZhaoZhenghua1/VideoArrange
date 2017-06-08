#include "LayerTitle.h"
#include "../Controls/StatusButton.h"
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QGraphicsSceneWheelEvent>
#include <QCursor>
#include <QPainter>
#include <QGraphicsAnchorLayout>
enum EStatus
{
	eOff, eOn
};
LayerTitle::LayerTitle()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);
	setLayout(layout);
	QGraphicsProxyWidget* btnUnfold = new QGraphicsProxyWidget;
	StatusButton* pS = new StatusButton;
	btnUnfold->setWidget(pS);
	pS->registerStatus({ eOn,eOff });
	pS->setStatusPixmap(eOn, ":/unfold_normal.png", ":/unfold_hover.png");
	pS->setStatusPixmap(eOff, ":/fold_normal.png", ":/fold_hover.png");
	layout->addItem(btnUnfold);
	QGraphicsProxyWidget* labelName = new QGraphicsProxyWidget;
	m_labelName = new QLabel(QString::fromLocal8Bit("Í¼²ã"));
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
	setAcceptHoverEvents(true);
}


LayerTitle::~LayerTitle()
{
}

QSizeF LayerTitle::sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */)
{
	QSizeF ret = QGraphicsWidget::sizeHint(which, constraint);
	ret.setHeight(24);
	return ret;
}


void LayerTitle::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QRectF rec = rect();
	rec.adjust(0, 0, 0, -(rec.height() - 4));
	QPointF hpos = event->pos();
	if (rec.contains(hpos))
	{
		setCursor(Qt::SizeVerCursor);
	}
}

void LayerTitle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);
}

void LayerTitle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(boundingRect(),QColor( 35, 35, 35));
}

void LayerTitle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (!m_bIsPressTitle)
		return;

	QGraphicsWidget* par = parentWidget();
	if (!par)
		return;

	QGraphicsAnchorLayout* layout = dynamic_cast<QGraphicsAnchorLayout*>(par->layout());
	if (!layout)
		return;

	int index = 0;
	for (; index < layout->count(); ++index)
		if(layout->itemAt(index) == this)
			break;

	if (0 == index || index == layout->count())
		return;

	QGraphicsLayoutItem * secondItem = layout->itemAt(index - 1);

	QGraphicsAnchor * anchorSpace = layout->anchor(this, Qt::AnchorTop, secondItem, Qt::AnchorBottom);
	if (!anchorSpace)
		return;

	int delt = event->pos().y() - event->lastPos().y();

	anchorSpace->setSpacing(anchorSpace->spacing() + delt);
}

void LayerTitle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QRectF rec = rect();
	rec.adjust(0, 0, 0, -(rec.height() - 4));
	if (rec.contains(event->pos()))
	{
		m_bIsPressTitle = true;
	}
}

void LayerTitle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_bIsPressTitle = false;
}

TitleLayer::TitleLayer()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Horizontal);
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

void TitleLayer::onStatusChanged(int before, int after)
{
	hideFellows(after == eOff);
}

FellowLayer::FellowLayer()
{
	QGraphicsLinearLayout* layout = new QGraphicsLinearLayout;
	setLayout(layout);
}

void FellowLayer::setWidget(QGraphicsWidget* widget)
{
	if (QGraphicsLinearLayout* lay = dynamic_cast<QGraphicsLinearLayout* >(layout()))
	{
		lay->addItem(widget);
	}
}
