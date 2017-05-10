#pragma once

#include <QGraphicsView>
class QGraphicsWidget;
class RootWidget;
class LayerView : public QGraphicsView
{
	Q_OBJECT
public:
	LayerView();
	~LayerView();
protected:
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
private slots:
	void onUnfoldRelayout();
private:
	RootWidget* m_rootWidget;
};
