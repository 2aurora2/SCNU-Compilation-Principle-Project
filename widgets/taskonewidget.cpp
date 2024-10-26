#include "taskonewidget.h"

#include "ui_taskonewidget.h"

TaskOneWidget::TaskOneWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::TaskOneWidget) {
    ui->setupUi(this);
    // 设置窗口的宽和高
    this->setBaseSize(1769, 923);

    // 通过标签页管理项目任务一分析结果
    ui->tabWidget->setTabText(0, "正则表达式NFA结果");
    ui->tabWidget->setTabText(1, "正则表达式DFA结果");
    ui->tabWidget->setTabText(2, "正则表达式最小化DFA结果");
    ui->tabWidget->setTabText(3, "生成词法分析源程序结果");
    ui->tabWidget->setTabText(4, "分析源程序结果");
}

TaskOneWidget::~TaskOneWidget() { delete ui; }
