#pragma once
#include <QGraphicsWidget>
#include <QDomElement>
#include "Controls/IOriginatorEditor.h"

/*
�ұ�ʱ������Ч���༭����
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
	//ˮƽ�������λ��
	void updatePos(qreal left, qreal width);
	void parentChanged(QGraphicsWidget* parent);
	//ȡ��ѡ��
	void clearSelect();
public:
	void setValue(int index, const QString& value);
	QString value(int index) const;
protected:
	//xPosλ���ϵ�ֵ
	QString positionToValue(qreal xPos)  const;
	//��ǰʱ��curtime�ϵ�ֵ��before ǰһ�����Ƶ㣬 after ��һ�����Ƶ�
	virtual QString average(QDomElement before, QDomElement after, qreal curtime) const = 0;
	//�������ͣ�ͨ�������ҵ��ڵ�����
	virtual QString type() = 0;

	//��ȡ����ͼ��
	RightLayer* layer();

	//��������
	virtual void setQsData(const QString& data);
	//��ȡ����
	virtual QString toQsData();
protected:
	//�����Ϣ
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

