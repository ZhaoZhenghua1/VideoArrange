#pragma once

#include <QPushButton>
#include <QMap>
#include <QPair>
//״̬��ť
class StatusButton : public QPushButton
{
	Q_OBJECT
public:
	StatusButton(QWidget* parent = nullptr);
	~StatusButton();
public:
	//�ػ�״̬
	void registerStatus(const QVector<int>& status);
	//����״̬ͼƬ
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

