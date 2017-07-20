#include "ResTaskView.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QFileInfo>
#include "..\Transcoder\include\transcoder.h"
#include <QTimer>
#include <QDir>
#include <algorithm>

ResTaskView::ResTaskView()
{
	header()->setStyleSheet("QHeaderView::section{background-color: rgb(29, 29, 29); color:rgb(189, 189, 189);padding-left: 4px;border: 1px solid #6c6c6c}");

	setStyleSheet("QTreeView{background-color: rgb(29, 29, 29);}");
	horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal { height: 11px; }");
	verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 11px; }");

	setModel(new ResTaskModel());
}


ResTaskView::~ResTaskView()
{
}

void ResTaskView::addFiles(const QStringList& files)
{
	((ResTaskModel*)model())->addFiles(files);

	doItemsLayout();
}

QVariant ResTaskModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int tindex = int(index.internalPointer());

	if (tindex < 0 || m_tasks.size() < tindex)
	{
		return QVariant();
	}

	//编辑和显示
	if (Qt::DisplayRole == role)
	{
		switch (index.column())
		{
		case 0:
			return QFileInfo(m_tasks[tindex].taskName).fileName();
		case  1:
			return QString("%1%").arg(m_tasks[tindex].percent);
		default:
			return "";
		}
	}
	else if (Qt::BackgroundRole == role)
	{
		return QVariant(QColor(29, 29, 29));
	}
	else if (Qt::TextColorRole == role)
	{
		return QVariant(QColor(138, 138, 138));
	}
	// 	else if (Qt::FontRole == role)
	// 	{
	// 		return QVariant(QFont("微软雅黑", 10));
	// 	}

	return QVariant();
}

bool ResTaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags ResTaskModel::flags(const QModelIndex &index) const
{
	return QAbstractItemModel::flags(index);
}

QVariant ResTaskModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
			case 0:
				return QString::fromLocal8Bit("任务");
			case 1:
				return QString::fromLocal8Bit("进度");
			default:
				return QVariant();
			}
		}
		else if (role == Qt::ForegroundRole)
		{
			return QVariant(QColor(189, 189, 189));
		}
	}

	return QVariant();
}

QModelIndex ResTaskModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	if (parent.isValid())
	{
		return QModelIndex();
	}

	return createIndex(row, column, row);
}

QModelIndex ResTaskModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

int ResTaskModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (parent.isValid())
	{
		return 0;
	}
	else
	{
		return m_tasks.size();
	}
}

int ResTaskModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 2;
}

void ResTaskModel::onProgressChanged(int percent)
{
	int index = -1;
	for (Task& t : m_tasks)
	{
		++index;
		if (&*(t.task) == sender())
		{
			t.percent = percent;
			emit dataChanged(this->index(index, 1), this->index(index, 1));
			break;
		}
	}

	if (percent == 100)
	{
		QTimer::singleShot(1000, [this]() {removeCompletedTask();});
	}
}

void ResTaskModel::addFiles(const QStringList& files)
{
	for (const QString& file : files)
	{
		Transcoder* tc = new Transcoder;
		connect(tc, SIGNAL(progress(int)), this, SLOT(onProgressChanged(int )));
		tc->setInput(file);
		QFileInfo fi(file);

		QString smallPath(fi.path() + "/.small/");
		QDir dir(smallPath);
		if (!dir.exists())
		{
			dir.mkpath(smallPath);
		}

		QString outFile(smallPath + fi.fileName());
		if (QFileInfo(outFile).exists())
		{
			continue;
		}

		tc->setOutput(outFile);
		m_tasks.push_back({ file,std::shared_ptr<Transcoder>(tc), 0});
		QTimer::singleShot(0, [tc]() {tc->transcode();});
	}
}

void ResTaskModel::removeCompletedTask()
{
	auto removeBegin = std::remove_if(m_tasks.begin(), m_tasks.end(), [](const Task& t) {return t.percent == 100;});
	if (removeBegin != m_tasks.end())
	{
		m_tasks.erase(removeBegin, m_tasks.end());

		layoutChanged();
	}
}
