#include "SwitchButton.h"
#include <QPainter>

/*
QPushButton{border-image:url(:/normal.png);}
QPushButton:hover{border-image:url(:/hover.png);}
QPushButton:pressed{border-image:url(:/press.png);}
*/
SwitchButton::SwitchButton()
{
	
}

SwitchButton::~SwitchButton()
{
}

void SwitchButton::setSwitchPixmap(const QString& map1, const QString& map2)
{
	m_qsMapStatus0 = map1;
	m_qsMapStatus1 = map2;
	setRect(m_qsMapStatus0.rect());
}

void SwitchButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (emit sigStatusAboutToChange(!m_bStatus))
	{
		m_bStatus = !m_bStatus;
		update();
	}
}

void SwitchButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	QPixmap pixmap = m_bStatus ? m_qsMapStatus0 : m_qsMapStatus1;
	painter->drawPixmap(QRect(rect().left(), rect().top(), rect().width(), rect().height()), QPixmap(pixmap));
}
