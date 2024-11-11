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
    showLR1OrLALR1DFA(ui->lr1DFATable, task2.lr1);
    showLR1OrLALR1DFA(ui->lalr1DFATable, task2.lalr1);
    showLALR1AnalyseTable();
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

/*!
    @Function   showLR1OrLALR1DFA
    @Description    展示LR1或LALR1的DFA
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::showLR1OrLALR1DFA(QTableView *tb, LR lr) {
    QStandardItemModel *model = new QStandardItemModel(tb);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("状态\\转移"));
    QSet<QString> keys;
    for (auto itOuter = lr.changeHash.begin(); itOuter != lr.changeHash.end();
         ++itOuter)
        for (auto itInner = itOuter.value().begin();
             itInner != itOuter.value().end(); ++itInner)
            keys.insert(itInner.key());
    QList<QString> list = keys.values();
    // 设置表头
    for (int i = 0; i < list.size(); ++i)
        model->setHorizontalHeaderItem(i + 1, new QStandardItem(list.at(i)));
    // 设置第一列
    for (int i = 1; i <= lr.size; ++i) {
        QString state = lr.stateHashT[i - 1].printState();
        model->setItem(i - 1, 0, new QStandardItem(state));
        model->item(i - 1, 0)->setTextAlignment(Qt::AlignCenter);
        if (i == 1) model->item(i - 1, 0)->setBackground(QBrush(Qt::red));
    }
    for (int i = 0; i < lr.size; ++i) {
        for (int j = 0; j < list.size(); ++j) {
            if (lr.changeHash[i].contains(list[j])) {
                model->setItem(i, j + 1,
                               new QStandardItem(QString::number(
                                   lr.changeHash[i][list[j]] + 1)));
                model->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    tb->setModel(model);
    tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tb->resizeColumnsToContents();
    tb->resizeRowsToContents();
}

/*!
    @Function       showLALR1AnalyseTable
    @Description 在LALR1的DFA无规约-规约冲突时展示其分析表
    @Parameter  是否有规约-规约冲突
    @Return
    @Attention 对于移进-规约冲突采取移进策略
*/
void TaskTwoWidget::showLALR1AnalyseTable() {
    // 展示可规约规则到下拉框
    ui->comboBox->clear();
    for (int i = 0; i < task2.tb.formula.size(); ++i)
        ui->comboBox->addItem("r" + QString::number(i) + ": " +
                              task2.tb.formula[i]);
    // 展示分析表
    QStandardItemModel *model = new QStandardItemModel(ui->analyseTable);
    model->clear();
    if (task2.lalr1.conflict == "规约-规约冲突") {
        model->setHorizontalHeaderLabels(QStringList() << "提示");
        QStandardItem *emptyItem = new QStandardItem(task2.lalr1.conflict);
        emptyItem->setEditable(false);
        model->setItem(0, 0, emptyItem);
        model->item(0, 0)->setTextAlignment(Qt::AlignCenter);
    } else {
        model->setHorizontalHeaderItem(0, new QStandardItem("状态\\转移"));
        // 找到所有的转移
        QSet<QString> keys;
        for (auto itOuter = task2.lalr1.changeHash.begin();
             itOuter != task2.lalr1.changeHash.end(); ++itOuter)
            for (auto itInner = itOuter.value().begin();
                 itInner != itOuter.value().end(); ++itInner)
                keys.insert(itInner.key());
        QList<QString> list = keys.values();
        list.append("$");  // 添加初态的accept
        // 设置表头
        for (int i = 0; i < list.size(); ++i)
            model->setHorizontalHeaderItem(i + 1,
                                           new QStandardItem(list.at(i)));
        // 设置第一列
        for (int i = 0; i < task2.lalr1.size; ++i) {
            model->setItem(i, 0, new QStandardItem(QString::number(i)));
            model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
            if (i == 0) model->item(i, 0)->setBackground(QBrush(Qt::red));
        }
        for (int i = 0; i < task2.lalr1.size; ++i) {
            for (int j = 0; j < list.size(); ++j) {
                if (task2.tb.tb[i].contains(list[j])) {
                    Cell cell = task2.tb.tb[i][list[j]];
                    QString temp;
                    if (cell.flag == 1)
                        temp = "s" + QString::number(cell.num);
                    else if (cell.flag == 2)
                        temp = "r" + QString::number(cell.num);
                    else
                        temp = "Accept";
                    model->setItem(i, j + 1, new QStandardItem(temp));
                    model->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }
    ui->analyseTable->setModel(model);
    ui->analyseTable->verticalHeader()->hide();
}
