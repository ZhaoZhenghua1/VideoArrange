#include "TimeLineScrollBar.h"

#include <QCommonStyle>
#include <QStyleOptionSlider> 
#include <QPainter>
#include <QMouseEvent>
class ScrollBarStyle : public QCommonStyle
{
public:
	ScrollBarStyle(TimeLineScrollBar* parentBar):m_scrollBar(parentBar){}
protected:
	virtual int pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr)const override
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
	virtual QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget = Q_NULLPTR) const
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
					//比率
					qreal k = (m_scrollBar->maxSceneLength() - m_scrollBar->pageStep()) * 1.0 / (maxlen - slidermin);
					sliderlen = maxlen - (m_scrollBar->maximum() - m_scrollBar->pageStep()) / k;

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
						//缩小按钮大小为1像素，从而不影响滚动条
						int buttonWidth = 1;//qMin(scrollBarRect.width() / 2, sbextent);
						ret.setRect(0, 0, buttonWidth, scrollBarRect.height());
					}
					else {
						int buttonHeight = qMin(scrollBarRect.height() / 2, sbextent);
						ret.setRect(0, 0, scrollBarRect.width(), buttonHeight);
					}
					break;
				case SC_ScrollBarAddLine:            // bottom/right button
					if (scrollbar->orientation == Qt::Horizontal) {
						//缩小按钮大小为1像素，从而不影响滚动条
						int buttonWidth = 1;//qMin(scrollBarRect.width() / 2, sbextent);
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

	void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
	{
		painter->save();
		if (control == QStyle::CC_ScrollBar)
		{
			State flags = option->state;
			painter->setRenderHint(QPainter::Antialiasing);

			QRect rect(0, 0, widget->width(), widget->height());
			painter->fillRect(rect, QColor(35, 35, 35));
			painter->setPen(QPen(QBrush(QColor(49, 49, 49)), 1));
			painter->setBrush(QColor(29,29,29));
			qreal radius = flags & State_Horizontal ? rect.height() / 2 : rect.width() / 2;
			painter->drawRoundedRect(rect, radius, radius);

			QRect rectSlider = proxy()->subControlRect(CC_ScrollBar, option, SC_ScrollBarSlider, widget);
			rectSlider.adjust(1, 1, -1, -1);

			radius = flags & State_Horizontal ? rectSlider.height() / 2 : rectSlider.width() / 2;
			painter->setPen(Qt::NoPen);
			painter->setBrush(QColor(49, 49, 49));
			painter->drawRoundedRect(rectSlider, radius, radius);

			
			QRect rectFirst(rectSlider.topLeft(), QSize(radius * 2, radius * 2));
			QRect rectSecond(rectSlider.bottomRight() -= QPoint(radius * 2, radius * 2), QSize(radius * 2, radius * 2));
			//if mouse pressed
			if (m_scrollBar->mousePressBtn() != TimeLineScrollBar::NotPressed)
			{
				if (m_scrollBar->mousePressBtn() == TimeLineScrollBar::LBtn)
				{
					painter->setPen(QPen(QBrush(QColor(168, 168, 168)), 2));
				}
				else
				{
					painter->setPen(QPen(QBrush(QColor(138, 138, 138)), 2));
				}
				painter->drawEllipse(rectFirst.adjusted(2, 2, -2, -2));

				if (m_scrollBar->mousePressBtn() == TimeLineScrollBar::RBtn)
				{
					painter->setPen(QPen(QBrush(QColor(168, 168, 168)), 2));
				}
				else
				{
					painter->setPen(QPen(QBrush(QColor(138, 138, 138)), 2));
				}
				painter->drawEllipse(rectSecond.adjusted(2, 2, -2, -2));
			}
			//if hover
			else
			{
				QRect rects2[2] = { rectFirst, rectSecond };
				for (QRect rect : rects2)
				{
					if (rect.contains(widget->mapFromGlobal(QCursor::pos())))
					{
						painter->setPen(QPen(QBrush(QColor(168, 168, 168)), 2));
					}
					else
					{
						painter->setPen(QPen(QBrush(QColor(138, 138, 138)), 2));
					}

					painter->drawEllipse(rect.adjusted(2, 2, -2, -2));
				}
			}
		}
		else
		{
			QCommonStyle::drawComplexControl(control, option, painter, widget);
		}
		painter->restore();
	}
private:
	TimeLineScrollBar* m_scrollBar;
};

TimeLineScrollBar::TimeLineScrollBar()
{
	setStyle(new ScrollBarStyle(this));
	setMouseTracking(true);
}


TimeLineScrollBar::~TimeLineScrollBar()
{
}

void TimeLineScrollBar::setTimeLength(unsigned int ms)
{
	m_timeLen = ms;
}

unsigned int TimeLineScrollBar::maxSceneLength()
{
	return m_timeLen ;
}

void TimeLineScrollBar::paintEvent(QPaintEvent *event)
{
	QScrollBar::paintEvent(event);
}

void TimeLineScrollBar::mouseMoveEvent(QMouseEvent * event) 
{
	if (m_fPressOnButton == NotPressed)
	{
		bool bOnBtn = btnFlag(event->pos()) != WhiteSpace;
		//state changed , update
		if (m_bOnButton != bOnBtn)
		{
			update();
			m_bOnButton = bOnBtn;
		}
		setCursor(QCursor(bOnBtn ? Qt::PointingHandCursor : Qt::ArrowCursor));
	}
	
	
	if (m_fPressOnButton != NotPressed && m_fPressOnButton != WhiteSpace)
	{
		int dis = event->pos().x() - m_posPressed.x();
		m_posPressed = event->pos();
		dis *= m_fPressOnButton == LBtn ? 1 : -1;
		QStyleOptionSlider opt;
		initStyleOption(&opt);
		int slidermin = style()->pixelMetric(QStyle::PM_ScrollBarSliderMin, &opt, this);
		const QRect scrollBarRect = opt.rect;
		int maxlen = ((orientation() == Qt::Horizontal) ? scrollBarRect.width() : scrollBarRect.height());

		qreal k = (maxSceneLength() - pageStep()) * 1.0 / (maxlen - slidermin);
		emit adjustWidth(k * dis + 0.5);
		return;
	}
	QScrollBar::mouseMoveEvent(event);
}

void TimeLineScrollBar::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_fPressOnButton = btnFlag(event->pos());
		m_posPressed = event->pos();
		if (m_fPressOnButton != WhiteSpace)
		{
			update();
		}
	}
	QScrollBar::mousePressEvent(event);
}

void TimeLineScrollBar::mouseReleaseEvent(QMouseEvent * event)
{
	setCursor(QCursor(Qt::ArrowCursor));
	if (m_fPressOnButton != WhiteSpace)
	{
		update();
	}
	m_fPressOnButton = NotPressed;
	QScrollBar::mouseReleaseEvent(event);
}


TimeLineScrollBar::PressFlag TimeLineScrollBar::btnFlag(const QPoint& pos)
{
	QStyleOptionSlider opt;
	initStyleOption(&opt);
	opt.subControls = QStyle::SC_All;

	QRect rectSlider = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, this);
	qreal radius = orientation() == Qt::Horizontal ? rectSlider.height() / 2 : rectSlider.width() / 2;

	QRect rectFirst(rectSlider.topLeft(), QSize(radius * 2, radius * 2));
	QRect rectSecond(rectSlider.bottomRight() -= QPoint(radius * 2, radius * 2), QSize(radius * 2, radius * 2));
	if (rectFirst.contains(pos))
	{
		return LBtn;
	}
	else if (rectSecond.contains(pos))
	{
		return RBtn;
	}
	else
	{
		return WhiteSpace;
	}
}
