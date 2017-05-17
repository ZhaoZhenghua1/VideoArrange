#include "PlayWidget.h"



PlayWidget::PlayWidget() : QWidget(nullptr, Qt::Dialog)
{
	setAttribute(Qt::WA_DontCreateNativeAncestors);
	setAttribute(Qt::WA_NativeWindow);
	setWindowFlags(windowFlags() | Qt::MSWindowsOwnDC);

	auto wid = winId();
}


PlayWidget::~PlayWidget()
{
}
