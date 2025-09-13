#include "fileanalyzertask.h"
#include <QDebug>


FileAnalyzerTask::FileAnalyzerTask(QSharedPointer<FileAnalyzerData> data)
    : QRunnable()
    , m_data(data)
{
}


void FileAnalyzerTask::run() {
    while (true) {
        m_data->mutex.lock();
        if (m_data->dirsToAnalyze.isEmpty()) {
            m_data->waitCondition.wait(&m_data->mutex); // Wait for new work
            if (m_data->dirsToAnalyze.isEmpty()) {
                m_data->mutex.unlock();
                break; // No more work, exit thread
            }
        }
        FileItem *item = m_data->dirsToAnalyze.dequeue();
        m_data->mutex.unlock();

        // Analyze directory
        qInfo() << "Analyzing" << item->path();
        QDir dir(item->path());

        QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        for (const QFileInfo& entry : entries) {
            if (entry.isDir())
            {
                FileItem *child = new FileItem(item, entry.absoluteFilePath(), entry.fileName(), true);
                item->append(child);

                m_data->mutex.lock();
                m_data->jobsCount.ref(); // Increment for the new job
                m_data->dirsToAnalyze.enqueue(child);
                m_data->waitCondition.wakeOne(); // Signal a waiting thread
                m_data->mutex.unlock();
            }
            else
            {
                FileItem *child = new FileItem(item, entry.absoluteFilePath(), entry.fileName(), false, entry.size());
                item->append(child);
            }
        }

        m_data->mutex.lock();

        // When a directory is fully analyzed and no new subdirectories are found
        if (!m_data->jobsCount.deref()) {
            // This thread is the last one to finish. The counter just went from 1 to 0.

            // It is now safe to emit the signal
            QMetaObject::invokeMethod(&m_data->signaler, "analysisFinished", Qt::QueuedConnection);

            // Wake up any threads that might be waiting to exit
            m_data->waitCondition.wakeAll();
        }

        m_data->mutex.unlock();
    }
}

FileItem::FileItem(FileItem *parent, const QString &path, const QString &name, bool isDir, quint64 size):
    m_pParent(parent),
    m_path(path),
    m_name(name),
    m_isDir(isDir),
    m_size(size),
    m_children()
{
}

FileItem::~FileItem()
{
    for (FileItem* child: m_children)
    {
        delete child;
    }
    m_children.clear();
}

QString FileItem::path() const
{
    return m_path;
}

QString FileItem::name() const
{
    return m_name;
}

quint64 FileItem::size() const
{
    if (!m_isDir)
        return m_size;
    else
    {
        quint64 result = 0;
        for (FileItem* child: m_children)
        {
            result += child->size();
        }
        return result;
    }
}

int FileItem::childCount() const
{
    return m_children.size();
}

void FileItem::append(FileItem *child)
{
    m_children.push_back(child);
}

int FileItem::row() const
{
    if (m_pParent)
    {
        auto it = std::find(m_pParent->m_children.begin(), m_pParent->m_children.end(), this);
        if (it != m_pParent->m_children.end())
        {
            return std::distance(m_pParent->m_children.begin(), it);
        }
    }
    return 0;
}

FileItem *FileItem::child(int index) const
{
    return m_children.at(index);
}

FileItem *FileItem::parentItem() const
{
    return m_pParent;
}
