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
    connect(ui->uploadTreeEncode, SIGNAL(clicked()), this,
            SLOT(uploadSyntaxTreeEncode()));
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
    reset();

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
    @Function   uploadSyntaxTreeEncode
    @Description    上传语法树编码文件
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::uploadSyntaxTreeEncode() {
    QString content = Util::ReadFile();
    QStringList lines = content.split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < lines.size(); i += 2) {
        QString rule = lines[i];
        QStringList encoder = lines[i + 1].split(" ");
        for (auto e : encoder) treeEncoder[rule].append(e.toInt());
    }
    if (content.size())
        QMessageBox::information(nullptr, "提示", "上传语法树编码文件成功！",
                                 QMessageBox::Ok);
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
    if (pairs.empty() || treeEncoder.empty()) {
        QMessageBox::warning(nullptr, "提示",
                             "请保证已上传下述文件，两者缺一不可： \n "
                             "(1)源程序词法分析结果文件(*.lex)\n "
                             "(2)对应源程序的语法树编码文件(*_syntax_tree.txt)",
                             QMessageBox::Ok);
        return;
    }
    QStandardItemModel *model = new QStandardItemModel(ui->processTable);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("分析栈"));
    model->setHorizontalHeaderItem(1, new QStandardItem("输入"));
    pairs.append(
        qMakePair(QString::fromUtf8("END_FLAG"), QString::fromUtf8("$")));
    QHash<int, QHash<QString, Cell>> tb = task2.tb.tb;
    // 节点栈
    QVector<SyntaxTreeNode *> nodeStk;
    // 分析栈：QPair第一个类型是元素内容，第二个元素是元素类型（1是状态编号，2是字符）
    QVector<QPair<QString, int>> analyseStk;
    analyseStk.push_back(qMakePair(QString::fromUtf8("$"), 2));
    analyseStk.push_back(qMakePair(QString::number(1), 1));
    int row = 0;
    addAnalyseRow(model, analyseStk, pairs, row++);
    for (int i = 0; i < pairs.size();) {
        // 从输入中拿出一个字符（first是类型，second是token）
        QPair<QString, QString> cur = pairs[i];
        int idx = analyseStk.back().first.toInt() - 1;
        if (!tb[idx].contains(cur.first) && !tb[idx].contains(cur.second) &&
            !tb[idx].contains(EPSILON)) {
            QMessageBox::warning(nullptr, "提示", "语法错误！",
                                 QMessageBox::Ok);
            break;
        }
        Cell to;
        QString matchForAnalyse;
        Token matchForNode;
        if (tb[idx].contains(cur.first)) {  // 匹配类型
            to = tb[idx][cur.first];
            matchForAnalyse = cur.second;
            matchForNode.type = cur.first;
            matchForNode.value = cur.second;
        } else if (tb[idx].contains(cur.second)) {  // 匹配token
            to = tb[idx][cur.second];
            matchForAnalyse = cur.second;
            matchForNode.type = cur.first;
            matchForNode.value = cur.second;
        } else if (tb[idx].contains(EPSILON)) {  // 匹配EPSILON
            to = tb[idx][EPSILON];
            matchForAnalyse = EPSILON;
            matchForNode.type = "EPSILON";
            matchForNode.value = EPSILON;
        }
        bool nextStep = false;
        bool protocol = false;
        while (!nextStep) {
            // 移进
            if (to.flag == 1) {
                // 句子分析：直接push移进到的状态编号到分析栈
                analyseStk.push_back(qMakePair(matchForAnalyse, 2));
                analyseStk.push_back(qMakePair(QString::number(to.num + 1), 1));
                if (matchForAnalyse != EPSILON) ++i;
                nextStep = true;
                if (task2.notEnd.contains(matchForAnalyse)) continue;
                // 语法树：对于终结符构造新的语法树节点并丢入节点栈
                SyntaxTreeNode *newNode =
                    new SyntaxTreeNode(matchForNode.type, matchForNode.value);
                nodeStk.push_back(newNode);
            }
            // 规约
            else if (to.flag == 2) {
                protocol = true;
                // 句子分析步骤<1,2,3>
                // 语法树步骤<A,B>
                // 1. 找到规约的规则，包括: 规约的符号symbol、规则右部的长度len
                QString rule = task2.tb.formula[to.num];
                QStringList ruleSplit = rule.split(" ");
                QString symbol = ruleSplit[0];
                int len = ruleSplit.size() - 2;
                // A. 找到该条规约规则的语法树编码
                QVector<int> encoder = treeEncoder[rule];
                // 2. 从分析栈弹出len*2的元素，如果元素不足则源程序存在语法错误
                int k;
                for (k = 0; k < len * 2 && analyseStk.size() > 0; ++k)
                    analyseStk.pop_back();
                if (k < len * 2) {
                    QMessageBox::warning(nullptr, "提示", "语法错误！",
                                         QMessageBox::Ok);
                    goto show;
                }
                // B. 从节点栈弹出len个元素并构造语法树，
                // 最后讲当前规约规则的根节点丢入节点栈
                QVector<SyntaxTreeNode *> tempVec;
                SyntaxTreeNode *tempRoot;  // 临时根节点
                for (k = 0; k < len && nodeStk.size() > 0; ++k) {
                    SyntaxTreeNode *temp = nodeStk.back();
                    tempVec.append(temp);
                    nodeStk.pop_back();
                    if (encoder[encoder.size() - 1 - k] == 1) tempRoot = temp;
                }
                for (k = len - 1; k >= 0; --k) {
                    if (encoder[encoder.size() - 1 - k] == 2)
                        tempRoot->sonVec.append(tempVec[k]);
                    else if (encoder[encoder.size() - 1 - k] == 3)
                        tempRoot->broVec.append(tempVec[k]);
                }
                nodeStk.append(tempRoot);
                // 3. 获取此时栈顶状态经过规约符号的转移，重复对to.flag的判断
                int _idx = analyseStk.back().first.toInt() - 1;
                matchForAnalyse = symbol;
                to = tb[_idx][symbol];
            }
            // 接受输入
            else {
                QMessageBox::information(nullptr, "提示", "接受输入！",
                                         QMessageBox::Ok);
                // 语法树：将节点栈剩下的元素当作程序开始节点start的子节点
                root->sonVec.append(nodeStk.back());
                nodeStk.pop_back();
                showSyntaxTree();
                nextStep = true;
                goto show;
            }
        }
        if (protocol) i--;  // 如果有规约操作则需重新拿输入的字符
        addAnalyseRow(model, analyseStk, pairs.mid(i), row++);
    }
show:
    ui->processTable->setModel(model);
    ui->processTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->processTable->resizeColumnsToContents();
    ui->processTable->resizeRowsToContents();
}

/*!
    @Function       reset
    @Description  清空项目任务二相关
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::reset() {
    pairs.clear();
    treeEncoder.clear();
    if (root) {
        delete root;
        root = nullptr;
    }
    root = new SyntaxTreeNode("START", "start");

    QStandardItemModel *model =
        static_cast<QStandardItemModel *>(ui->lexTable->model());
    if (model) model->clear();
    model = static_cast<QStandardItemModel *>(ui->processTable->model());
    if (model) model->clear();
    model = static_cast<QStandardItemModel *>(ui->treeView->model());
    if (model) model->clear();
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
    qDebug() << task2.lalr1.conflict;
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

/*!
    @Function       dfs
    @Description  渲染语法树的辅助函数
    @Parameter  语法树节点t，当前节点的父节点parent
    @Return
    @Attention
*/
void TaskTwoWidget::dfs(SyntaxTreeNode *t, QStandardItem *parent) {
    if (!t) return;
    QStandardItem *item = new QStandardItem;
    item->setEditable(false);
    item->setText(t->token.type + ": " + t->token.value);
    if (t->token.value != EPSILON) parent->appendRow(item);
    for (int i = 0; i < t->sonVec.size(); ++i) dfs(t->sonVec[i], item);
    for (int i = 0; i < t->broVec.size(); ++i) dfs(t->broVec[i], parent);
}

/*!
    @Function       showSyntaxTree
    @Description  展示语法树
    @Parameter
    @Return
    @Attention
*/
void TaskTwoWidget::showSyntaxTree() {
    QStandardItemModel *model = new QStandardItemModel(ui->treeView);
    QStandardItem *item = new QStandardItem;
    item->setText(root->token.type + ": " + root->token.value);
    item->setEditable(false);
    model->appendRow(item);
    dfs(root->sonVec[0], item);
    ui->treeView->setModel(model);
}
