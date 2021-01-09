#include "ObjectProject.h"

ObjectProject::ObjectProject(QObject* pobj)
{
	listOfParams = new QList<ObjectProjectData>();
}

void ObjectProject::retranslateUi()
{
	//«ададим переводимые хедеры
	setHeaderData(0, Qt::Horizontal, QObject::tr("Mark"));
	setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
	setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
	setHeaderData(3, Qt::Horizontal, QObject::tr("Object Type"));
	setHeaderData(4, Qt::Horizontal, QObject::tr("Signature"));
	setHeaderData(5, Qt::Horizontal, QObject::tr("Controller"));
	setHeaderData(6, Qt::Horizontal, QObject::tr("Resource"));
	setHeaderData(7, Qt::Horizontal, QObject::tr("Group events"));
	setHeaderData(8, Qt::Horizontal, QObject::tr("KKS"));
	setHeaderData(9, Qt::Horizontal, QObject::tr("Template object"));
	setHeaderData(10, Qt::Horizontal, QObject::tr("Mnemonic Frame Name"));
	setHeaderData(11, Qt::Horizontal, QObject::tr("Mnemonic Frame Template"));
	setHeaderData(12, Qt::Horizontal, QObject::tr("Mnemonic Frame Parent"));
	setHeaderData(13, Qt::Horizontal, QObject::tr("Technical Program Name"));
	setHeaderData(14, Qt::Horizontal, QObject::tr("Technical Program Parent"));

}

//метод data вызываетс€ автоматически и не требует €вного вызова, ровно как и rowCount с columnCount
//считаем эти методы параметрами таблицы

QVariant ObjectProject::data(const QModelIndex & index, int role) const
{
	if (index.isValid() && role == Qt::DisplayRole)
		return getData(index.row(), index.column());
	return QVariant();
}
/*
bool ObjectProject::setData(const QModelIndex & index, const QVariant & value, int nRole)
{
	if (index.isValid() && nRole == Qt::EditRole)
	{
		m_hash[index] = value;
		emit dataChanged(index, index);
		return true;
	}
	return false;
}
*/
int ObjectProject::rowCount(const QModelIndex &) const
{
	return listOfParams->size();
}

int ObjectProject::columnCount(const QModelIndex &) const
{
	//return m_nColumns;
	return 15;
}

Qt::ItemFlags ObjectProject::flags(const QModelIndex & index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	return index.isValid()
		? (flags | Qt::ItemIsEditable)
		: flags;
//	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ObjectProject::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical)
		//тут можно задать вертикальный хедер
		return QVariant(section + 1);
	else
		switch (section) {
		case 0:
			return QVariant("Mark1");
		case 1:
			return QVariant("Name1");
		case 2:
			return QVariant("Description1");
		case 3:
			return QVariant("Object Type1");
		case 4:
			return QVariant("Signature1");
		case 5:
			return QVariant("Controller1");
		case 6:
			return QVariant("Resource1");
		case 7:
			return QVariant("Group events1");
		case 8:
			return QVariant("KKS1");
		case 9:
			return QVariant("Template object1");
		case 10:
			return QVariant("Mnemonic Frame Name1");
		case 11:
			return QVariant("Mnemonic Frame Template1");
		case 12:
			return QVariant("Mnemonic Frame Parent1");
		case 13:
			return QVariant("Technical Program Name1");
		case 14:
			return QVariant("Technical Program Parent1");
		default:
			return QVariant();
		}
}

QVariant ObjectProject::getData(int num, int position) const {
	//бегаем по колонке (position)

	switch (position) {
	case 0:
		return listOfParams->at(num).getMark();
	case 1:
		return listOfParams->at(num).getName();
	case 2:
		return listOfParams->at(num).getDescription();
	case 3:
		return listOfParams->at(num).getObjectType();
	case 4:
		return listOfParams->at(num).getSignature();
	case 5:
		return listOfParams->at(num).getController();
	case 6:
		return listOfParams->at(num).getResource();
	case 7:
		return listOfParams->at(num).getEventGroup();
	case 8:
		return listOfParams->at(num).getKKS();
	case 9:
		return listOfParams->at(num).getObjectTemplate();
	case 10:
		return listOfParams->at(num).getMnemonicFrameName();
	case 11:
		return listOfParams->at(num).getMnemonicFrameTemplate();
	case 12:
		return listOfParams->at(num).getMnemonicFrameParent();
	case 13:
		return listOfParams->at(num).getTechnicalProgramName();
	case 14:
		return listOfParams->at(num).getTechnicalProgramParent();
	default:
		return QVariant();
	}
}

ObjectProject::~ObjectProject()
{
}

ObjectProjectData& ObjectProject::getObject(const QModelIndex & index) const
{
	return (*listOfParams)[index.row()];
}

void ObjectProject::addObject(ObjectProjectData& newData)
{
	beginInsertRows(QModelIndex(), listOfParams->size(), listOfParams->size());
	listOfParams->append(newData);
	endInsertRows();
}

void ObjectProject::delObject(const QModelIndex & index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	listOfParams->removeAt(index.row());
	endRemoveRows();
}

void ObjectProject::delData()
{
	for (int k = 0; k < listOfParams->count(); k++)
		listOfParams->removeAt(k);
	listOfParams->clear();
//	QMessageBox::information(this, "tt", "ttnt");
}
