#pragma once
#include <QGraphicsWidget>
//QGraphicsScene ����İ�ť
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