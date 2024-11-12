#ifndef TASKTWOWIDGET_H
#define TASKTWOWIDGET_H

#include <tasktwo/solutiontwo.h>

#include <QStandardItemModel>
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
    void uploadLex();
    void analyseLex();

private:
    Ui::TaskTwoWidget *ui;

    SolutionTwo task2;
    QVector<QPair<QString, QString>> pairs;

    void showNotEndFirstFollow();
    void showLR1OrLALR1DFA(QTableView *tb, LR lr);
    void showLALR1AnalyseTable();
};

#endif  // TASKTWOWIDGET_H
