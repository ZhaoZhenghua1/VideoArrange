#pragma once
//加载资源任务表
#include <QTreeView>
#include <QAbstractItemModel>
#include <memory>


class ResTaskView : public QTreeView
{
	Q_OBJECT
public:
	ResTaskView();
	~ResTaskView();
public slots:
	void addFiles(const QStringList& files);
};

class Transcoder;
struct Task
{
	QString taskName;
	std::shared_ptr<Transcoder> task;
	int percent;
};

class ResTaskModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	QVariant data(const QModelIndex &index, int role) const override;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
public slots:
	void onProgressChanged(int percent);
	void addFiles(const QStringList& files);
private:
	void removeCompletedTask();
private:
	QList<Task> m_tasks;
};