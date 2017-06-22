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

const QPen PEN(QBrush(QColor(163, 163, 163)), 1);
const QFont FONT("",12,50,false);

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
	QGraphicsProxyWidget* pw = new NoHoverProxyWidget(this);
	m_edit = new QLineEdit;
	m_edit->setFixedSize(50, 20);
	pw->setWidget(m_edit);
	QIntValidator* validator = new QIntValidator(0, 100);
	m_edit->setValidator(validator);
	connect(m_edit, &QLineEdit::textChanged, this, &TransparencyWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	QGraphicsProxyWidget* pwt = new NoHoverProxyWidget(this);
	m_editTime = new QLineEdit;
	connect(m_editTime, &QLineEdit::textChanged, this, &TransparencyWidgetEditor::onTextChanged);
	pwt->setWidget(m_editTime);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void TransparencyWidgetEditor::onTextChanged()
{
	setOrigData(m_edit->text() + (QString('%') + ';' + m_editTime->text()));
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
	m_editTime->setText(datas[1]);
}

void TransparencyWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   透明度:          %"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

PositionWidgetEditor::PositionWidgetEditor()
{
	QGraphicsProxyWidget* pwx = new NoHoverProxyWidget(this);
	QGraphicsProxyWidget* pwy = new NoHoverProxyWidget(this);
	m_editX = new QLineEdit;
	m_editX->setFixedSize(50, 20);
	pwx->setWidget(m_editX);
	connect(m_editX, &QLineEdit::textChanged, this, &PositionWidgetEditor::onTextChanged);
	m_editY = new QLineEdit;
	m_editY->setFixedSize(50, 20);
	pwy->setWidget(m_editY);
	connect(m_editY, &QLineEdit::textChanged, this, &PositionWidgetEditor::onTextChanged);
	pwx->setPos(65, 2);
	pwy->setPos(153, 2);

	QGraphicsProxyWidget* pwt = new NoHoverProxyWidget(this);
	m_editTime = new QLineEdit;
	connect(m_editTime, &QLineEdit::textChanged, this, &PositionWidgetEditor::onTextChanged);
	pwt->setWidget(m_editTime);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void PositionWidgetEditor::onTextChanged()
{
	QString adjustValue = m_editX->text() + ',' + m_editY->text() + ';' + m_editTime->text();
	setOrigData(adjustValue);
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
	m_editTime->setText(datas[1]);
}

void PositionWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   位置  X:            Y:"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

RotateWidgetEditor::RotateWidgetEditor()
{
	QGraphicsProxyWidget* pw = new NoHoverProxyWidget(this);
	m_edit = new QLineEdit;
	m_edit->setFixedSize(50, 20);
	pw->setWidget(m_edit);
	connect(m_edit, &QLineEdit::textChanged, this, &RotateWidgetEditor::onTextChanged);
	pw->setPos(65, 2);

	QGraphicsProxyWidget* pwt = new NoHoverProxyWidget(this);
	m_editTime = new QLineEdit;
	connect(m_editTime, &QLineEdit::textChanged, this, &RotateWidgetEditor::onTextChanged);
	pwt->setWidget(m_editTime);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void RotateWidgetEditor::onTextChanged()
{
	setOrigData(m_edit->text() + ';' + m_editTime->text());
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
	m_editTime->setText(datas[1]);
}

void RotateWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   旋转  Z:        °"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

ScailWidgetEditor::ScailWidgetEditor()
{
	QGraphicsProxyWidget* pwx = new NoHoverProxyWidget(this);
	m_editX = new QLineEdit;
	m_editX->setFixedSize(35, 20);
	pwx->setWidget(m_editX);
	connect(m_editX, &QLineEdit::textChanged, this, &ScailWidgetEditor::onTextChanged);

	QGraphicsProxyWidget* pwy = new NoHoverProxyWidget(this);
	m_editY = new QLineEdit;
	m_editY->setFixedSize(35, 20);
	pwy->setWidget(m_editY);
	connect(m_editY, &QLineEdit::textChanged, this, &ScailWidgetEditor::onTextChanged);

	pwx->setPos(65, 2);
	pwy->setPos(135, 2);

	QGraphicsProxyWidget* pwc = new NoHoverProxyWidget(this);
	m_checkBox = new QCheckBox;
	m_checkBox->setChecked(true);
	connect(m_checkBox, &QCheckBox::stateChanged, this, &ScailWidgetEditor::onStateChanged);
	pwc->setWidget(m_checkBox);
	pwc->setPos(185, 6);


	QGraphicsProxyWidget* pwt = new NoHoverProxyWidget(this);
	m_editTime = new QLineEdit;
	connect(m_editTime, &QLineEdit::textChanged, this, &ScailWidgetEditor::onTextChanged);
	pwt->setWidget(m_editTime);
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
	QString adjustValue = m_editX->text() + ',' + m_editY->text() + ';' + m_editTime->text();
	setOrigData(adjustValue);
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
	m_editTime->setText(datas[1]);
}

void ScailWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   缩放  X:      %  Y:       %    X=Y"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

VoiceWidgetEditor::VoiceWidgetEditor()
{
	QGraphicsProxyWidget* pw = new NoHoverProxyWidget(this);
	m_edit = new QLineEdit;
	m_edit->setFixedSize(50, 20);
	pw->setWidget(m_edit);
	connect(m_edit, &QLineEdit::textChanged, this, &VoiceWidgetEditor::onTextChanged);

	pw->setPos(50, 2);

	QGraphicsProxyWidget* pwt = new NoHoverProxyWidget(this);
	m_editTime = new QLineEdit;
	connect(m_editTime, &QLineEdit::textChanged, this, &VoiceWidgetEditor::onTextChanged);
	pwt->setWidget(m_editTime);
	m_editTime->setFixedSize(100, 20);
	pwt->setPos(50, 26);
}

void VoiceWidgetEditor::onTextChanged()
{
	setOrigData(m_edit->text() + '%' + ';' + m_editTime->text());
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
	m_editTime->setText(datas[1]);
}

void VoiceWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit("   音量:         %"));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间: "));
}

MediaItemWidgetEditor::MediaItemWidgetEditor()
{
	QGraphicsProxyWidget* proxyWidgetTimeStart = new NoHoverProxyWidget(this);
	QGraphicsProxyWidget* proxyWidgetTimeLen = new NoHoverProxyWidget(this);
	m_editTimeLen = new QLineEdit;
	connect(m_editTimeLen, &QLineEdit::textChanged, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeLen->setFixedSize(100, 20);
	m_editTimeStart = new QLineEdit;
	connect(m_editTimeStart, &QLineEdit::textChanged, this, &MediaItemWidgetEditor::onValueChanged);
	m_editTimeStart->setFixedSize(100, 20);
	proxyWidgetTimeStart->setWidget(m_editTimeStart);
	proxyWidgetTimeLen->setWidget(m_editTimeLen);

	QGraphicsProxyWidget* pwOffset = new NoHoverProxyWidget(this);
	m_editOffset = new QLineEdit;
	m_editOffset->setFixedSize(100, 20);
	connect(m_editOffset, &QLineEdit::textChanged, this, &MediaItemWidgetEditor::onValueChanged);
	pwOffset->setWidget(m_editOffset);

	proxyWidgetTimeStart->setPos(73, 3);
	proxyWidgetTimeLen->setPos(73, 25);
	pwOffset->setPos(73, 47);
}

void MediaItemWidgetEditor::onValueChanged(const QString& value)
{
	if (m_orig)
	{
		m_orig->setQsData(m_editTimeStart->text() + ';' + m_editTimeLen->text() + ';' + m_editOffset->text());
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
	m_editTimeStart->setText(datas[0]);
	m_editTimeLen->setText(datas[1]);
	m_editOffset->setText(datas[2]);

}

void MediaItemWidgetEditor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = Q_NULLPTR */)
{
	painter->setPen(PEN);
	painter->drawText(QPointF(0, 15), QString::fromLocal8Bit(     "       时间:                "));
	painter->drawText(QPointF(0, 15 + 22), QString::fromLocal8Bit("   时间长度:                "));
	painter->drawText(QPointF(0, 15 + 44), QString::fromLocal8Bit("   开始时间:                "));
}
