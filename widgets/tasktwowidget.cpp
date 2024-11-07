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
    showNotEndFirst();
    // TODO: 展示分析结果
}

/*!
    @Function       showNotEndFirst
    @Description  展示非终结符的FIRST集合
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::showNotEndFirst() {
    QStandardItemModel *model = new QStandardItemModel(ui->firstTable);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("非终结符"));
    model->setHorizontalHeaderItem(1, new QStandardItem("FIRST集合"));
    QHashIterator<QString, QSet<QString>> i(task2.first);
    int j = 0;
    while (i.hasNext()) {
        i.next();
        QString key = i.key();
        QString result = "{";
        const QSet<QString> &value = i.value();
        for (auto val : value) result += val + "，";
        result.chop(1);
        result += "}";
        model->setItem(j, 0, new QStandardItem(key));
        model->setItem(j, 1, new QStandardItem(result));
        model->item(j, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(j, 1)->setTextAlignment(Qt::AlignCenter);
        ++j;
    }
    ui->firstTable->setModel(model);
    ui->firstTable->resizeColumnsToContents();
    ui->firstTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
