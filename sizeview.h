#pragma once

#include <QWidget>
#include "fileanalyzertask.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SizeView;
}
QT_END_NAMESPACE

class SizeView : public QWidget
{
    Q_OBJECT

public:
    SizeView(QWidget *parent = nullptr);
    ~SizeView();

    void setData(QSharedPointer<FileAnalyzerData> data);

private:
    Ui::SizeView *ui;
};
