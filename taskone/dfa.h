/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    dfa.h
*  @brief   DFA的数据结构定义
*
*  @author  林江荣
*  @date    2024-11-05
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#ifndef DFA_H
#define DFA_H

#include <QHash>
#include <QSet>

#include "nfa.h"

class DFA {
public:
    DFA();

    int stateNum;  // 状态数
    int startNum;
    QHash<int, QSet<int>> M;  // 映射：DFA状态编号 -> NFA状态编号集合
    QHash<QSet<int>, int> MT;  // 映射：NFA状态编号集合 -> 对应DFA状态编号
    QHash<int, QHash<QString, int>> G;  // DFA状态的转换
    QSet<int> endStates;                // DFA终态集合
    QSet<QString> operands;             // DFA操作数集合

    void fromNFA(NFA nfa);  // NFA转DFA
    void minDFA(DFA dfa);   // DFA最小化
    void addEndState(QSet<int> s, int en);
};

#endif  // DFA_H
