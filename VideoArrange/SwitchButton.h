#pragma once
#include <QPushButton>
#include <QPixmap>

class SwitchButton: public QPushButton
{
	Q_OBJECT
public:
	SwitchButton();
	~SwitchButton();
public:
	void setSwitchPixmap(const QString& map1, const QString& map2);
private slots:
void onClicked();
signals:
bool sigStatusAboutToChange(bool status);
private:
	QString m_qsMapStatus0;
	QString m_qsMapStatus1;
	bool m_bStatus = true;
};

