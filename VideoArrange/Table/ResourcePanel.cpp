#include "ResourcePanel.h"
#include <QPushButton>
#include <QVBoxLayout>
#include "ResTableView.h"
#include "ResTaskView.h"
#include <QFileDialog>
#include <QPainter>
#include <QSplitter>

ResourcePanel::ResourcePanel()
{
	QVBoxLayout* thisLayout = new QVBoxLayout;
	thisLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(thisLayout);

	QWidget* resWidget = new QWidget;
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QColor(29, 29, 29)));
	resWidget->setPalette(palette);
	resWidget->setAutoFillBackground(true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setSpacing(2);
	layout->setContentsMargins(0, 0, 0, 0);
	resWidget->setLayout(layout);

	QPushButton*  addResBtn = new QPushButton;
	addResBtn->setFixedSize(67, 21);
	addResBtn->setStyleSheet("border-image:url(:/add_layer.png);");
	connect(addResBtn, SIGNAL(clicked(bool)), this, SLOT(onAddResource()));
	layout->addWidget(addResBtn);
	layout->addWidget(m_resTableView = new ResTableView);

	QSplitter* pSplitter = new QSplitter(Qt::Vertical);
	pSplitter->setHandleWidth(3);
	pSplitter->setChildrenCollapsible(false);
	pSplitter->addWidget(resWidget);
	pSplitter->addWidget(m_resTaskView = new ResTaskView);
	QSplitterHandle* handle = pSplitter->handle(1);
	handle->setStyleSheet("background-color: rgb(22, 22, 22);");

	thisLayout->addWidget(pSplitter);
	connect(m_resTableView, SIGNAL(addTaskFiles(const QStringList&)), m_resTaskView, SLOT(addFiles(const QStringList&)));
}


ResourcePanel::~ResourcePanel()
{
}

void ResourcePanel::onAddResource()
{
	QStringList files = QFileDialog::getOpenFileNames(this, QString::fromLocal8Bit("选中资源"));
	m_resTableView->addFiles(files);
}
