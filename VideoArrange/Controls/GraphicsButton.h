#pragma once
#include <QGraphicsWidget>
//QGraphicsScene 里面的按钮
class GraphicsButton : public QGraphicsWidget
{
	Q_OBJECT
public:
	using QGraphicsWidget::QGraphicsWidget;
public:
	void setPixmap(const QPixmap& map);
	void setPixmap(const QString& path);
signals:
	void clicked();
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
private:
	QPixmap m_pixmap;
};

class GraphicsStatusButton : public GraphicsButton
{
	Q_OBJECT
public:
	GraphicsStatusButton(QGraphicsItem* parent = nullptr);
public:
	//回环状态
	void registerStatus(const QVector<int>& status);
	//设置状态图片
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