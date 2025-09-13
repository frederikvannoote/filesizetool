#pragma once

#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QAtomicInt>
#include <QQueue>
#include <QWaitCondition>
#include <QDir>
#include <QMap>


class CompletionSignaler : public QObject {
    Q_OBJECT
Q_SIGNALS:
    void analysisFinished();
};

class FileItem {
public:
    FileItem(FileItem *parent, const QString &path, const QString &name, bool isDir, quint64 size = 0);
    ~FileItem();

    QString path() const;
    QString name() const;
    quint64 size() const;
    int childCount() const;

    void append(FileItem *child);

    int row() const;
    FileItem* child(int index) const;
    FileItem* parentItem() const;

private:
    FileItem *m_pParent;
    const QString m_path;
    const QString m_name;
    const bool m_isDir;
    quint64 m_size;

    std::vector<FileItem*> m_children;
};

class FileAnalyzerData {
public:
    QMutex mutex;
    QQueue<FileItem*> dirsToAnalyze;
    QSharedPointer<FileItem> root;
    QAtomicInt jobsCount;
    QWaitCondition waitCondition;
    CompletionSignaler signaler;
};

class FileAnalyzerTask : public QRunnable {
public:
    FileAnalyzerTask(QSharedPointer<FileAnalyzerData> data);

    void run() override;

private:
    QSharedPointer<FileAnalyzerData> m_data;
};
