#ifndef TASKTWOWIDGET_H
#define TASKTWOWIDGET_H

#include <tasktwo/solutiontwo.h>

#include <QTableView>
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

    void showNotEndFirstFollow();
    void showLR1OrLALR1DFA(QTableView *tb, LR lr);
};

#endif  // TASKTWOWIDGET_H
