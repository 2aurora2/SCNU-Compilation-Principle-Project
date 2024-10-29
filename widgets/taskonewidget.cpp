#include "taskonewidget.h"

#include <QDateTime>
#include <QMessageBox>

#include "ui_taskonewidget.h"
#include "util/utils.h"

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

    // connect信号槽
    connect(ui->uploadRegexButton, SIGNAL(clicked()), this,
            SLOT(uploadRegex()));
    connect(ui->uploadCodeButton, SIGNAL(clicked()), this, SLOT(uploadCode()));
    connect(ui->saveFileButton, SIGNAL(clicked()), this,
            SLOT(saveTextToFile()));
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
    // TODO: 保存正则表达式到某个成员，后续分析
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
    ui->textEdit->setPlainText(content);
    // TODO: 保存源代码到某个成员，后续分析
}

/*!
    @Function   saveTextToFile
    @Description  保存文本编辑框的内容到文件，以当前时间作为文件名
    @Parameter  无
    @Return 无
    @Attention  槽事件
*/
void TaskOneWidget::saveTextToFile() {
    QString content = ui->textEdit->toPlainText();
    if (content == "")
        QMessageBox::warning(nullptr, "提示", "文本为空！", QMessageBox::Yes);
    else
        Util::SaveFile(content,
                       QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
}
