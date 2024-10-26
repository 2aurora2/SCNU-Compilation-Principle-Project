#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"
#include "util/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 进行主窗口的基本设置：禁用最大化按钮、禁止调节窗口大小和设置窗口标题
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    setWindowTitle("20212821028-林江荣-编译原理项目任务");

    // 针对两个项目任务的内容，通过标签页进行管理
    ui->tabWidget->setTabText(0, "项目任务1");
    ui->tabWidget->setTabText(1, "项目任务2");
}

MainWindow::~MainWindow() { delete ui; }
