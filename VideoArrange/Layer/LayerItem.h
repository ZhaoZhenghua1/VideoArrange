#pragma once
#include <QGraphicsWidget>

class SwitchButton;
class GraphicsLayoutTextItem;
//Í¼²ã
class LayerItem : public QGraphicsWidget
{
	Q_OBJECT
public:
	LayerItem();
	LayerItem(const QString& layername);
	~LayerItem();
public:
	void setLayerName(const QString& layername);
	private slots:
	bool onLayerUnfold(bool unfold);
	bool onLightOn(bool lignton);
	bool onVoiceOn(bool voiceon);
	bool onUnlock(bool unlock);

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint /* = QSizeF() */);
signals:
	void sigLayerUnfold(bool unfold);
	void sigLightOn(bool lighton);
	void sigVoiceOn(bool voiceon);
	void sigUnlock(bool unlock);
private:
	SwitchButton* m_arr3Btns[3] = { 0 };
	GraphicsLayoutTextItem* m_pLabel = nullptr;
};

