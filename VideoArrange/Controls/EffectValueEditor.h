#pragma once
#include <QGraphicsProxyWidget>
#include <QWidget>
#include "Controls/IOriginatorEditor.h"
#include <functional>
#include <QComboBox>
/*
左边的值编辑工具
*/

class QGraphicsTextItem;
class EffectEditor;
class QLineEdit;
class QTimeEdit;
class QComboBox;
class QCheckBox;

//值编辑器
class EffectValueEditor : public QGraphicsWidget, public IEditor
{
public:
	using QGraphicsWidget::QGraphicsWidget;

	//设置当前编辑值
	virtual void setValue(const QString&) = 0;
	//链接原发器
	virtual void setOriginator(IOriginator* orig)override;
public:
	//设置原发器的值
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

//编辑Marker
class MarkerWidetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	MarkerWidetEditor();
public slots:
	//值改变后，提交改变后的值
	void onTextChanged();
	//选中类型后，改变子编辑控件类型
	void onChangeEditor();
protected:
	virtual void setValue(const QString& value)override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */);
private:
	void createMarkerEditor(const QString& type);
private:
	QTimeEdit* m_editTime = nullptr;
	QComboBox* m_comboType = nullptr;
	//QLineEdit* m_editParam = nullptr;
	IOriginator* m_paraEditor = nullptr;

};

class PauseMarkerEditor : public QGraphicsWidget, public IOriginator
{
	Q_OBJECT
		signals :
	void sigChanged();
public:
	PauseMarkerEditor(QGraphicsWidget* parent = nullptr);
protected:
	//设置数据
	virtual void setQsData(const QString& data) 
	{
		
	}
	//获取数据
	virtual QString toQsData() { return ""; }
};

class PostionMarkerEditor : public QGraphicsWidget, public IOriginator
{
	Q_OBJECT
		signals:
	void sigChanged();
public:
	PostionMarkerEditor(QGraphicsWidget* parent = nullptr);
protected:
	//设置数据
	virtual void setQsData(const QString& data);
	//获取数据
	virtual QString toQsData();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	QLineEdit* m_editParam = nullptr;
};

class JumpMarkerEditor : public QGraphicsWidget, public IOriginator
{
	Q_OBJECT
		signals :
	void sigChanged();
public:
	JumpMarkerEditor(QGraphicsWidget* parent = nullptr);
protected:
	//设置数据
	virtual void setQsData(const QString& data);
	//获取数据
	virtual QString toQsData();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	QLineEdit* m_editParam = nullptr;
};

class TriggerMarkerEditor : public QGraphicsWidget, public IOriginator
{
	Q_OBJECT
		signals :
	void sigChanged();
public:
	TriggerMarkerEditor(QGraphicsWidget* parent = nullptr);
protected:
	//设置数据
	virtual void setQsData(const QString& data);
	//获取数据
	virtual QString toQsData();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	QLineEdit* m_editParam = nullptr;
};