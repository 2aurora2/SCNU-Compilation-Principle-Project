#include "tasktwowidget.h"

#include <utils.h>

#include <QDebug>
#include <QMessageBox>
#include <QStack>
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

    ui->tabWidget_2->setCurrentIndex(0);
    ui->tabWidget_2->setTabText(0, "源程序语法分析过程");
    ui->tabWidget_2->setTabText(1, "源程序语法树");

    // connect信号槽
    connect(ui->uploadBNFButton, SIGNAL(clicked()), this,
            SLOT(uploadGrammar()));
    connect(ui->grammarAnalyseButton, SIGNAL(clicked()), this,
            SLOT(analyseGrammar()));
    connect(ui->uploadLexButton, SIGNAL(clicked()), this, SLOT(uploadLex()));
    connect(ui->codeAnalyseButton, SIGNAL(clicked()), this, SLOT(analyseLex()));
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
}

/*!
    @Function   uploadLex
    @Description 上传项目任务一得到的源程序词法分析结果文件
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::uploadLex() {
    pairs.clear();
    QString content = Util::ReadFile();
    QStringList lines = content.split("\n", QString::SkipEmptyParts);
    for (const QString &line : lines) {
        QStringList elements = line.split(" ", QString::SkipEmptyParts);
        if (elements.size() < 2 || elements[0] == "comment") {
            continue;
        }
        pairs.append(qMakePair(elements.at(0), elements.at(1)));
    }
    // 展示上传的文件
    QStandardItemModel *model = new QStandardItemModel(ui->lexTable);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("单词"));
    model->setHorizontalHeaderItem(1, new QStandardItem("类型"));
    for (int i = 0; i < pairs.size(); ++i) {
        model->setItem(i, 0, new QStandardItem(pairs[i].second));
        model->setItem(i, 1, new QStandardItem(pairs[i].first));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
    ui->lexTable->setModel(model);
    ui->lexTable->resizeRowsToContents();
    ui->lexTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lexTable->verticalHeader()->hide();
}

/*!
    @Function       addAnalyseRow
    @Description    添加句子分析过程表格的一行
    @Parameter  表格模型model，分析栈stk，pairs
    @Return
    @Attention
*/
void addAnalyseRow(QStandardItemModel *model, QVector<QPair<QString, int>> stk,
                   QVector<QPair<QString, QString>> pairs, int row) {
    QString analyse, input;
    for (auto item : stk) analyse += item.first + " ";
    for (int j = 0; j < pairs.size(); ++j) input += pairs[j].second + " ";
    model->setItem(row, 0, new QStandardItem(analyse));
    model->setItem(row, 1, new QStandardItem(input));
    model->item(row, 0)->setTextAlignment(Qt::AlignLeft);
    model->item(row, 1)->setTextAlignment(Qt::AlignRight);
}

/*!
    @Function       analyseLex
    @Description  利用上传的源程序词法分析结果进行语法分析
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::analyseLex() {
    QStandardItemModel *model = new QStandardItemModel(ui->processTable);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("分析栈"));
    model->setHorizontalHeaderItem(1, new QStandardItem("输入"));
    pairs.append(
        qMakePair(QString::fromUtf8("END_FLAG"), QString::fromUtf8("$")));
    QHash<int, QHash<QString, Cell>> tb = task2.tb.tb;
    // 分析栈：QPair第一个类型是元素内容，第二个元素是元素类型（1是状态编号，2是字符）
    QVector<QPair<QString, int>> stk;
    stk.push_back(qMakePair(QString::fromUtf8("$"), 2));
    stk.push_back(qMakePair(QString::number(1), 1));
    int row = 0;
    addAnalyseRow(model, stk, pairs, row++);
    for (int i = 0; i < pairs.size();) {
        // 从输入中拿出一个字符（first是类型，second是token）
        QPair<QString, QString> cur = pairs[i];
        int idx = stk.back().first.toInt() - 1;
        if (!tb[idx].contains(cur.first) && !tb[idx].contains(cur.second) &&
            !tb[idx].contains(EPSILON)) {
            QMessageBox::warning(nullptr, "提示", "语法错误！",
                                 QMessageBox::Yes);
            break;
        }
        Cell to;
        QString match;
        if (tb[idx].contains(cur.first))  // 匹配类型
            to = tb[idx][cur.first], match = cur.second;
        else if (tb[idx].contains(cur.second))  // 匹配token
            to = tb[idx][cur.second], match = cur.second;
        else if (tb[idx].contains(EPSILON))  // 匹配EPSILON
            to = tb[idx][EPSILON], match = EPSILON;
        bool nextStep = false;
        bool protocol = false;
        while (!nextStep) {
            if (to.flag == 1) {  // 移进操作，直接push移进到的状态编号到分析栈
                stk.push_back(qMakePair(match, 2));
                stk.push_back(qMakePair(QString::number(to.num + 1), 1));
                ++i;
                nextStep = true;
            } else if (to.flag == 2) {  // 规约操作
                protocol = true;
                // 1. 找到规约的规则
                //     a. 规约的符号 b. 规则右部的长度len
                QString rule = task2.tb.formula[to.num];
                QStringList ruleSplit = rule.split(" ");
                QString symbol = ruleSplit[0];
                int len = ruleSplit.size() - 2;
                // 2. 从分析栈弹出len * 2的元素
                //     a. 如果元素不足则源程序存在语法错误
                int k = 0;
                while (k < len * 2 && stk.size() > 0) {
                    stk.pop_back();
                    k++;
                }
                if (k < len * 2) {
                    QMessageBox::warning(nullptr, "提示", "语法错误！",
                                         QMessageBox::Yes);
                    goto show;
                }
                // 3. 获取此时栈顶状态经过规约符号的转移，重复对to.flag的判断
                int _idx = stk.back().first.toInt() - 1;
                match = symbol;
                to = tb[_idx][symbol];
            } else {  // Accept
                QMessageBox::information(nullptr, "提示", "接受输入！",
                                         QMessageBox::Yes);
                nextStep = true;
                goto show;
            }
        }
        if (protocol) i--;  // 如果有规约操作则需重新拿去输入的字符
        addAnalyseRow(model, stk, pairs.mid(i), row++);
    }
show:
    ui->processTable->setModel(model);
    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->processTable->resizeColumnsToContents();
    ui->processTable->resizeRowsToContents();
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
            model->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
            model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
            if (i == 0) model->item(i, 0)->setBackground(QBrush(Qt::red));
        }
        for (int i = 0; i < task2.lalr1.size; ++i) {
            for (int j = 0; j < list.size(); ++j) {
                if (task2.tb.tb[i].contains(list[j])) {
                    Cell cell = task2.tb.tb[i][list[j]];
                    QString temp;
                    if (cell.flag == 1)
                        temp = "s" + QString::number(cell.num + 1);
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
