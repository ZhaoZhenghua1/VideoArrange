#pragma once
#include <QGraphicsWidget>
#include <QDomElement>

class TimeZone;
class EffectEdit : public QGraphicsWidget
{
protected:
	class Line
	{
	public:
		Line(EffectEdit* owner, const QDomElement& elem):m_owner(owner), m_data(elem){}
		QVector<QPointF> points()const ;
		void addHeadAndTail(QVector<QPointF>& ret);
		int addAdjustPoint(qreal time, qreal value);
		void adjustPointValue(int index, qreal time, qreal value);
		void deleteAdjustPoint(int index);
		int selectIndex(const QPointF&);
		bool isOnLine(const QPointF& pos);
		void paint(QPainter *painter, int selectIndex);
	private:
		EffectEdit* m_owner = nullptr;
		QDomElement m_data;
	};
public:
	EffectEdit(TimeZone* timeZone);
	~EffectEdit();
	void initData(const QDomElement& elem);
	//水平方向调整位置
	void updatePos(qreal left, qreal width);
	void parentChanged(QGraphicsWidget* parent);
protected:
	virtual unsigned int lineCount() const = 0;
	virtual Line lineAt(unsigned int index) = 0;
	virtual qreal valueToPosition(qreal val) const = 0;
	virtual qreal positionToValue(qreal yPos)  const = 0;
protected:
	//鼠标消息
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void keyPressEvent(QKeyEvent *event)override;
	virtual void focusInEvent(QFocusEvent *event)override;
	virtual void focusOutEvent(QFocusEvent *event)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	
protected:
	QDomElement m_data;
	TimeZone* m_timeZone = nullptr;
	int m_selectedLineIndex = -1, m_selectedPointIndex = -1;//选中的线，选中的点
};

class TransparencyEdit : public EffectEdit
{
public:
	using EffectEdit::EffectEdit;
protected:
	virtual unsigned int lineCount() const override;
	virtual Line lineAt(unsigned int index) ;
	virtual qreal valueToPosition(qreal val) const ;
	virtual qreal positionToValue(qreal yPos)  const ;
};

