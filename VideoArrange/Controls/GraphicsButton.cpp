#include "GraphicsButton.h"
#include <QPixmap>
#include <QPainter>

void GraphicsButton::setPixmap(const QString& path)
{
	m_pixmap = QPixmap(path);
}

void GraphicsButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->drawPixmap(rect(), m_pixmap, m_pixmap.rect());
}

void GraphicsButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsWidget::mousePressEvent(event);
	emit clicked();
}

GraphicsStatusButton::GraphicsStatusButton(QGraphicsItem* parent /*= nullptr*/):GraphicsButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void GraphicsStatusButton::registerStatus(const QVector<int>& status)
{
	m_status = status;
}

void GraphicsStatusButton::setStatusPixmap(const int status, const QString& mapnormal, const QString& maphover)
{
	if (!m_status.contains(status))
	{
		Q_ASSERT(false);
		return;
	}
	m_mapStatusToMaps[status] = { mapnormal, maphover };

	if (m_status.first() == status)
	{
		setStatusPixmap(status);
	}
}

void GraphicsStatusButton::setStatusPixmap(const int status)
{
	QString normal = m_mapStatusToMaps[status].first;
	QString hover = m_mapStatusToMaps[status].second;
	if (QPixmap(hover).isNull())
	{
		hover = normal;
	}
	GraphicsButton::setPixmap(normal);
	update();
}

void GraphicsStatusButton::onClicked()
{
	if (m_status.size() <= 1)
	{
		return;
	}

	int indexbefore = m_currentStatusIndex;
	int indexafter = indexbefore + 1;
	if (indexafter >= m_status.size())
	{
		indexafter = 0;
	}
	m_currentStatusIndex = indexafter;

	int statusBefore = m_status[indexbefore];
	int statusAfter = m_status[indexafter];
	emit statusChanged(statusBefore, statusAfter);

	setStatusPixmap(statusAfter);
}