/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    common.h
*  @brief   项目任务一 一些小型数据结构的定义
*
*  @author  林江荣
*  @date    2024-11-05
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include <QChar>
#include <QString>

#ifndef COMMON_H
#define COMMON_H

#define EPSILON "ε"

// 后缀表达式单个部分的数据结构（操作数/运算符）
struct Node {
    int flag;     // 1为操作数，2为运算符
    QChar ch;     // 运算符的值
    QString num;  // 操作数的值
};

// NFA/DFA边的数据结构：包括指向编号和被指向编号
struct Edge {
    int sn, en;
    Edge(int s = 0, int e = 1) : sn(s), en(e) {}
};

#endif  // COMMON_H
