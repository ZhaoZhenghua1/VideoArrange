#pragma once

#include <QPushButton>
#include <QMap>
#include <QPair>
//×´Ì¬°´Å¥
class StatusButton : public QPushButton
{
	Q_OBJECT
public:
	StatusButton(QWidget* parent = nullptr);
	~StatusButton();
public:
	//»Ø»·×´Ì¬
	void registerStatus(const QVector<int>& status);
	//ÉèÖÃ×´Ì¬Í¼Æ¬
	void setStatusPixmap(const int status, const QString& mapnormal, const QString& maphover);
signals:
	void statusChanged(int before, int after);
private slots:
void onClicked();
private:
	void setStatusPixmap(const int status);
private:
	QMap<int, QPair<QString, QString>> m_mapStatusToMaps;
	QVector<int> m_status;
	unsigned int m_currentStatusIndex = 0;
};

