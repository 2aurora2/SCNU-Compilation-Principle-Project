#ifndef TASKTWOWIDGET_H
#define TASKTWOWIDGET_H

#include <tasktwo/solutiontwo.h>

#include <QWidget>

namespace Ui {
class TaskTwoWidget;
}

class TaskTwoWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskTwoWidget(QWidget *parent = nullptr);
    ~TaskTwoWidget();

private slots:
    void uploadGrammar();
    void analyseGrammar();

private:
    Ui::TaskTwoWidget *ui;

    SolutionTwo task2;

    void showNotEndFirst();
};

#endif  // TASKTWOWIDGET_H
