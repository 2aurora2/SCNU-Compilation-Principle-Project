/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    syntaxtreenode.h
*  @brief   语法树相关数据结构的定义
*
*  @author  林江荣
*  @date    2024-11-13
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include <QString>
#include <QVector>
#ifndef SYNTAXTREENODE_H
#define SYNTAXTREENODE_H

struct Token {
    QString type;
    QString value;
};

class SyntaxTreeNode {
public:
    SyntaxTreeNode();
    SyntaxTreeNode(QString t, QString v);

    Token token;
    QVector<SyntaxTreeNode*> broVec;
    QVector<SyntaxTreeNode*> sonVec;
};

#endif  // SYNTAXTREENODE_H
