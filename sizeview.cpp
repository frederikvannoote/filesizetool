#include "sizeview.h"
#include "./ui_sizeview.h"
#include "filelistmodel.h"
#include "fileanalyzertask.h"
#include <QSortFilterProxyModel>


SizeView::SizeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SizeView)
{
    ui->setupUi(this);
}

SizeView::~SizeView()
{
    delete ui;
}

void SizeView::setData(QSharedPointer<FileAnalyzerData> data)
{
    FileTreeModel *model = new FileTreeModel(this);
    QSortFilterProxyModel *proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setSortRole(Qt::UserRole + 1);
    model->setAnalysisResults(data->root.data());

    ui->treeView->setModel(proxy);
    ui->treeView->setSortingEnabled(true);
    ui->treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
}
