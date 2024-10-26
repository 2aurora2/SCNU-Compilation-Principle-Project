#ifndef TASKTWOWIDGET_H
#define TASKTWOWIDGET_H

#include <QWidget>

namespace Ui {
class TaskTwoWidget;
}

class TaskTwoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskTwoWidget(QWidget *parent = nullptr);
    ~TaskTwoWidget();

private:
    Ui::TaskTwoWidget *ui;
};

#endif // TASKTWOWIDGET_H
