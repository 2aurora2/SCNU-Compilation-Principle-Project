#include "tasktwowidget.h"

#include <utils.h>

#include <QMessageBox>
#include <QStandardItemModel>

#include "ui_tasktwowidget.h"

TaskTwoWidget::TaskTwoWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::TaskTwoWidget) {
    ui->setupUi(this);
    // 设置窗口的宽和高
    this->setBaseSize(1769, 923);

    // 通过标签页管理项目任务一分析结果
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabText(0, "FIRST ＆ FOLLOW");
    ui->tabWidget->setTabText(1, "LR(1)DFA");
    ui->tabWidget->setTabText(2, "LALR(1)DFA");
    ui->tabWidget->setTabText(3, "LALR(1)分析表");
    ui->tabWidget->setTabText(4, "源程序语法分析");

    // connect信号槽
    connect(ui->uploadBNFButton, SIGNAL(clicked()), this,
            SLOT(uploadGrammar()));
    connect(ui->grammarAnalyseButton, SIGNAL(clicked()), this,
            SLOT(analyseGrammar()));
}

TaskTwoWidget::~TaskTwoWidget() { delete ui; }

/*!
    @Function       uploadGrammar
    @Description  用户点击上传文法文件触发的槽事件
    @Parameter   无
    @Return        无
    @Attention
*/
void TaskTwoWidget::uploadGrammar() {
    QString content = Util::ReadFile();
    ui->textEdit->setPlainText(content);
}

/*!
    @Function       analyseGrammar
    @Description  用户点击分析文法触发的槽事件
    @Parameter   无
    @Return        无
    @Attention
*/
void TaskTwoWidget::analyseGrammar() {
    QString content = ui->textEdit->toPlainText();
    if (content.size() == 0) {
        QMessageBox::warning(nullptr, "提示", "文法不能为空！",
                             QMessageBox::Ok);
        return;
    }
    task2.analyseGrammar(content);
    showNotEndFirstFollow();
    // TODO: 展示分析结果
}

/*!
    @Function       showNotEndFirst
    @Description  展示非终结符的FIRST集合
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::showNotEndFirstFollow() {
    QStandardItemModel *model = new QStandardItemModel(ui->firstFollowTable);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("非终结符"));
    model->setHorizontalHeaderItem(1, new QStandardItem("FIRST集合"));
    model->setHorizontalHeaderItem(2, new QStandardItem("FOLLOW集合"));
    int j = 0;
    for (auto symbol : task2.notEnd) {
        const QSet<QString> &v1 = task2.first[symbol];
        const QSet<QString> &v2 = task2.follow[symbol];
        QString s1 = "{ ";
        QString s2 = "{ ";
        for (auto v : v1) s1 += v + "，";
        for (auto v : v2) s2 += v + "，";
        s1.replace(s1.size() - 1, 1, " }");
        s2.replace(s2.size() - 1, 1, " }");
        model->setItem(j, 0, new QStandardItem(symbol));
        model->setItem(j, 1, new QStandardItem(s1));
        model->setItem(j, 2, new QStandardItem(s2));
        model->item(j, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(j, 1)->setTextAlignment(Qt::AlignCenter);
        model->item(j, 2)->setTextAlignment(Qt::AlignCenter);
        ++j;
    }
    ui->firstFollowTable->setModel(model);
    ui->firstFollowTable->resizeColumnsToContents();
    ui->firstFollowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
