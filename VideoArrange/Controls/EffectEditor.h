#pragma once
#include <QGraphicsWidget>
#include <QDomElement>
#include "Controls/IOriginatorEditor.h"

/*
右边时间线上效果编辑窗口
*/

class TimeZone;
class RightLayer;
class EffectEditor : public QGraphicsWidget, public IOriginator
{
	Q_OBJECT
protected:
	class Line
	{
	public:
		Line(EffectEditor* owner, const QDomElement& elem) :m_owner(owner), m_data(elem) {}
		QVector<qreal> points()const;
		void addHeadAndTail(QVector<qreal>& ret);
		int addAdjustPoint(qreal time, const QString& value);
//		void adjustPointValue(int index, qreal time, const QString& value);
		void deleteAdjustPoint(int index);
		int selectIndex(const QPointF&);
		bool isOnLine(const QPointF& pos);
		void paint(QPainter *painter, int selectIndex);
		void setPointTime(int index, unsigned int time);
		QString pointValue(int index);
	private:
		EffectEditor* m_owner = nullptr;
		QDomElement m_data;
	};
public:
	EffectEditor(TimeZone* timeZone);
	~EffectEditor();
	void initDataFromParent(QDomElement& parentElem);
	//水平方向调整位置
	void updatePos(qreal left, qreal width);
	void parentChanged(QGraphicsWidget* parent);
	//取消选择
	void clearSelect();
public:
	void setValue(int index, const QString& value);
	QString value(int index) const;
protected:
	//xPos位置上的值
	QString positionToValue(qreal xPos)  const;
	//当前时间curtime上的值，before 前一个控制点， after 后一个控制点
	virtual QString average(QDomElement before, QDomElement after, qreal curtime) const = 0;
	//数据类型，通过类型找到节点数据
	virtual QString type() = 0;

	//获取所在图层
	RightLayer* layer();

	//设置数据
	virtual void setQsData(const QString& data);
	//获取数据
	virtual QString toQsData();
protected:
	//鼠标消息
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)override;
	virtual void keyPressEvent(QKeyEvent *event)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	Line getLine();
protected:
	QDomElement m_data;
	TimeZone* m_timeZone = nullptr;
	int m_pressIndex = -1;
};

class TransparencyEditor : public EffectEditor
{
public:
	using EffectEditor::EffectEditor;
protected:
	virtual QString average(QDomElement before, QDomElement after, qreal curtime)  const;
	virtual QString type() override { return "Transparency"; };
};

class PositionEdiotr : public EffectEditor
{
public:
	using EffectEditor::EffectEditor;
protected:
	virtual QString average(QDomElement before, QDomElement after, qreal curtime)  const;
	virtual QString type() override { return "Position"; };
};

class RotateEdiotr : public EffectEditor
{
public:
	using EffectEditor::EffectEditor;
protected:
	virtual QString average(QDomElement before, QDomElement after, qreal curtime)  const;
	virtual QString type() override { return "Rotate"; };
};

class ScailEdiotr : public EffectEditor
{
public:
	using EffectEditor::EffectEditor;
protected:
	virtual QString average(QDomElement before, QDomElement after, qreal curtime) const;
	virtual QString type() override { return "Scail"; };
};

class VoiceEditor : public EffectEditor
{
public:
	using EffectEditor::EffectEditor;
protected:
	virtual QString average(QDomElement before, QDomElement after, qreal curtime)  const;
	virtual QString type() override { return "Voice"; };
};

