#ifndef VIDEOARRANGE_H
#define VIDEOARRANGE_H

#include <QtWidgets/QMainWindow>

class VideoEditor : public QMainWindow
{
	Q_OBJECT

public:
	VideoEditor(QWidget *parent = 0);
	~VideoEditor();
	public slots:
	void onOpen();
	void onSave();
	void onNew();
private:

};

#endif // VIDEOARRANGE_H
