
#include "catalogModel.h"

catalogModel::catalogModel(QObject *parent)
	: QAbstractItemModel(parent)
{

}

int catalogModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
			return m_project.count();
}

int catalogModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
				return LAST;
}

QVariant catalogModel::data(const QModelIndex &index, int role) const
{
	//int row = index.row();
	//int col = index.column();
	//// generate a log message when this method gets called
	//qDebug() << QString("row %1, col%2, role %3")
	//	.arg(row).arg(col).arg(role);

	//switch (role) {
	//case Qt::DisplayRole:
	//	if (row == 0 && col == 1) return QString("<--left");
	//	if (row == 1 && col == 1) return QString("right-->");

	//	return QString("Row%1, Column%2")
	//		.arg(row + 1)
	//		.arg(col + 1);
	//case Qt::FontRole:
	//	if (row == 0 && col == 0) { //change font only for cell(0,0)
	//		QFont boldFont;
	//		boldFont.setBold(true);
	//		return boldFont;
	//	}
	//	break;
	//case Qt::BackgroundRole:
	//	if (row == 1 && col == 2)  //change background only for cell(1,2)
	//		return QBrush(Qt::red);
	//	break;
	//case Qt::TextAlignmentRole:
	//	if (row == 1 && col == 1) //change text alignment only for cell(1,1)
	//		return Qt::AlignRight + Qt::AlignVCenter;
	//	break;
	//case Qt::CheckStateRole:
	//	if (row == 1 && col == 0) //add a checkbox to cell(1,0)
	//		return Qt::Checked;
	//	break;
	//}
//	return QVariant();


	if (
		!index.isValid() ||
		m_project.count() <= index.row() ||
		(role != Qt::DisplayRole && role != Qt::EditRole)
		) {
		return QVariant();
	}

	return m_project[index.row()][Column(index.column())];

//	return QVariant();
}

QVariant catalogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	if (orientation == Qt::Vertical) {
		return section;
	}

	switch (section) {
	case SURNAME:
		return tr("Name project");
	case NAME:
		return tr("Path project");
	case PATRONYMIC:
		return tr("PATRONYMIC");
	case SELECTION:
		return tr("SELECTION");
	}

	return QVariant();
}

Qt::ItemFlags catalogModel::flags(const QModelIndex & index) const
{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.isValid()) {
		if (index.column() == SELECTION) {
			flags |= Qt::ItemIsEditable;
		}
	}

	return flags;
}

void catalogModel::appendProject(const QString & surname, const QString & name, const QString & patronymic)
{
	ProjectData person;
	person[SURNAME] = surname;
	person[NAME] = name;
	person[PATRONYMIC] = patronymic;
	person[SELECTION] = false;

	int row = m_project.count();
	beginInsertRows(QModelIndex(),
 row, row);
	m_project.append(person);
	endInsertRows();
}

bool catalogModel::setData(const QModelIndex& index, const QVariant& value, int role) 
{
	if (!index.isValid() || role != Qt::EditRole || m_project.count() <= index.row()) {
		return false;
	}

	m_project[index.row()][Column(index.column())] = value;
	emit dataChanged(index, index);

	return true;
}

void catalogModel::removeSelected() 
{
	int i = 0;
	Project::iterator it = m_project.begin();
	while (it != m_project.end()) {
		if (it->value(SELECTION, false).toBool()) {
			beginRemoveRows(QModelIndex(), i, i);
			it = m_project.erase(it);
			endRemoveRows();
		}
		else {
			++i;
			++it;
		}
	}
}

void catalogModel::OnDoubleClicked(const QModelIndex &index)
{
	const QString a = (index.model()->data(index.model()->index(index.row(), index.column())).toString());

	//QMessageBox msg;
	//msg.setText(a);
	//msg.exec();

//	TeconAssistants w;
//	w.showMaximized();
	//испустим сигнал на закрытие окна
	emit closeWindow();
}