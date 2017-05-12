#pragma once
#include <QGraphicsRectItem>
#include <QObject>

//×ÊÔ´Ïî
class SizeGripItem;
class TimeVideoItem : public QGraphicsRectItem
{
public:
	TimeVideoItem(const QString& res);
	~TimeVideoItem();
	void setRect(const QRectF& rect);
protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)Q_DECL_OVERRIDE;
	virtual  QVariant itemChange(GraphicsItemChange change, const QVariant &value)Q_DECL_OVERRIDE;
private:
	QString m_qsRes;
	SizeGripItem* m_sizeGrip = nullptr;

};

