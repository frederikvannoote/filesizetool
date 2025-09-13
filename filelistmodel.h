#pragma once

#include <QAbstractItemModel>
#include <QMap>
class FileItem;


class FileTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit FileTreeModel(QObject *parent = nullptr);
    virtual ~FileTreeModel();

    // Required QAbstractItemModel overrides
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void setAnalysisResults(FileItem *item);

private:
    QString fileSize(int bytes) const;

    FileItem *m_pRoot;
    QMap<QString, qint64> m_fileSizes;
    QMap<QString, QStringList> m_directoryStructure; // Parent path -> list of children
};
