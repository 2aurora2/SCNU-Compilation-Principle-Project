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

#include "tasktwo/macro.h"
#ifndef SOLUTIONTWO_H
#define SOLUTIONTWO_H

class SolutionTwo {
private:
    void saveFormula(QString input);
    QSet<QString> getFirst(QString symbol);
    void initFirst();
    void initFollow();

public:
    SolutionTwo();

    QString start;                         // 文法开始符号
    QSet<QString> notEnd;                  // 非终结符集合
    QHash<QString, QSet<QString>> first;   // Vn到First集合的映射
    QHash<QString, QSet<QString>> follow;  // Vn到Follow集合的映射
    QHash<QString, QVector<QVector<QString>>> formula;  // 文法规则

    void analyseGrammar(QString grammar);
};

#endif  // SOLUTIONTWO_H
