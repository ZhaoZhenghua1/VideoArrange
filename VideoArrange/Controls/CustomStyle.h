#pragma once
#include <QCommonStyle>
#include <QProxyStyle>

class CustomStyle : public QProxyStyle
{
public:
	CustomStyle();
	~CustomStyle();
};

