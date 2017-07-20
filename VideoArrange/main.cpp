
#include <QtWidgets/QApplication>


#include "uilogic/VideoEditor.h"

#include "Controls/TimeLineScrollBar.h"
#include "Controls/CustomStyle.h"
#include "Controls/StatusButton.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle(new CustomStyle);
	VideoEditor ve;
	ve.resize(1000, 600);
	ve.show();

	return a.exec();
}
