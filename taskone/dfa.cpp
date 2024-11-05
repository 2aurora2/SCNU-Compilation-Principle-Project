/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    dfa.cpp
*  @brief   DFA数据结构成员函数的实现
*
*  @author  林江荣
*  @date    2024-11-05
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "dfa.h"

#include <QDebug>
#include <QQueue>

/*!
    @Function       DFA
    @Description  DFA类构造函数，用于初始化成员变量
    @Parameter
    @Return
    @Attention
*/
DFA::DFA() { stateNum = 0; }

/*!
    @Function       fromNFA
    @Description  NFA转DFA
    @Parameter   待转换的NFA
    @Return
    @Attention
*/
void DFA::fromNFA(NFA nfa) {
    QSet<int> startEpsilon;
    startEpsilon.insert(nfa.startNum);
    // 求出始态的eplision闭包
    nfa.epsilonClosure(startEpsilon);
    operands = nfa.operands;
    operands.remove(EPSILON);

    stateNum = 1;
    M.insert(stateNum, startEpsilon);
    MT.insert(startEpsilon, stateNum);
    addEndState(startEpsilon, nfa.endNum);

    QQueue<QSet<int>> que;
    que.push_back(startEpsilon);
    while (!que.empty()) {
        QSet<int> cur = que.front();
        que.pop_front();
        // 遍历每一个非EPSILON操作数
        for (QString operand : nfa.operands) {
            if (operand == EPSILON) continue;
            QSet<int> tmp;
            for (int i : cur) tmp.unite(nfa.edges[i][operand]);
            // 添加集合中每个元素的EPSILON闭包
            nfa.epsilonClosure(tmp);
            if (tmp.empty()) continue;
            if (!MT.contains(tmp)) {
                que.push_back(tmp);
                stateNum += 1;
                M.insert(stateNum, tmp);
                MT.insert(tmp, stateNum);
                addEndState(tmp, nfa.endNum);
            }
            if (!G.contains(MT[cur])) G[MT[cur]] = QHash<QString, int>();
            G[MT[cur]].insert(operand, MT[tmp]);
        }
    }
}

/*!
    @Function       minDFA
    @Description   实现DFA最小化
    @Parameter   待最小化DFA对象
    @Return
    @Attention
*/
void DFA::minDFA(DFA dfa) {
    // 找到非终态集合
    QSet<int> notEndState;
    for (int i = 1; i <= dfa.stateNum; ++i)
        if (!dfa.endStates.contains(i)) notEndState.insert(i);
    // 创建划分集合队列
    QQueue<QSet<int>> que;
    if (!notEndState.empty()) que.push_back(notEndState);
    if (!dfa.endStates.empty()) que.push_back(dfa.endStates);

    int lastSetNum = 0, cnt = 0;
    if (!notEndState.empty()) lastSetNum++;
    if (!dfa.endStates.empty()) lastSetNum++;

    while (true) {
        QSet<int> queItem = que.front();
        // 标记当前集合是否被划分
        bool flag = false;
        for (QString operand : dfa.operands) {
            // 存放当前弹出集合每个元素能到达的集合
            QSet<QSet<int>> finishSet;
            // 存储<到达的某个集合，对应哪些元素到达>的映射
            QHash<QSet<int>, QSet<int>> setHash;
            for (int i : queItem) {
                if (!dfa.G[i].contains(operand)) {
                    QSet<int> emptySet;
                    finishSet.insert(emptySet);
                    setHash[emptySet].insert(i);
                } else {
                    int j = dfa.G[i][operand];
                    for (QSet<int> k : que)
                        if (k.contains(j)) {
                            finishSet.insert(k);
                            setHash[k].insert(i);
                            break;
                        }
                }
            }
            if (finishSet.size() <= 1)
                continue;
            else {
                for (QSet<int> finishItem : finishSet)
                    que.push_back(setHash[finishItem]);
                flag = true;
                break;
            }
        }

        que.pop_front();
        // 没有被划分重新push进去队列
        if (!flag) que.push_back(queItem);

        cnt++;
        // 扫过一遍队列
        if (lastSetNum == cnt) {
            if (lastSetNum == que.size()) break;
            lastSetNum = que.size();
            cnt = 0;
        }
    }

    // 构造标识符集合
    operands = dfa.operands;

    // 为新的集合进行编号和mapping
    for (QSet<int> queItem : que) {
        if (queItem.empty()) continue;
        stateNum++;
        M[stateNum] = queItem;
        MT[queItem] = stateNum;
    }

    // 构造初态和终态
    for (QSet<int> queItem : que) {
        if (queItem.contains(1)) {
            startNum = MT[queItem];
            break;
        }
    }
    for (int end : dfa.endStates) {
        for (QSet<int> queItem : que) {
            if (queItem.contains(end)) {
                endStates.insert(MT[queItem]);
            }
        }
    }

    // 构造G
    for (int i = 1; i <= stateNum; ++i) {
        int j = *(M[i].begin());
        for (QString operand : operands) {
            int k = dfa.G[j][operand];
            for (int l = 1; l <= stateNum; ++l)
                if (M[l].contains(k)) {
                    G[i][operand] = l;
                    break;
                }
        }
    }
}

void DFA::addEndState(QSet<int> s, int en) {
    if (s.contains(en)) {
        endStates.insert(MT[s]);
    }
}
