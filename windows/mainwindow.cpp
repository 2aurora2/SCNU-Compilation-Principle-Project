#include "mainwindow.h"

#include <QDebug>

#include "ui_mainwindow.h"
#include "util/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }
