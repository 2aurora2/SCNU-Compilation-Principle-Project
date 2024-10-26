#ifndef TASKONEWIDGET_H
#define TASKONEWIDGET_H

#include <QWidget>

namespace Ui {
class TaskOneWidget;
}

class TaskOneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskOneWidget(QWidget *parent = nullptr);
    ~TaskOneWidget();

private:
    Ui::TaskOneWidget *ui;
};

#endif // TASKONEWIDGET_H
