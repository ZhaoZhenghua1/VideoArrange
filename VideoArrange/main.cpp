#include "VideoEditor.h"
#include <QtWidgets/QApplication>

#include "SwitchButton.h"
#include "Layer.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QVBoxLayout>
#include "VideoArrange.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
// 	VideoArrange w;
// 	w.show();
// 
// 	QVBoxLayout* playout = new QVBoxLayout;
// 	QWidget * pw = new QWidget;
// 	pw->setLayout(playout);
// 
// 	QGraphicsScene scene;
// 	//scene.addItem(new QGraphicsRectItem(0, 10, 100, 200));
// 	scene.setSceneRect(0, 0, 1000, 2000);
// 
// 	QGraphicsView* pv1 = new QGraphicsView;
// 	pv1->setScene(&scene);
// 	playout->addWidget(pv1);
// 	QGraphicsView* pv2 = new QGraphicsView;
// 	pv2->setScene(&scene);
// 	playout->addWidget(pv2);
// 	pv1->setVerticalScrollBar(pv2->verticalScrollBar());
// 	pv2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 	w.setCentralWidget(pw);

	VideoArrange le;
	le.show();
	return a.exec();
}
