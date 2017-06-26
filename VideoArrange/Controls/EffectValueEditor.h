#pragma once
#include <QGraphicsProxyWidget>
#include <QWidget>
#include "Controls/IOriginatorEditor.h"

/*
��ߵ�ֵ�༭����
*/

class QGraphicsTextItem;
class EffectEditor;
class QLineEdit;
class QTimeEdit;
class QCheckBox;

//ֵ�༭��
class EffectValueEditor : public QGraphicsWidget, public IEditor
{
public:
	using QGraphicsWidget::QGraphicsWidget;

	virtual void setValue(const QString&) = 0;

	virtual void setOriginator(IOriginator* orig)override;
public:
	void setOrigData(const QString& value);
};

//�༭ý����ʱ�䣬ʱ������Ϣ
class MediaItemWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	MediaItemWidgetEditor();
public slots:
	void onValueChanged();

protected:
	virtual void setValue(const QString&);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QTimeEdit* m_editTimeStart = nullptr;
	QTimeEdit* m_editTimeLen = nullptr;
	QTimeEdit* m_editOffset = nullptr;
};

//�༭͸����
class TransparencyWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	TransparencyWidgetEditor();
public slots:
	void onTextChanged();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QLineEdit* m_edit = nullptr;
	QTimeEdit* m_editTime = nullptr;
};

//�༭λ��
class PositionWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	PositionWidgetEditor();
public slots:
	void onTextChanged();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QLineEdit* m_editX = nullptr;
	QLineEdit* m_editY = nullptr;
	QTimeEdit* m_editTime = nullptr;
};

//�༭��ת
class RotateWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	RotateWidgetEditor();
public slots:
	void onTextChanged();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QLineEdit* m_edit = nullptr;
	QTimeEdit* m_editTime = nullptr;
};

//���ű༭
class ScailWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	ScailWidgetEditor();
public slots:
	void onTextChanged();
	void onStateChanged();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QLineEdit* m_editX = nullptr;
	QLineEdit* m_editY = nullptr;
	QTimeEdit* m_editTime = nullptr;
	QCheckBox* m_checkBox = nullptr;
	bool m_bXequalY = true;
};

//�༭��ת
class VoiceWidgetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	VoiceWidgetEditor();
public slots:
	void onTextChanged();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	QLineEdit* m_edit = nullptr;
	QTimeEdit* m_editTime = nullptr;
};

template<class T>
EffectValueEditor* createValueEditor()
{
	return new T;
}

class NoHoverProxyWidget : public QGraphicsProxyWidget
{
public:
	using QGraphicsProxyWidget::QGraphicsProxyWidget;
protected:
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
};