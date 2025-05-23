/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    solutiontwo.h
*  @brief   项目任务二解决方案类，将解决项目任务二的算法作为其成员
*
*  @author  林江荣
*  @date    2024-11-07
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include <QSet>
#include <QString>

#include "analysetable.h"
#include "lr.h"
#include "tasktwo/macro.h"
#ifndef SOLUTIONTWO_H
#define SOLUTIONTWO_H

class SolutionTwo {
private:
    void saveFormula(QString input);

    QSet<QString> getFirst(QString symbol);
    void initFirst();   // 构造非终结符的FIRST集合
    void initFollow();  // 构造非终结符的FOLLOW集合

    // 构造DFA的辅助函数CLOSURE和GOTO
    State CLOSURE(State I);
    State GOTO(State I, QString X);

    LR buildLR1(QString start);
    LR buildLALR1(LR lr1);

public:
    SolutionTwo();

    QString start;                         // 文法开始符号
    QSet<QString> notEnd;                  // 非终结符集合
    QHash<QString, QSet<QString>> first;   // Vn到First集合的映射
    QHash<QString, QSet<QString>> follow;  // Vn到Follow集合的映射
    QHash<QString, QVector<QVector<QString>>> formula;  // 文法规则

    LR lr1;
    LR lalr1;
    AnalyseTable tb;

    void analyseGrammar(QString grammar);
};

#endif  // SOLUTIONTWO_H
