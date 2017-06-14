#include "CustomStyle.h"
#include <QStyleOptionSlider>
#include <QPainter>


CustomStyle::CustomStyle()
{
}


CustomStyle::~CustomStyle()
{
}

int CustomStyle::pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt /*= nullptr*/, const QWidget* widget /*= nullptr*/) const
{
	int ret;
	switch (m)
	{
	case PM_ScrollBarSliderMin:
		ret = 32;
		break;
	default:
		ret = QCommonStyle::pixelMetric(m, opt, widget);
		break;
	}
	return ret;
}

QRect CustomStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget /*= Q_NULLPTR*/) const
{
	QRect ret;
	switch (cc)
	{
	case CC_ScrollBar:
		if (const QStyleOptionSlider *scrollbar = qstyleoption_cast<const QStyleOptionSlider *>(opt)) {
			const QRect scrollBarRect = scrollbar->rect;
			int sbextent = 0;
			if (!proxy()->styleHint(SH_ScrollBar_Transient, scrollbar, widget))
				sbextent = proxy()->pixelMetric(PM_ScrollBarExtent, scrollbar, widget);
			int maxlen = ((scrollbar->orientation == Qt::Horizontal) ? scrollBarRect.width() : scrollBarRect.height());
			int sliderlen;

			// calculate slider length
			if (scrollbar->maximum != scrollbar->minimum) {
				uint range = scrollbar->maximum - scrollbar->minimum;
				sliderlen = (qint64(scrollbar->pageStep) * maxlen) / (range + scrollbar->pageStep);
				int slidermin = proxy()->pixelMetric(PM_ScrollBarSliderMin, scrollbar, widget);

				if (sliderlen < slidermin || range > INT_MAX / 2)
					sliderlen = slidermin;
				if (sliderlen > maxlen)
					sliderlen = maxlen;
			}
			else {
				sliderlen = maxlen;
			}

			int sliderstart = sliderPositionFromValue(scrollbar->minimum,
				scrollbar->maximum,
				scrollbar->sliderPosition,
				maxlen - sliderlen,
				scrollbar->upsideDown);

			switch (sc) {
			case SC_ScrollBarSubLine:            // top/left button
				if (scrollbar->orientation == Qt::Horizontal) {
					int buttonWidth = qMin(scrollBarRect.width() / 2, sbextent);
					ret.setRect(0, 0, buttonWidth, scrollBarRect.height());
				}
				else {
					int buttonHeight = qMin(scrollBarRect.height() / 2, sbextent);
					ret.setRect(0, 0, scrollBarRect.width(), buttonHeight);
				}
				break;
			case SC_ScrollBarAddLine:            // bottom/right button
				if (scrollbar->orientation == Qt::Horizontal) {
					int buttonWidth = qMin(scrollBarRect.width() / 2, sbextent);
					ret.setRect(scrollBarRect.width() - buttonWidth, 0, buttonWidth, scrollBarRect.height());
				}
				else {
					int buttonHeight = qMin(scrollBarRect.height() / 2, sbextent);
					ret.setRect(0, scrollBarRect.height() - buttonHeight, scrollBarRect.width(), buttonHeight);
				}
				break;
			case SC_ScrollBarSubPage:            // between top/left button and slider
				if (scrollbar->orientation == Qt::Horizontal)
					ret.setRect(sbextent, 0, sliderstart - sbextent, scrollBarRect.height());
				else
					ret.setRect(0, sbextent, scrollBarRect.width(), sliderstart - sbextent);
				break;
			case SC_ScrollBarAddPage:            // between bottom/right button and slider
				if (scrollbar->orientation == Qt::Horizontal)
					ret.setRect(sliderstart + sliderlen, 0,
						maxlen - sliderstart - sliderlen + sbextent, scrollBarRect.height());
				else
					ret.setRect(0, sliderstart + sliderlen, scrollBarRect.width(),
						maxlen - sliderstart - sliderlen + sbextent);
				break;
			case SC_ScrollBarGroove:
				if (scrollbar->orientation == Qt::Horizontal)
					ret.setRect(0, 0, scrollBarRect.width(), scrollBarRect.height());
				else
					ret.setRect(0, 0, scrollBarRect.width(), scrollBarRect.height());
				break;
			case SC_ScrollBarSlider:
				if (scrollbar->orientation == Qt::Horizontal)
					ret.setRect(sliderstart, 0, sliderlen, scrollBarRect.height());
				else
					ret.setRect(0, sliderstart, scrollBarRect.width(), sliderlen);
				break;
			default:
				break;
			}
			ret = visualRect(scrollbar->direction, scrollBarRect, ret);
		}
		break;
	default:
		return QCommonStyle::subControlRect(cc, opt, sc, widget);
	}
	return ret;
}

void CustomStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
	if (control == QStyle::CC_ScrollBar)
	{
		painter->save();
		State flags = option->state;
		painter->setRenderHint(QPainter::Antialiasing);

		QRect rect(0, 0, widget->width(), widget->height());
		painter->fillRect(rect, QColor(35, 35, 35));
		painter->setPen(QPen(QBrush(QColor(49, 49, 49)), 1));//
		painter->setBrush(QColor(29, 29, 29));
		qreal radius = flags & State_Horizontal ? rect.height() / 2 : rect.width() / 2;
		painter->drawRoundedRect(rect, radius, radius);

		QRect rectSlider = proxy()->subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
		rectSlider.adjust(1, 1, -1, -1);
		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(49, 49, 49));
		painter->drawRoundedRect(rectSlider, radius, radius);
		painter->restore();
	}
	else
	{
		QCommonStyle::drawComplexControl(control, option, painter, widget);
	}
}
