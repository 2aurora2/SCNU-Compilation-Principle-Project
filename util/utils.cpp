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
        } else {
            // 读取文件失败返回空字符串
            return "";
        }
    }
    return "";
}
