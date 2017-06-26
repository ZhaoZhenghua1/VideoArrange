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

const QPen PEN(QBrush(QColor(163, 163, 163)), 1);
const QFont FONT("",12,50,false);
const QString STYLE_SHEET(R"(color: rgb(163, 163, 163);background-color: rgb(38, 38, 38);)");

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
	T* widget() { return m_widget; }

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
	m_edit = pw->widget();
	m_edit->setFixedSize(50, 20);
	QIntValidator* validator = new QIntValidator(0, 100);
	m_edit->setValidator(validator);
	connect(m_edit, &QLineEdit::editingFinished, this, &TransparencyWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->widget();

	m_editTime->setDisplayFormat("hh:mm:ss:zzz");
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
	m_editX = pwx->widget();
	QIntValidator* validator = new QIntValidator(-999999, 999999);
	m_editX->setValidator(validator);
	m_editX->setFixedSize(50, 20);
	connect(m_editX, &QLineEdit::editingFinished, this, &PositionWidgetEditor::onTextChanged);
	m_editY = pwy->widget();
	QIntValidator* validator2 = new QIntValidator(-999999, 999999);
	m_editY->setValidator(validator2);
	m_editY->setFixedSize(50, 20);
	connect(m_editY, &QLineEdit::editingFinished, this, &PositionWidgetEditor::onTextChanged);
	pwx->setPos(65, 2);
	pwy->setPos(153, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->widget();
	m_editTime->setDisplayFormat("hh:mm:ss:zzz");
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
	m_edit = pw->widget();
	QIntValidator* validator = new QIntValidator(-360, 360);
	m_edit->setValidator(validator);
	m_edit->setFixedSize(50, 20);
	connect(m_edit, &QLineEdit::editingFinished, this, &RotateWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->widget();
	m_editTime->setDisplayFormat("hh:mm:ss:zzz");
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
	m_editX = pwx->widget();
	QIntValidator* validator = new QIntValidator(0, 99999);
	m_editX->setValidator(validator);
	m_editX->setFixedSize(35, 20);
	connect(m_editX, &QLineEdit::editingFinished, this, &ScailWidgetEditor::onTextChanged);

	auto pwy = new ClickProxyWidget<QLineEdit>(35, this);
	m_editY = pwy->widget();
	QIntValidator* validator2 = new QIntValidator(0, 99999);
	m_editY->setValidator(validator2);
	m_editY->setFixedSize(35, 20);
	connect(m_editY, &QLineEdit::editingFinished, this, &ScailWidgetEditor::onTextChanged);

	pwx->setPos(65, 2);
	pwy->setPos(135, 2);

	auto pwc = new NoHoverProxyWidget(this);
	m_checkBox = new QCheckBox;
	m_checkBox->setChecked(true);
	connect(m_checkBox, &QCheckBox::stateChanged, this, &ScailWidgetEditor::onStateChanged);
	pwc->setWidget(m_checkBox);
	pwc->setPos(185, 6);


	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->widget();
	m_editTime->setDisplayFormat("hh:mm:ss:zzz");
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
	m_edit = pw->widget();
	QIntValidator* validator = new QIntValidator(0, 100);
	m_edit->setValidator(validator);
	m_edit->setFixedSize(50, 20);
	connect(m_edit, &QLineEdit::editingFinished, this, &VoiceWidgetEditor::onTextChanged);

	pw->setPos(50, 2);

	auto pwt = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editTime = pwt->widget();
	m_editTime->setDisplayFormat("hh:mm:ss:zzz");
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
	m_editTimeLen = proxyWidgetTimeLen->widget();
	m_editTimeLen->setDisplayFormat("hh:mm:ss:zzz");
	connect(m_editTimeLen, &QTimeEdit::editingFinished, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeLen->setFixedSize(100, 20);
	m_editTimeStart = proxyWidgetTimeStart->widget();
	m_editTimeStart->setDisplayFormat("hh:mm:ss:zzz");
	connect(m_editTimeStart, &QTimeEdit::editingFinished, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeStart->setFixedSize(100, 20);

	auto pwOffset = new ClickProxyWidget<QTimeEdit>(100, this);
	m_editOffset = pwOffset->widget();
	m_editOffset->setDisplayFormat("hh:mm:ss:zzz");
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
 #include <QGraphicsScene>
 void NoHoverProxyWidget::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
 {
 	QGraphicsProxyWidget::hoverEnterEvent(event);
 	//解决闪烁问题
 	scene()->update(scene()->itemsBoundingRect());
 }
 
 void NoHoverProxyWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
 {
 	QGraphicsProxyWidget::hoverLeaveEvent(event);
 	//解决闪烁问题
 	scene()->update(scene()->itemsBoundingRect());
 }
 
 void NoHoverProxyWidget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
 {
 	QGraphicsProxyWidget::hoverMoveEvent(event);
 	//解决闪烁问题
 	scene()->update(scene()->itemsBoundingRect());
 }
 
 void NoHoverProxyWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
 {
 	QGraphicsProxyWidget::mousePressEvent(event);
 //	scene()->update();
 }
 
 void NoHoverProxyWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
 {
 	QGraphicsProxyWidget::mouseReleaseEvent(event);
 //	scene()->update();
 }
 
 void NoHoverProxyWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
 {
 	QGraphicsProxyWidget::mouseMoveEvent(event);
 //	scene()->update();
 }
 
 void NoHoverProxyWidget::focusInEvent(QFocusEvent *event)
 {
 	QGraphicsProxyWidget::focusInEvent(event);
 //	scene()->update();
 }
 
 void NoHoverProxyWidget::focusOutEvent(QFocusEvent *event)
 {
 	QGraphicsProxyWidget::focusOutEvent(event);
 //	scene()->update();
 }
 
