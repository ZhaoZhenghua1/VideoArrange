#include "StatusButton.h"



StatusButton::StatusButton(QWidget* parent):QPushButton(parent)
{
	connect(this, SIGNAL(pressed()), this, SLOT(onClicked()));
	setFixedSize(33, 20);
}


StatusButton::~StatusButton()
{
}

void StatusButton::registerStatus(const QVector<int>& status)
{
	m_status = status;
}

void StatusButton::setStatusPixmap(const int status, const QString& mapnormal, const QString& maphover)
{
	if (!m_status.contains(status))
	{
		Q_ASSERT(false);
		return;
	}
	m_mapStatusToMaps[status] = {mapnormal, maphover};

	if (m_status.first() == status)
	{
		setStatusPixmap(status);
	}
}

void StatusButton::setStatusPixmap(const int status)
{
	QString stylesheet = R"(
QPushButton{border-image:url(%1);}
QPushButton:hover{border-image:url(%2);}
QPushButton:pressed{border-image:url(%3);}
)";
	QString normal = m_mapStatusToMaps[status].first;
	QString hover = m_mapStatusToMaps[status].second;
	if (QPixmap(hover).isNull())
	{
		hover = normal;
	}
	setStyleSheet(stylesheet.arg(normal).arg(hover).arg(hover));
}

void StatusButton::toStatus(const int status)
{
	if (!m_status.contains(status))
	{
		return;
	}

	int indexbefore = m_currentStatusIndex;
	int indexafter = m_status.indexOf(status);
	if (indexafter == indexbefore)
	{
		return;
	}

	m_currentStatusIndex = indexafter;

	int statusBefore = m_status[indexbefore];
	int statusAfter = m_status[indexafter];
	emit statusChanged(statusBefore, statusAfter);

	setStatusPixmap(statusAfter);
}

void StatusButton::onClicked()
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

