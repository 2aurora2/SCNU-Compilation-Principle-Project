/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    syntaxtreenode.cpp
*  @brief   语法树相关数据结构成员函数的实现
*
*  @author  林江荣
*  @date    2024-11-13
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "syntaxtreenode.h"

/*!
    @Function       SyntaxTreeNode
    @Description  SyntaxTreeNode无参构造函数
    @Parameter
    @Return
    @Attention
*/
SyntaxTreeNode::SyntaxTreeNode() {}

/*!
    @Function       SyntaxTreeNode
    @Description  SyntaxTreeNode有参构造函数
    @Parameter  节点类型t和节点值v
    @Return
    @Attention
*/
SyntaxTreeNode::SyntaxTreeNode(QString t, QString v) {
    token.type = t;
    token.value = v;
}
