#pragma once
#include <QCommonStyle>
#include <QProxyStyle>

class CustomStyle : public QProxyStyle
{
public:
	CustomStyle();
	~CustomStyle();

protected:
	virtual int pixelMetric(QStyle::PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr)const override;
	virtual QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget = Q_NULLPTR) const override;
	virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;
};

