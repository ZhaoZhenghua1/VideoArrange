#pragma once
#include <QGraphicsProxyWidget>
#include <QWidget>
#include "Controls/IOriginatorEditor.h"
#include <functional>
#include <QComboBox>
/*
��ߵ�ֵ�༭����
*/

class QGraphicsTextItem;
class EffectEditor;
class QLineEdit;
class QTimeEdit;
class QComboBox;
class QCheckBox;

//ֵ�༭��
class EffectValueEditor : public QGraphicsWidget, public IEditor
{
public:
	using QGraphicsWidget::QGraphicsWidget;

	//���õ�ǰ�༭ֵ
	virtual void setValue(const QString&) = 0;
	//����ԭ����
	virtual void setOriginator(IOriginator* orig)override;
public:
	//����ԭ������ֵ
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

//�༭Marker
class MarkerWidetEditor : public EffectValueEditor
{
	Q_OBJECT
public:
	MarkerWidetEditor();
public slots:
	//ֵ�ı���ύ�ı���ֵ
	void onTextChanged();
	//ѡ�����ͺ󣬸ı��ӱ༭�ؼ�����
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
	//��������
	virtual void setQsData(const QString& data) 
	{
		
	}
	//��ȡ����
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
	//��������
	virtual void setQsData(const QString& data);
	//��ȡ����
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
	//��������
	virtual void setQsData(const QString& data);
	//��ȡ����
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
	//��������
	virtual void setQsData(const QString& data);
	//��ȡ����
	virtual QString toQsData();
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)override;
private:
	QLineEdit* m_editParam = nullptr;
};