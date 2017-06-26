#pragma once
#include <QGraphicsProxyWidget>
#include <QWidget>
#include "Controls/IOriginatorEditor.h"

/*
左边的值编辑工具
*/

class QGraphicsTextItem;
class EffectEditor;
class QLineEdit;
class QTimeEdit;
class QCheckBox;

//值编辑器
class EffectValueEditor : public QGraphicsWidget, public IEditor
{
public:
	using QGraphicsWidget::QGraphicsWidget;

	virtual void setValue(const QString&) = 0;

	virtual void setOriginator(IOriginator* orig)override;
public:
	void setOrigData(const QString& value);
};

//编辑媒体项时间，时长等信息
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

//编辑透明度
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

//编辑位置
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

//编辑旋转
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

//缩放编辑
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

//编辑旋转
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