#include "tasktwowidget.h"

#include "ui_tasktwowidget.h"

TaskTwoWidget::TaskTwoWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::TaskTwoWidget) {
    ui->setupUi(this);
    // 设置窗口的宽和高
    this->setBaseSize(1769, 923);
}

TaskTwoWidget::~TaskTwoWidget() { delete ui; }
