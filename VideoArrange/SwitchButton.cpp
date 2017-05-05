#include "SwitchButton.h"
#include <QPainter>

/*
QPushButton{border-image:url(:/normal.png);}
QPushButton:hover{border-image:url(:/hover.png);}
QPushButton:pressed{border-image:url(:/press.png);}
*/
SwitchButton::SwitchButton()
{
	connect(this, &QPushButton::clicked, this, &SwitchButton::onClicked);
}

SwitchButton::~SwitchButton()
{
}

void SwitchButton::setSwitchPixmap(const QString& map1, const QString& map2)
{

	m_qsMapStatus0 = map1;
	m_qsMapStatus1 = map2;
}

void SwitchButton::onClicked()
{
	if (emit sigStatusAboutToChange(!m_bStatus))
	{
		m_bStatus = !m_bStatus;
		update();
	}
}
