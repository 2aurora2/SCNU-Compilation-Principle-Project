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
    void uploadSyntaxTreeEncode();
    void analyseLex();

private:
    Ui::TaskTwoWidget *ui;

    SolutionTwo task2;
    QVector<QPair<QString, QString>> pairs;    // 源程序单词编码
    QHash<QString, QVector<int>> treeEncoder;  // 语法树编码

    void reset();
    void showNotEndFirstFollow();
    void showLR1OrLALR1DFA(QTableView *tb, LR lr);
    void showLALR1AnalyseTable();
};

#endif  // TASKTWOWIDGET_H
