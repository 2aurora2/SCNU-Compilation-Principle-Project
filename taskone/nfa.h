/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    nfa.h
*  @brief   NFA数据结构的定义
*
*  @author  林江荣
*  @date    2024-11-04
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#ifndef NFA_H
#define NFA_H

#include <QSet>
#include <QString>
#include <QVector>

#include "common.h"

/*!
    @ClassName  NFA
    @Description    NFA数据结构
*/
class NFA {
private:
    int additionalState;  // 状态数满后添加的状态数
    void resizeMatrix();  // 新增状态

public:
    NFA();

    int stateNum;                      // NFA状态数
    int startNum;                      // NFA初态编号，从0开始
    int endNum;                        // NFA终态编号
    int maxStateNum;                   // NFA当前可存在的最多状态数
    QSet<QString> operands;            // 正则表达式所有的操作数
    QVector<QVector<QString>> matrix;  // 邻接矩阵存储NFA状态转移

    Edge addOperand(QString operand);  // 新增操作数操作
    Edge conn(Edge e1, Edge e2);       // 连接运算
    Edge either(Edge e1, Edge e2);     // 或运算
    Edge closure(Edge e1);             // 闭包运算
    Edge positiveClosure(Edge e1);     // 正闭包运算
    Edge optional(Edge e1);            // 可选运算
};

#endif  // NFA_H
