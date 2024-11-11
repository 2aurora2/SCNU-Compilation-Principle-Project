/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    analysetable.h
*  @brief   LALR(1)分析表相关的数据结构定义
*
*  @author  林江荣
*  @date    2024-11-11
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#ifndef ANALYSETABLE_H
#define ANALYSETABLE_H

#include <QHash>
#include <QString>
#include <QVector>

#include "lr.h"

struct Cell {
    int flag;  // 1代表移进，2代表规约，3代表accept
    int num;   // 移进的状态编号或者规约的规则编号
};

class AnalyseTable {
public:
    AnalyseTable();

    QVector<QString> formula;             // 存储有序的规约规则
    QHash<int, QHash<QString, Cell>> tb;  // 存储表

    void init(LR lalr1, QHash<QString, QVector<QVector<QString>>> f);
};

#endif  // ANALYSETABLE_H
