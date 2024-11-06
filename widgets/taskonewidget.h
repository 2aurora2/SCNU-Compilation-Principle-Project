#ifndef TASKONEWIDGET_H
#define TASKONEWIDGET_H

#include <taskone/solutionone.h>

#include <QWidget>

namespace Ui {
class TaskOneWidget;
}

class TaskOneWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskOneWidget(QWidget *parent = nullptr);
    ~TaskOneWidget();

private slots:
    void uploadRegex();
    void uploadCode();
    void saveTextToFile();
    void analyseRegex();
    void analyseCode();

    void checkoutRegex(const QString &regex);

private:
    Ui::TaskOneWidget *ui;

    SolutionOne task1;

    void showNFATable(QString regex);
    void showDFATable(QString regex);
    void showMinDFATable(QString regex);
};

#endif  // TASKONEWIDGET_H
