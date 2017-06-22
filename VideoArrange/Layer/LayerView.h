#pragma once
#include "../Document/Observer.h"

#include <QGraphicsView>
class QGraphicsWidget;
class RootWidget;

class LayerView : public QGraphicsView, public Observer
{
	Q_OBJECT
public:
	LayerView();
	~LayerView();
protected:
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
	virtual void paintEvent(QPaintEvent *event)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *event);
protected:
	virtual void init() override;
private:
	QGraphicsWidget* m_rootWidget = nullptr;
};

