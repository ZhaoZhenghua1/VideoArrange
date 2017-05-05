#include "Layer.h"
#include "SwitchButton.h"
#include <QHBoxLayout>
#include <QLabel>

Layer::Layer()
{
	QVBoxLayout* pVBox = new QVBoxLayout;
	setLayout(pVBox);

	QHBoxLayout* pHTop = new QHBoxLayout;
	pVBox->addLayout(pHTop);

	SwitchButton* pLayerFold = new SwitchButton;
	connect(pLayerFold, &SwitchButton::sigStatusAboutToChange, this, &Layer::onLayerUnfold);
	pLayerFold->setSwitchPixmap(":/map1.png", ":/map2.png");
	pHTop->addWidget(pLayerFold);

	m_pLabel = new QLabel("layer");
	pHTop->addWidget(m_pLabel);

	
	QHBoxLayout* pHBottom = new QHBoxLayout;
	pVBox->addLayout(pHBottom);

	
	SwitchButton* pLayerLightOn = new SwitchButton;
	m_arr3Btns[0] = pLayerLightOn;
	pLayerLightOn->setSwitchPixmap(":/map3.png", ":/map4.png");
	connect(pLayerLightOn, &SwitchButton::sigStatusAboutToChange, this, &Layer::onLightOn);
	pHBottom->addWidget(pLayerLightOn);
	
	SwitchButton* pLayerVoice = new SwitchButton;
	pLayerVoice->setSwitchPixmap(":/map5.png", ":/map6.png");
	connect(pLayerVoice, &SwitchButton::sigStatusAboutToChange, this, &Layer::onVoiceOn);
	pHBottom->addWidget(pLayerVoice);
	m_arr3Btns[1] = pLayerVoice;
	
	SwitchButton* pLayerLock = new SwitchButton;
	pLayerLock->setSwitchPixmap(":/map7.png", ":/map8.png");
	connect(pLayerLock, &SwitchButton::sigStatusAboutToChange, this, &Layer::onUnlock);
	pHBottom->addWidget(pLayerLock);
	m_arr3Btns[2] = pLayerLock;
}


Layer::~Layer()
{
}

void Layer::setLayerName(const QString& layername)
{
	m_pLabel->setText(layername);
}

bool Layer::onLayerUnfold(bool unfold)
{
	emit sigLayerUnfold(unfold);

	for (SwitchButton* btn : m_arr3Btns)
	{
		unfold ? btn->show() : btn->hide();
	}
	
	return true;
}

bool Layer::onLightOn(bool lignton)
{
	emit sigLightOn(lignton);
	return true;
}

bool Layer::onVoiceOn(bool voiceon)
{
	emit sigVoiceOn(voiceon);
	return true;
}

bool Layer::onUnlock(bool unlock)
{
	emit sigUnlock(unlock);
	return true;
}
