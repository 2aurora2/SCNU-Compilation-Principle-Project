/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    solutionone.h
*  @brief   项目任务一解决方案类，将解决项目任务一的算法作为其成员
*
*  @author  林江荣
*  @date    2024-11-05
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include <QMap>
#include <QVector>

#include "common.h"
#include "dfa.h"
#include "nfa.h"

#ifndef SOLUTIONONE_H
#define SOLUTIONONE_H

class SolutionOne {
private:
    QString removeSquareBrackets(QString initial);
    QString bracketToOr(QString input);
    QString addConnectSymbol(QString input);
    QVector<Node> regexToPostfix(QString regex);
    void postfixToNFA(QVector<Node> postfix, NFA &nfa);

public:
    SolutionOne();

    QVector<QString> regexs;   // 按行存储输入的正则表达式
    QVector<QChar> operators;  // 正则表达式支持的运算符
    QMap<QString, QString> trans;  // 待转换正则表达式，键为名称，值为正则表达式
    QMap<QString, QVector<Node>> postfixs;  // 待转换正则表达式对应后缀表达式
    QMap<QString, NFA> nfas;  // 待转换正则表达式对应NFA对象
    QMap<QString, DFA> dfas;  // 待转换正则表达式对应DFA对象
    QMap<QString, DFA> mindfas;  // 待转换正则表达式对应最小化DFA对象

    void analyseRegex();
    void preProcessing();
    void nfaProcessing();
    void dfaProcessing();
    void mindfaProcessing();
};

#endif  // SOLUTIONONE_H
