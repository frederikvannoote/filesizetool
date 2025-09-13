#include "sizeview.h"
#include "fileanalyzertask.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFileDialog>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FileSizeTool_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    const QString root = QFileDialog::getExistingDirectory(nullptr, QWidget::tr("Select root to start examining file size"));

    QSharedPointer<FileAnalyzerData> data(new FileAnalyzerData());

    QTimer::singleShot(1, &a, [data, root]{
        data->jobsCount.ref();
        QFileInfo rootInfo(root);
        data->root = QSharedPointer<FileItem>(new FileItem(nullptr, root, rootInfo.fileName(), true));
        data->dirsToAnalyze.enqueue(data->root.data());
        QThreadPool *pool = QThreadPool::globalInstance();
        int threadCount = pool->maxThreadCount();

        for (int i = 0; i < threadCount; ++i) {
            pool->start(new FileAnalyzerTask(data));
        }
    });

    // Connect a slot to the application's aboutToQuit signal
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        QThreadPool *pool = QThreadPool::globalInstance();
        qDebug() << "Application is about to quit. Waiting for threads to finish...";
        pool->waitForDone();
        qDebug() << "All threads have finished. Exiting.";
    });

    SizeView w;
    QObject::connect(&data->signaler, &CompletionSignaler::analysisFinished, &w, [&w, data](){
        w.setData(data);
        w.show();

        data->waitCondition.wakeAll();
    });

    return a.exec();
}
