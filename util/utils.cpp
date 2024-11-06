/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    utils.cpp
*  @brief   Util类静态成员函数的具体实现
*
*  @author  林江荣
*  @date    2024-10-26
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include "utils.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

/*!
    @Function   ReadFile
    @Description    读取文件内容并返回文件内容的字符串
    @Parameter      无
    @Return     文件的内容字符串
    @Attention      如果没有选择文件或打开文件失败会返回空字符串
*/
QString Util::ReadFile() {
    QString appDirPath = QCoreApplication::applicationDirPath();
    // 获取选择的文件路径
    QString filePath =
        QFileDialog::getOpenFileName(nullptr, "选择文件", appDirPath, "");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString content = stream.readAll();
            file.close();
            return content;
        }
    }
    // 如果没有选择文件或打开文件失败返回空字符串
    return "";
}

/*!
    @Function   SaveFile
    @Description    将文本内容保存为txt文件
    @Parameter  content 保存文本内容 folderPath 保存的文件夹名 filename文件名
    @Return 保存的绝对路径
    @Attention
*/
bool Util::SaveFile(QString content, QString filename) {
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
        if (filename.contains("sample")) return true;
        QMessageBox::information(nullptr, "提示",
                                 filename + "文件保存 " + "成功！",
                                 QMessageBox::Yes);
        return true;
    } else {
        QMessageBox::warning(nullptr, "提示", "文件保存失败！",
                             QMessageBox::Yes);
        return false;
    }
}
