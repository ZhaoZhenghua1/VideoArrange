
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
	ve.show();

	return a.exec();
}
