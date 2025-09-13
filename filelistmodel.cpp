#include "filelistmodel.h"
#include "fileanalyzertask.h"
#include <QFileInfo>
#include <QDir>

FileTreeModel::FileTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_pRoot(nullptr)
{
}

FileTreeModel::~FileTreeModel()
{
    if (m_pRoot)
        delete m_pRoot;
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileItem *parentItem;
    if (!parent.isValid())
        parentItem = m_pRoot;
    else
        parentItem = static_cast<FileItem*>(parent.internalPointer());

    FileItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex FileTreeModel::parent(const QModelIndex &child) const {
    if (!child.isValid())
        return QModelIndex();

    FileItem *childItem = static_cast<FileItem*>(child.internalPointer());
    FileItem *parentItem = childItem->parentItem();

    if (parentItem == m_pRoot)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FileTreeModel::rowCount(const QModelIndex &parent) const {
    FileItem *parentItem = nullptr;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_pRoot;
    else
        parentItem = static_cast<FileItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FileTreeModel::columnCount(const QModelIndex &parent) const {
    // We now have two columns
    return 2;
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    FileItem *item = static_cast<FileItem*>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return item->name();
        else if (index.column() == 1)
            return fileSize(item->size());
    }
    if (role == Qt::UserRole + 1) {
        if (index.column() == 0)
            return item->name();
        else if (index.column() == 1)
            return item->size();
    }

    return QVariant();
}

QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0) {
            return "File/Directory Name";
        } else if (section == 1) {
            return "Size";
        }
    }
    return QVariant();
}

void FileTreeModel::setAnalysisResults(FileItem *item) {
    beginResetModel();

    if (m_pRoot)
        delete m_pRoot;

    m_pRoot = item;

    endResetModel();
}

QString FileTreeModel::fileSize(int bytes) const
{
    if (bytes > 1024*1024*1024)
        return QString("%1 GB").arg(bytes / (1024*1024*1024));
    if (bytes > 1024*1024)
        return QString("%1 MB").arg(bytes / (1024*1024));
    if (bytes > 1024)
        return QString("%1 KB").arg(bytes / 1024);

    return QString("%1 bytes").arg(bytes);
}
