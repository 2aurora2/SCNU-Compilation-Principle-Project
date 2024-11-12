/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    lr.cpp
*  @brief   lr.h头文件中部分数据结构成员函数的实现
*
*  @author  林江荣
*  @date    2024-11-11
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "lr.h"

LR::LR() : size(0) {}

/*!
    @Function       isConflict
    @Description    判断LALR(1)是否有移进-规约冲突或者规约-规约冲突
    @Parameter
    @Return 是否有冲突
    @Attention 会保存冲突的类型到conflict成员变量
*/
bool LR::isConflict() {
    for (auto it = stateHash.begin(); it != stateHash.end(); ++it) {
        State state = it.key();
        QSet<QString> footIn;    // 获取移进项的下一个字符
        QSet<QString> callback;  // 获取规约项的向前看字符
        for (auto item : state.st) {
            if (item.pos == item.rule.size()) {  // 规约项
                for (auto next : item.next) {
                    if (callback.contains(next)) {  // 向前看字符有冲突
                        conflict = "规约-规约冲突";
                        return true;
                    } else
                        callback.insert(next);
                }
            } else {  // 移进项
                footIn.insert(item.rule[item.pos]);
            }
        }
        for (auto i : footIn) {
            if (callback.contains(i)) {  // 移进-规约冲突
                conflict = "移进-规约冲突";
                return true;
            }
        }
    }
    return false;
}

/*!
    @Function       qHash
    @Description  为 Item 类型提供 qHash 特化版本
    @Parameter
    @Return
    @Attention
*/
uint qHash(const Item &item) {
    return qHash(item.name) + qHash(item.rule) + qHash(item.next) +
           qHash(item.pos);
}

/*!
    @Function       qHash
    @Description  为 State 类型提供 qHash 特化版本
    @Parameter
    @Return
    @Attention
*/
uint qHash(const State &state) { return qHash(state.st); }

/*!
    @Function       haveSameCore
    @Description  判断Item之间的核心是否一致
    @Parameter  另一个Item对象
    @Return 是否一致true/false
    @Attention
*/
bool Item::haveSameCore(const Item &other) const {
    if (other.rule.size() != rule.size()) return false;
    for (auto symbol : rule)
        if (!other.rule.contains(symbol)) return false;
    return name == other.name && pos == other.pos;
}

/*!
    @Function       printItem
    @Description  返回Item对象的字符串表示
    @Parameter
    @Return Item对象的字符串表示
    @Attention
*/
QString Item::printItem() const {
    QString print = "[ " + name + " → ";
    for (int i = 0; i < rule.size(); ++i) {
        if (pos == i) print += ".";
        print += rule[i];
        if (pos != rule.size() - 1) print += " ";
    }
    if (pos == rule.size()) print += ".";
    print += ", ";
    for (auto it = next.begin(); it != next.end(); ++it) {
        print += *it;
        if (it != next.end() - 1) print += " / ";
    }
    print += " ]";
    return print;
}

/*!
    @Function       printRule
    @Description  返回Item对象规则的字符串表示
    @Parameter
    @Return Item对象规则的字符串表示
    @Attention
*/
QString Item::printRule() const {
    QString print = name + " → ";
    for (int i = 0; i < rule.size(); ++i) {
        print += rule[i];
        if (i < rule.size() - 1) print += " ";
    }
    return print;
}

/*!
    @Function       haveSameCore
    @Description  判断State之间的核心是否一致
    @Parameter  另一个State对象
    @Return 是否一致true/false
    @Attention
*/
bool State::haveSameCore(const State &other) const {
    if (st.size() != other.st.size()) return false;
    for (auto it = st.begin(); it != st.end(); ++it) {
        bool flag = false;
        for (auto _it = other.st.begin(); _it != other.st.end(); ++_it)
            if (it->haveSameCore(*_it)) {
                flag = true;
                break;
            }
        if (!flag) return false;
    }
    return true;
}

/*!
    @Function       printItem
    @Description  返回State对象的字符串表示
    @Parameter
    @Return State对象的字符串表示
    @Attention
*/
QString State::printState() {
    QString print;
    for (QSet<Item>::iterator i = st.begin(); i != st.end(); ++i) {
        print += i->printItem();
        if (i != st.end() - 1) print += "\n";
    }
    return print;
}
