#include "EffectValueEditor.h"
#include "EffectEditor.h"
#include "StatusButton.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsTextItem>
#include <QGraphicsLinearLayout>
#include <QMouseEvent>
#include <QIntValidator>
#include <QPen>
#include <QPainter>
#include <QLineEdit>
#include <QCheckBox>
#include <QTimeEdit>
#include <QComboBox>
#include <QAbstractItemView>
#include <QTimer>

const QPen PEN(QBrush(QColor(163, 163, 163)), 1);
const QFont FONT("",12,50,false);
const QString STYLE_SHEET(R"(color: rgb(163, 163, 163);background-color: rgb(38, 38, 38);)");

//点击后创建控件，正常状态时不显示控件
template<class T>
class ClickProxyWidget : public QGraphicsProxyWidget
{
public:
	ClickProxyWidget(int width, QGraphicsItem* parent = nullptr) :QGraphicsProxyWidget(parent)
	{
		m_widget = new T;
		setAcceptHoverEvents(true);
		setGeometry(QRectF(0, 0, width, 20));
	}
	T* proxyWidget() { return m_widget; }

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		if (QGraphicsProxyWidget::widget())
		{
			QGraphicsProxyWidget::paint(painter, option, widget);
		}
		else
		{
			painter->setPen(QPen(QColor(45, 140, 235)));
			QString text = m_widget->text();
			if (text.isEmpty())
			{
				text = '*';
			}
			painter->drawText(QPointF(0, 13), text);
		}
	}
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::PointingHandCursor);
		QGraphicsProxyWidget::hoverEnterEvent(event);
	}
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::ArrowCursor);
		QGraphicsProxyWidget::hoverLeaveEvent(event);
	}
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		QGraphicsProxyWidget::mousePressEvent(event);
		setWidget(m_widget);
		QGraphicsProxyWidget::setPos(m_pos);
	}
	virtual void focusOutEvent(QFocusEvent *event)
	{
		QGraphicsProxyWidget::focusOutEvent(event);
		setWidget(nullptr);
		QGraphicsProxyWidget::setPos(m_pos);
		setAcceptHoverEvents(true);
	}
	void setPos(qreal x, qreal y)
	{
		m_pos = QPointF(x, y);
		QGraphicsProxyWidget::setPos(m_pos);
	}
private:
	T* m_widget = nullptr;
	QPointF m_pos;
};

//下拉框隐藏后同时隐藏控件
class ProxyComboBox : public QComboBox
{
public:
	typedef std::function<void(void)> ComboBoxCallBack;
	ProxyComboBox(const ComboBoxCallBack& callback):m_callBack(callback){}
protected:
	virtual void hidePopup() override
	{
		QComboBox::hidePopup();
		if (m_callBack)
		{
			m_callBack();
		}
	}
private:
	ComboBoxCallBack m_callBack;
};

//ClickProxyWidget 针对ProxyComboBox的偏特化
template<>
class ClickProxyWidget<ProxyComboBox> : public QGraphicsProxyWidget
{

public:
	ClickProxyWidget(int width, QGraphicsItem* parent = nullptr) :QGraphicsProxyWidget(parent)
	{
		m_widget = new ProxyComboBox([this]() {onCurrentChanged();});
//		Helper* pHelper = new Helper(m_widget, [this]() {onCurrentChanged();});
		setAcceptHoverEvents(true);
		setGeometry(QRectF(0, 0, width, 20));
	}
	QComboBox* proxyWidget() { return m_widget; }

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		if (QGraphicsProxyWidget::widget())
		{
			QGraphicsProxyWidget::paint(painter, option, widget);
		}
		else
		{
			painter->setPen(QPen(QColor(45, 140, 235)));
			QString text = m_widget->currentText();
			if (text.isEmpty())
			{
				text = '*';
			}
			painter->drawText(QPointF(0, 13), text);
		}
	}
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::PointingHandCursor);
		QGraphicsProxyWidget::hoverEnterEvent(event);
	}
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
	{
		setCursor(Qt::ArrowCursor);
		QGraphicsProxyWidget::hoverLeaveEvent(event);
	}
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		QGraphicsProxyWidget::mousePressEvent(event);
		if (!QGraphicsProxyWidget::widget())
		{
			setWidget(m_widget);
			QGraphicsProxyWidget::setPos(m_pos);
		}
	}
	
	virtual void focusOutEvent(QFocusEvent *event)
	{
		QGraphicsProxyWidget::focusOutEvent(event);
		if (!m_widget->view()->isVisible())
		{
			setWidget(nullptr);
			QGraphicsProxyWidget::setPos(m_pos);
			setAcceptHoverEvents(true);
		}
	}
	void setPos(qreal x, qreal y)
	{
		m_pos = QPointF(x, y);
		QGraphicsProxyWidget::setPos(m_pos);
	}
	void onCurrentChanged()
	{
		QTimer::singleShot(0, [this]() 
		{
			setWidget(nullptr);
			QGraphicsProxyWidget::setPos(m_pos);
			setAcceptHoverEvents(true);
		});
	}
private:
	QComboBox* m_widget = nullptr;
	QPointF m_pos;
};

void EffectValueEditor::setOriginator(IOriginator* orig)
{
	IEditor::setOriginator(orig);

	if (orig)
	{
		setValue(orig->toQsData());
	}
	else
	{
		setValue("");
	}
}

void EffectValueEditor::setOrigData(const QString& value)
{
	if (m_orig)
	{
		m_orig->setQsData(value);
	}
}

TransparencyWidgetEditor::TransparencyWidgetEditor()
{
	auto pw = new ClickProxyWidget<QLineEdit>(50, this);
	m_edit = pw->proxyWidget();
	m_edit->setFixedSize(50, 20);
	QIntValidator* validator = new QIntValidator(0, 100);
	m_edit->setValidator(validator);
	connect(m_edit, &QLineEdit::editingFinished, this, &TransparencyWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->proxyWidget();

	m_editTime->setDisplayFormat("hh:mm:s.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &TransparencyWidgetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void TransparencyWidgetEditor::onTextChanged()
{
	QString data = QString("%1%;%2").
		arg(m_edit->text()).
		arg(m_editTime->time().msecsSinceStartOfDay());
	setOrigData(data);
}

void TransparencyWidgetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		datas = QStringList({ "","" });
	}
	QString valueSet = datas[0];
	valueSet.remove('%');
	const QSignalBlocker blocker(m_edit);
	m_edit->setText(valueSet);

	const QSignalBlocker blocker2(m_editTime);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	update();
}

void TransparencyWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   透明度:          %"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

PositionWidgetEditor::PositionWidgetEditor()
{
	auto pwx = new ClickProxyWidget<QLineEdit>(50, this);
	auto pwy = new ClickProxyWidget<QLineEdit>(50, this);
	m_editX = pwx->proxyWidget();
	QIntValidator* validator = new QIntValidator(-999999, 999999);
	m_editX->setValidator(validator);
	m_editX->setFixedSize(50, 20);
	connect(m_editX, &QLineEdit::editingFinished, this, &PositionWidgetEditor::onTextChanged);
	m_editY = pwy->proxyWidget();
	QIntValidator* validator2 = new QIntValidator(-999999, 999999);
	m_editY->setValidator(validator2);
	m_editY->setFixedSize(50, 20);
	connect(m_editY, &QLineEdit::editingFinished, this, &PositionWidgetEditor::onTextChanged);
	pwx->setPos(65, 2);
	pwy->setPos(153, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->proxyWidget();
	m_editTime->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &PositionWidgetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void PositionWidgetEditor::onTextChanged()
{
	QString data = QString("%1,%2;%3").
		arg(m_editX->text()).
		arg(m_editY->text()).
		arg(m_editTime->time().msecsSinceStartOfDay());
	setOrigData(data);
}

void PositionWidgetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		datas = QStringList({ "","" });
	}

	QStringList strlst = datas[0].split(',');
	if (strlst.size() != 2)
	{
		strlst = QStringList({ "","" });
	}
	const QSignalBlocker blocker(m_editX);
	const QSignalBlocker blocker2(m_editY);
	m_editX->setText(strlst.at(0));
	m_editY->setText(strlst.at(1));
	const QSignalBlocker blocker3(m_editTime);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	update();
}

void PositionWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   位置  X:            Y:"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

RotateWidgetEditor::RotateWidgetEditor()
{
	auto pw = new ClickProxyWidget<QLineEdit>(50, this);
	m_edit = pw->proxyWidget();
	QIntValidator* validator = new QIntValidator(-360, 360);
	m_edit->setValidator(validator);
	m_edit->setFixedSize(50, 20);
	connect(m_edit, &QLineEdit::editingFinished, this, &RotateWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->proxyWidget();
	m_editTime->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &RotateWidgetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void RotateWidgetEditor::onTextChanged()
{
	QString data = QString("%1;%2").
		arg(m_edit->text()).
		arg(m_editTime->time().msecsSinceStartOfDay());
	setOrigData(data);
}

void RotateWidgetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		datas = QStringList({ "","" });
	}
	const QSignalBlocker blocker0(m_edit);
	m_edit->setText(datas[0]);
	const QSignalBlocker blocker1(m_editTime);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	update();
}

void RotateWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   旋转  Z:        °"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

ScailWidgetEditor::ScailWidgetEditor()
{
	auto pwx = new ClickProxyWidget<QLineEdit>(35, this);
	m_editX = pwx->proxyWidget();
	QIntValidator* validator = new QIntValidator(0, 99999);
	m_editX->setValidator(validator);
	m_editX->setFixedSize(35, 20);
	connect(m_editX, &QLineEdit::editingFinished, this, &ScailWidgetEditor::onTextChanged);

	auto pwy = new ClickProxyWidget<QLineEdit>(35, this);
	m_editY = pwy->proxyWidget();
	QIntValidator* validator2 = new QIntValidator(0, 99999);
	m_editY->setValidator(validator2);
	m_editY->setFixedSize(35, 20);
	connect(m_editY, &QLineEdit::editingFinished, this, &ScailWidgetEditor::onTextChanged);

	pwx->setPos(65, 2);
	pwy->setPos(135, 2);

	auto pwc = new QGraphicsProxyWidget(this);
	m_checkBox = new QCheckBox;
	m_checkBox->setChecked(true);
	connect(m_checkBox, &QCheckBox::stateChanged, this, &ScailWidgetEditor::onStateChanged);
	pwc->setWidget(m_checkBox);
	pwc->setPos(185, 6);


	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->proxyWidget();
	m_editTime->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &ScailWidgetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void ScailWidgetEditor::onTextChanged()
{
	if ((sender() == m_editX || sender() == m_editY )&& m_bXequalY)
	{
		QString value = (sender() == m_editX) ? m_editX->text() : m_editY->text();
		const QSignalBlocker blocker(m_editX);
		const QSignalBlocker blocker2(m_editY);
		m_editX->setText(value);
		m_editY->setText(value);
	}
	QString data = QString("%1,%2;%3").
		arg(m_editX->text()).
		arg(m_editY->text()).
		arg(m_editTime->time().msecsSinceStartOfDay());
	setOrigData(data);
}

void ScailWidgetEditor::onStateChanged()
{
	m_bXequalY = m_checkBox->checkState();
}

void ScailWidgetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		datas = QStringList({ "","" });
	}

	QStringList strlst = datas[0].split(',');
	for (QString& str : strlst)
	{
		str.remove('%');
	}
	if (strlst.size() != 2)
	{
		strlst = QStringList({ "","" });
	}

	const QSignalBlocker blocker(m_editX);
	const QSignalBlocker blocker2(m_editY);
	m_editX->setText(strlst.at(0));
	m_editY->setText(strlst.at(1));
	const QSignalBlocker blocker3(m_editTime);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	update();
}

void ScailWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   缩放  X:      %  Y:       %    X=Y"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

VoiceWidgetEditor::VoiceWidgetEditor()
{
	auto pw = new ClickProxyWidget<QLineEdit>(50, this);
	m_edit = pw->proxyWidget();
	QIntValidator* validator = new QIntValidator(0, 100);
	m_edit->setValidator(validator);
	m_edit->setFixedSize(50, 20);
	connect(m_edit, &QLineEdit::editingFinished, this, &VoiceWidgetEditor::onTextChanged);

	pw->setPos(50, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->proxyWidget();
	m_editTime->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &VoiceWidgetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void VoiceWidgetEditor::onTextChanged()
{
	QString data = QString("%1%;%2").
		arg(m_edit->text()).
		arg(m_editTime->time().msecsSinceStartOfDay());

	setOrigData(data);
}

void VoiceWidgetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 2)
	{
		datas = QStringList({ "","" });
	}

	const QSignalBlocker blocker0(m_edit);
	QString valueSet = datas[0];
	valueSet.remove('%');
	m_edit->setText(valueSet);

	const QSignalBlocker blocker1(m_editTime);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	update();
}

void VoiceWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   音量:         %"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

MediaItemWidgetEditor::MediaItemWidgetEditor()
{
	auto proxyWidgetTimeStart = new ClickProxyWidget<QTimeEdit>(100, this);
	auto proxyWidgetTimeLen = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTimeLen = proxyWidgetTimeLen->proxyWidget();
	m_editTimeLen->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTimeLen, &QTimeEdit::editingFinished, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeLen->setFixedSize(100, 20);
	m_editTimeStart = proxyWidgetTimeStart->proxyWidget();
	m_editTimeStart->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTimeStart, &QTimeEdit::editingFinished, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeStart->setFixedSize(100, 20);

	auto pwOffset = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editOffset = pwOffset->proxyWidget();
	m_editOffset->setDisplayFormat("hh:mm:ss.zzz");
	m_editOffset->setFixedSize(100, 20);
	connect(m_editOffset, &QTimeEdit::editingFinished, this, &MediaItemWidgetEditor::onValueChanged);

	proxyWidgetTimeStart->setPos(73, 3);
	proxyWidgetTimeLen->setPos(73, 25);
	pwOffset->setPos(73, 47);
}

void MediaItemWidgetEditor::onValueChanged()
{
	if (m_orig)
	{
		QString data = QString("%1;%2;%3").
			arg(m_editTimeStart->time().msecsSinceStartOfDay()).
			arg(m_editTimeLen->time().msecsSinceStartOfDay()).
			arg(m_editOffset->time().msecsSinceStartOfDay());
		m_orig->setQsData(data);
	}
}

void MediaItemWidgetEditor::setValue(const QString& data)
{
	QStringList datas = data.split(';');
	if (datas.size() != 3)
	{
		datas = QStringList({ "","","" });
	}

	const QSignalBlocker blocker1(m_editTimeStart);
	const QSignalBlocker blocker2(m_editTimeLen);
	const QSignalBlocker blocker3(m_editOffset);
	m_editTimeStart->setTime(QTime::fromMSecsSinceStartOfDay(datas[0].toInt()));
	m_editTimeLen->setTime(QTime::fromMSecsSinceStartOfDay(datas[1].toInt()));
	m_editOffset->setTime(QTime::fromMSecsSinceStartOfDay(datas[2].toInt()));
	for (QGraphicsItem* item : childItems())
	{
		item->update();
	}

}

void MediaItemWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit(     "       时间:                "));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间长度:                "));
	painter->drawText(QPointF(0, 15 + 44), QString::fromLocal8Bit("   开始时间:                "));
}

MarkerWidetEditor::MarkerWidetEditor()
{
	//创建时间设置
	auto proxyWidgetTimeStart = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = proxyWidgetTimeStart->proxyWidget();
	m_editTime->setDisplayFormat("hh:mm:ss.zzz");
	connect(m_editTime, &QTimeEdit::editingFinished, this, &MarkerWidetEditor::onTextChanged);
	m_editTime->setFixedSize(100, 20);
	proxyWidgetTimeStart->setPos(55, 2);


	//创建下拉类型选择
	auto pw = new ClickProxyWidget<ProxyComboBox>(50, this);
	m_comboType = pw->proxyWidget();
	m_comboType->addItems({ "Position","Pause", "Jump", "Trigger" });
	m_comboType->setFixedSize(100, 20);
	connect(m_comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeEditor()));
	connect(m_comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(onTextChanged()));
	pw->setPos(55, 26);

	//创建参数输入
//	auto pwt = new ClickProxyWidget<QLineEdit>(100, this);
//	m_editParam = pwt->proxyWidget();
//	connect(m_editParam, &QLineEdit::editingFinished, this, &MarkerWidetEditor::onTextChanged);
//	m_editParam->setFixedSize(100, 20);
//	pwt->setPos(55, 50);
}

void MarkerWidetEditor::onChangeEditor()
{
	createMarkerEditor(m_comboType->currentText());
	dynamic_cast<QGraphicsItem*>(m_paraEditor)->update();
	update();
}

void MarkerWidetEditor::onTextChanged()
{
	QString data = QString("%1;%2;%3").
		arg(m_editTime->time().msecsSinceStartOfDay()).
		arg(m_comboType->currentText()).
		arg(m_paraEditor ? m_paraEditor->toQsData() : "");

	setOrigData(data);
}

void MarkerWidetEditor::setValue(const QString& value)
{
	QStringList datas = value.split(';');
	if (datas.size() != 3)
	{
		datas = QStringList({ "","", ""});
	}

	const QSignalBlocker blocker1(m_editTime);
	const QSignalBlocker blocker2(m_comboType);
	m_editTime->setTime(QTime::fromMSecsSinceStartOfDay(datas[0].toInt()));
	m_comboType->setCurrentText(datas[1]);
	createMarkerEditor(datas[1]);
	if (m_paraEditor)
	{
		m_paraEditor->setQsData(datas[2]);
	}
	for (QGraphicsItem* item : childItems())
	{
		item->update();
	}
}

void MarkerWidetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit(     "    时间:"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit(     "控制类型:"));
//	painter->drawText(QPointF(0, 15 + 22 + 24), QString::fromLocal8Bit("    参数: "));
}

void MarkerWidetEditor::createMarkerEditor(const QString& type)
{
	delete m_paraEditor;
	m_paraEditor = nullptr;
	// "Position","Pause", "Jump", "Trigger"
	if (type == "Position")
	{
		PostionMarkerEditor* pe = new PostionMarkerEditor(this);
		connect(pe, &PostionMarkerEditor::sigChanged, this, &MarkerWidetEditor::onTextChanged);
		pe->setPos(0, 60);
		m_paraEditor = pe;
	}
	else if (type == "Pause")
	{
		PauseMarkerEditor* pe = new PauseMarkerEditor(this);
		connect(pe, &PauseMarkerEditor::sigChanged, this, &MarkerWidetEditor::onTextChanged);
		pe->setPos(0, 60);
		m_paraEditor = pe;
	}
	else if (type =="Jump")
	{
		JumpMarkerEditor* pe = new JumpMarkerEditor(this);
		connect(pe, &JumpMarkerEditor::sigChanged, this, &MarkerWidetEditor::onTextChanged);
		pe->setPos(0, 60);
		m_paraEditor = pe;
	}
	else if (type == "Trigger")
	{
		TriggerMarkerEditor* pe = new TriggerMarkerEditor(this);
		connect(pe, &TriggerMarkerEditor::sigChanged, this, &MarkerWidetEditor::onTextChanged);
		pe->setPos(0, 60);
		m_paraEditor = pe;
	}
}

PauseMarkerEditor::PauseMarkerEditor(QGraphicsWidget* parent /*= nullptr*/) :QGraphicsWidget(parent)
{
	setPreferredSize(100, 40);
}

PostionMarkerEditor::PostionMarkerEditor(QGraphicsWidget* parent /*= nullptr*/) : QGraphicsWidget(parent)
{
	//创建参数输入
	auto pwt = new ClickProxyWidget<QLineEdit>(100, this);
	m_editParam = pwt->proxyWidget();
	connect(m_editParam, &QLineEdit::editingFinished, this, &PostionMarkerEditor::sigChanged);
	m_editParam->setFixedSize(100, 20);
	pwt->setPos(55, -13);

	setPreferredSize(100, 40);
}

void PostionMarkerEditor::setQsData(const QString& data)
{
	const QSignalBlocker blocker3(m_editParam);
	m_editParam->setText(data);
}

QString PostionMarkerEditor::toQsData()
{
	return m_editParam->text();
}

void PostionMarkerEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 0), QString::fromLocal8Bit("    名称: "));
}

JumpMarkerEditor::JumpMarkerEditor(QGraphicsWidget* parent /*= nullptr*/) : QGraphicsWidget(parent)
{
	//创建参数输入
	auto pwt = new ClickProxyWidget<QLineEdit>(100, this);
	m_editParam = pwt->proxyWidget();
	connect(m_editParam, &QLineEdit::editingFinished, this, &JumpMarkerEditor::sigChanged);
	m_editParam->setFixedSize(100, 20);
	pwt->setPos(55, -13);

	setPreferredSize(100, 40);
}

void JumpMarkerEditor::setQsData(const QString& data)
{
	const QSignalBlocker blocker3(m_editParam);
	m_editParam->setText(data);
}

QString JumpMarkerEditor::toQsData()
{
	return m_editParam->text();
}

void JumpMarkerEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);                                 //"    名称: "
	painter->drawText(QPointF(0, 0), QString::fromLocal8Bit("  位置名:"));
}

TriggerMarkerEditor::TriggerMarkerEditor(QGraphicsWidget* parent /*= nullptr*/) : QGraphicsWidget(parent)
{
	//创建参数输入
	auto pwt = new ClickProxyWidget<QLineEdit>(100, this);
	m_editParam = pwt->proxyWidget();
	connect(m_editParam, &QLineEdit::editingFinished, this, &TriggerMarkerEditor::sigChanged);
	m_editParam->setFixedSize(100, 20);
	pwt->setPos(55, -13);

	setPreferredSize(100, 40);
}

void TriggerMarkerEditor::setQsData(const QString& data)
{
	const QSignalBlocker blocker3(m_editParam);
	m_editParam->setText(data);
}

QString TriggerMarkerEditor::toQsData()
{
	return m_editParam->text();
}

void TriggerMarkerEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);                                 //"    名称: "
	painter->drawText(QPointF(0, 0), QString::fromLocal8Bit("触发器名:"));
}
