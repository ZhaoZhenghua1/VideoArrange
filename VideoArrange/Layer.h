#pragma once
#include <QWidget>

class SwitchButton;
class QLabel;
class Layer : public QWidget
{
	Q_OBJECT
public:
	Layer();
	~Layer();
public:
	void setLayerName(const QString& layername);
	private slots:
	bool onLayerUnfold(bool unfold);
	bool onLightOn(bool lignton);
	bool onVoiceOn(bool voiceon);
	bool onUnlock(bool unlock);
signals:
	void sigLayerUnfold(bool unfold);
	void sigLightOn(bool lighton);
	void sigVoiceOn(bool voiceon);
	void sigUnlock(bool unlock);
private:
	SwitchButton* m_arr3Btns[3] = { 0 };
	QLabel* m_pLabel = nullptr;
};

