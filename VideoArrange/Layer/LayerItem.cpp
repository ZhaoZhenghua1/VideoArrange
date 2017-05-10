#include "LayerItem.h"
#include "SwitchButton.h"
#include <QGraphicsLinearLayout> 
#include "GraphicsLayoutTextItem.h"
#include "SwitchButton.h"

LayerItem::LayerItem()
{
	QGraphicsLinearLayout* vlayout = new QGraphicsLinearLayout(Qt::Vertical);
	setLayout(vlayout);

	QGraphicsLinearLayout* hlayout = new QGraphicsLinearLayout(Qt::Horizontal);
	vlayout->addItem(hlayout);

	SwitchButton* pLayerFold = new SwitchButton;
	connect(pLayerFold, &SwitchButton::sigStatusAboutToChange, this, &LayerItem::onLayerUnfold);
	pLayerFold->setSwitchPixmap(":/map1.png", ":/map2.png");
	hlayout->addItem(pLayerFold);

	m_pLabel = new GraphicsLayoutTextItem("layer");
	hlayout->addItem(m_pLabel);

	
	QGraphicsLinearLayout* pHBottom = new QGraphicsLinearLayout(Qt::Horizontal);
	vlayout->addItem(pHBottom);
	
	SwitchButton* pLayerLightOn = new SwitchButton;
	m_arr3Btns[0] = pLayerLightOn;
	pLayerLightOn->setSwitchPixmap(":/map3.png", ":/map4.png");
	connect(pLayerLightOn, &SwitchButton::sigStatusAboutToChange, this, &LayerItem::onLightOn);
	pHBottom->addItem(pLayerLightOn);
	
	SwitchButton* pLayerVoice = new SwitchButton;
	pLayerVoice->setSwitchPixmap(":/map5.png", ":/map6.png");
	connect(pLayerVoice, &SwitchButton::sigStatusAboutToChange, this, &LayerItem::onVoiceOn);
	pHBottom->addItem(pLayerVoice);
	m_arr3Btns[1] = pLayerVoice;
	
	SwitchButton* pLayerLock = new SwitchButton;
	pLayerLock->setSwitchPixmap(":/map7.png", ":/map8.png");
	connect(pLayerLock, &SwitchButton::sigStatusAboutToChange, this, &LayerItem::onUnlock);
	pHBottom->addItem(pLayerLock);
	m_arr3Btns[2] = pLayerLock;

}


LayerItem::LayerItem(const QString& layername):LayerItem()
{
	setLayerName(layername);
}

LayerItem::~LayerItem()
{
}

void LayerItem::setLayerName(const QString& layername)
{
	m_pLabel->setText(layername);
}

bool LayerItem::onLayerUnfold(bool unfold)
{
	for (SwitchButton* btn : m_arr3Btns)
	{
		unfold ? btn->show() : btn->hide();
	}
	layout()->invalidate();
	emit sigLayerUnfold(unfold);
	return true;
}

bool LayerItem::onLightOn(bool lignton)
{
	emit sigLightOn(lignton);
	return true;
}

bool LayerItem::onVoiceOn(bool voiceon)
{
	emit sigVoiceOn(voiceon);
	return true;
}

bool LayerItem::onUnlock(bool unlock)
{
	emit sigUnlock(unlock);
	return true;
}

#include <QPainter>
void LayerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->fillRect(boundingRect(), Qt::blue);
}

QSizeF LayerItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */)
{
	return QGraphicsWidget::sizeHint(which, constraint);
}
