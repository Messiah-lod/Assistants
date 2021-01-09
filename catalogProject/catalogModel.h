#pragma once

#include <qabstractitemmodel.h>
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 

class catalogModel :
	public QAbstractItemModel
{
	Q_OBJECT

public:
	catalogModel::catalogModel(QObject *parent = nullptr);
	int rowCount(const QModelIndex& parent) const;
	int columnCount(const QModelIndex& parent) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;

	void appendProject(const QString& surname, const QString& name, const QString& patronymic);

	bool setData(const QModelIndex & index, const QVariant & value, int role);

public slots:
	void removeSelected();
	void OnDoubleClicked(const QModelIndex &index);

signals:
	void closeWindow();

private:
	enum Column {
		SURNAME = 0,
		NAME,
		PATRONYMIC,
		SELECTION,
		LAST
	};

	typedef QHash< Column, QVariant > ProjectData;
	typedef QList< ProjectData > Project;
	Project m_project;
};