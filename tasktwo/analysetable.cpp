/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    analysetable.cpp
*  @brief   LALR(1)分析表相关数据结构的成员函数实现
*
*  @author  林江荣
*  @date    2024-11-11
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#include "analysetable.h"

#include <QDebug>

AnalyseTable::AnalyseTable() {}

/*!
    @Function       init
    @Description   由LALR(1)以及文法规则构造分析表
    @Parameter  LALR(1)对象lalr1，文法规则f
    @Return
    @Attention
*/
void AnalyseTable::init(LR lalr1, QHash<QString, QVector<QVector<QString>>> f) {
    // 先初始化规约的规则列表
    formula.clear();
    QHashIterator<QString, QVector<QVector<QString>>> i(f);
    while (i.hasNext()) {
        i.next();
        QString nonTerminal = i.key();
        const QVector<QVector<QString>>& productionRules = i.value();
        for (int j = 0; j < productionRules.size(); ++j) {
            const QVector<QString>& rule = productionRules[j];
            QString ruleStr;
            for (int k = 0; k < rule.size(); ++k) {
                ruleStr += rule[k];
                if (k < rule.size() - 1) ruleStr += " ";
            }
            formula.append(nonTerminal + " → " + ruleStr);
        }
    }
    // 构建分析表
    tb.clear();
    Cell cell;
    for (int i = 0; i < lalr1.size; ++i) {
        QHash<QString, int> change = lalr1.changeHash[i];
        State state = lalr1.stateHashT[i];
        for (auto it = state.st.begin(); it != state.st.end(); ++it) {
            // 规约
            if (it->pos == it->rule.size()) {
                cell.flag = 2;
                QString rule = it->printRule();
                int index = formula.indexOf(rule);
                cell.num = index;
                for (auto symbol : it->next) tb[i][symbol] = cell;
            }
        }
        if (change.size()) {
            // 移进
            cell.flag = 1;
            for (auto it = change.begin(); it != change.end(); ++it) {
                cell.num = it.value();
                tb[i][it.key()] = cell;
            }
        }
    }
    // 初态碰到$则Accept
    cell.flag = 3;
    tb[0]["$"] = cell;
}
