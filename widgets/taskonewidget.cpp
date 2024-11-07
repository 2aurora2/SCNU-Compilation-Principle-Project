#include "taskonewidget.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>

#include "ui_taskonewidget.h"
#include "util/utils.h"

TaskOneWidget::TaskOneWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::TaskOneWidget) {
    ui->setupUi(this);
    // 设置窗口的宽和高
    this->setBaseSize(1769, 923);

    // 通过标签页管理项目任务一分析结果
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabText(0, "NFA状态转换表");
    ui->tabWidget->setTabText(1, "DFA状态转换表");
    ui->tabWidget->setTabText(2, "最小化DFA状态转换表");
    ui->tabWidget->setTabText(3, "生成的词法分析源程序");
    ui->tabWidget->setTabText(4, "源程序分词结果");

    // connect信号槽
    connect(ui->uploadRegexButton, SIGNAL(clicked()), this,
            SLOT(uploadRegex()));
    connect(ui->uploadCodeButton, SIGNAL(clicked()), this, SLOT(uploadCode()));
    connect(ui->saveFileButton, SIGNAL(clicked()), this,
            SLOT(saveTextToFile()));
    connect(ui->regexAnalyseButton, SIGNAL(clicked()), this,
            SLOT(analyseRegex()));
    connect(ui->analyseCodeButton, SIGNAL(clicked()), this,
            SLOT(analyseCode()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(const QString &)), this,
            SLOT(checkoutRegex(const QString &)));
}

TaskOneWidget::~TaskOneWidget() { delete ui; }

/*!
    @Function   uploadRegex
    @Description    上传正则表达式文件并展示内容到窗口的文本编辑框
    @Parameter  无
    @Return 无
    @Attention  槽事件
*/
void TaskOneWidget::uploadRegex() {
    QString content = Util::ReadFile();
    ui->textEdit->setPlainText(content);
}

/*!
    @Function   uploadCode
    @Description    上传源文件并展示内容到窗口的文本编辑框
    @Parameter  无
    @Return 无
    @Attention  槽事件
*/
void TaskOneWidget::uploadCode() {
    QString content = Util::ReadFile();
    ui->sourceCodeEdit->setPlainText(content);
}

/*!
    @Function   saveTextToFile
    @Description  保存文本框正则表达式的内容到文件，以当前时间作为文件名
    @Parameter  无
    @Return 无
    @Attention  槽事件
*/
void TaskOneWidget::saveTextToFile() {
    QString content = ui->textEdit->toPlainText();
    if (content == "")
        QMessageBox::warning(nullptr, "提示", "文本为空！", QMessageBox::Yes);
    else
        Util::SaveFile(content, "regex.txt");
}

/*!
    @Function       analyseRegex
    @Description  分析用户上传的正则表达式
    @Parameter   无
    @Return        无
    @Attention  槽事件
*/
void TaskOneWidget::analyseRegex() {
    QStringList lines =
        ui->textEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    if (lines.size() == 0) {
        QMessageBox::warning(nullptr, "提示", "正则表达式不能为空！",
                             QMessageBox::Ok);
        return;
    }
    // 按行保存正则表达式到下拉框内容
    task1.regexs.clear();
    for (const QString &line : lines) task1.regexs.append(line);
    // 调用项目任务一解决方案类的正则表达式分析主程序
    task1.analyseRegex();
    ui->textBrowser->setText(task1.code);
    // 填充要转换的正则表达式到下拉框中以供用户选择
    ui->comboBox->clear();
    for (const QString &line : lines)
        if (line[0] == '_') ui->comboBox->addItem(line);
}

/*!
    @Function       analyseCode
    @Description  对用户上传的源代码进行词法分析
    @Parameter
    @Return
    @Attention 槽事件
*/
void TaskOneWidget::analyseCode() {
    QString code = ui->sourceCodeEdit->toPlainText();
    QVector<QPair<QString, QString>> results = task1.analyseCode(code);

    QStandardItemModel *model = new QStandardItemModel(ui->tokenWidget);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("单词"));
    model->setHorizontalHeaderItem(1, new QStandardItem("类型"));
    for (int i = 0; i < results.size(); ++i) {
        model->setItem(i, 0, new QStandardItem(results[i].second));
        model->setItem(i, 1, new QStandardItem(results[i].first));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }
    ui->tokenWidget->setModel(model);
    ui->tokenWidget->resizeRowsToContents();
    ui->tokenWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*!
    @Function       checkoutRegex
    @Description  切换要展示状态转换表的正则表达式时触发
    @Parameter  要切换的正则表达式
    @Return
    @Attention 槽事件
*/
void TaskOneWidget::checkoutRegex(const QString &regex) {
    int equalIndex = regex.indexOf('=');
    QString left = regex.mid(1, equalIndex - 1);
    showNFATable(left);
    showDFATable(left);
    showMinDFATable(left);
}

/*!
    @Function       showNFATable
    @Description  按照选择的正则表达式展示其NFA
    @Parameter  要展示的正则表达式标识符
    @Return
    @Attention
*/
void TaskOneWidget::showNFATable(QString regex) {
    NFA nfa = task1.nfas[regex];
    QStandardItemModel *model = new QStandardItemModel(ui->NFAWidget);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("状态\\转移"));
    QList<QString> list =
        QList<QString>(nfa.operands.begin(), nfa.operands.end());
    // 设置表头
    for (int i = 0; i < list.size(); ++i)
        model->setHorizontalHeaderItem(i + 1, new QStandardItem(list.at(i)));
    // 设置状态列
    for (int i = 0; i < nfa.stateNum; ++i) {
        model->setItem(i, 0, new QStandardItem(QString::number(i)));
        model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        // 标明起始态和终态
        if (i == nfa.startNum)
            model->item(i, 0)->setBackground(QBrush(Qt::red));
        if (i == nfa.endNum)
            model->item(i, 0)->setBackground(QBrush(Qt::green));
    }
    for (int i = 0; i < nfa.stateNum; ++i) {
        for (int j = 0; j < list.size(); ++j) {
            if (list.at(j) != EPSILON) {
                for (int k = 0; k < nfa.stateNum; ++k) {
                    if (nfa.matrix[i][k] == list.at(j)) {
                        model->setItem(i, j + 1,
                                       new QStandardItem(QString::number(k)));
                        model->item(i, j + 1)->setTextAlignment(
                            Qt::AlignCenter);
                    }
                }
            } else {
                QString tmp;
                for (int k = 0; k < nfa.stateNum; ++k) {
                    if (nfa.matrix[i][k] == EPSILON) {
                        tmp += QString::number(k);
                        tmp += "、";
                    }
                }
                model->setItem(i, j + 1,
                               new QStandardItem(tmp.left(tmp.size() - 1)));
                model->item(i, j + 1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    ui->NFAWidget->setModel(model);
    ui->NFAWidget->resizeRowsToContents();
    ui->NFAWidget->resizeColumnsToContents();
    ui->NFAWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->NFAWidget->verticalHeader()->hide();
}

/*!
    @Function       showDFATable
    @Description  按照选择的正则表达式展示其DFA
    @Parameter  要展示的正则表达式标识符
    @Return
    @Attention
*/
void TaskOneWidget::showDFATable(QString regex) {
    DFA dfa = task1.dfas[regex];
    QStandardItemModel *model = new QStandardItemModel(ui->DFAWidget);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("状态集合\\转移"));
    QList<QString> list =
        QList<QString>(dfa.operands.begin(), dfa.operands.end());
    // 设置表头
    for (int i = 0; i < list.size(); ++i)
        model->setHorizontalHeaderItem(i + 1, new QStandardItem(list.at(i)));
    // 设置第一列
    for (int i = 1; i <= dfa.stateNum; ++i) {
        QString colName = "{ ";
        for (int j : dfa.M[i]) colName += QString::number(j) + ", ";
        model->setItem(
            i - 1, 0,
            new QStandardItem(colName.left(colName.size() - 1) + " }"));
        model->item(i - 1, 0)->setTextAlignment(Qt::AlignCenter);

        if (i == 1) model->item(i - 1, 0)->setBackground(QBrush(Qt::red));
        if (dfa.endStates.contains(i))
            model->item(i - 1, 0)->setBackground(QBrush(Qt::green));
    }
    for (int i = 1; i <= dfa.stateNum; ++i) {
        for (int j = 0; j < list.size(); ++j) {
            if (dfa.G.contains(i) && dfa.G[i].contains(list[j])) {
                QString label = "{ ";
                for (int k : dfa.M[dfa.G[i][list[j]]])
                    label += QString::number(k) + ", ";
                model->setItem(
                    i - 1, j + 1,
                    new QStandardItem(label.left(label.size() - 1) + " }"));
                model->item(i - 1, j + 1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    ui->DFAWidget->setModel(model);
    ui->DFAWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*!
    @Function       showMinDFATable
    @Description  按照选择的正则表达式展示其最小化DFA结果
    @Parameter  要展示的正则表达式标识符
    @Return
    @Attention
*/
void TaskOneWidget::showMinDFATable(QString regex) {
    DFA dfa = task1.mindfas[regex];
    QStandardItemModel *model = new QStandardItemModel(ui->minDFAWidget);
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("状态集合\\转移"));
    QList<QString> list =
        QList<QString>(dfa.operands.begin(), dfa.operands.end());
    // 设置表头
    for (int i = 0; i < list.size(); ++i)
        model->setHorizontalHeaderItem(i + 1, new QStandardItem(list.at(i)));
    // 设置第一列
    for (int i = 1; i <= dfa.stateNum; ++i) {
        QString colName = "{ ";
        for (int j : dfa.M[i]) colName += QString::number(j) + ", ";
        model->setItem(
            i - 1, 0,
            new QStandardItem(colName.left(colName.size() - 1) + " }"));
        model->item(i - 1, 0)->setTextAlignment(Qt::AlignCenter);

        if (i == dfa.startNum)
            model->item(i - 1, 0)->setBackground(QBrush(Qt::red));
        if (dfa.endStates.contains(i))
            model->item(i - 1, 0)->setBackground(QBrush(Qt::green));
    }
    for (int i = 1; i <= dfa.stateNum; ++i) {
        for (int j = 0; j < list.size(); ++j) {
            if (dfa.G.contains(i) && dfa.G[i].contains(list[j])) {
                QString label = "{ ";
                for (int k : dfa.M[dfa.G[i][list[j]]])
                    label += QString::number(k) + ", ";
                model->setItem(
                    i - 1, j + 1,
                    new QStandardItem(label.left(label.size() - 1) + " }"));
                model->item(i - 1, j + 1)->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
    ui->minDFAWidget->setModel(model);
    ui->minDFAWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
