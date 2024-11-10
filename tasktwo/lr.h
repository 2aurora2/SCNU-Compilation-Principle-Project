/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    lr.h
*  @brief   LR(1)、LALR(1)相关数据结构的定义
*
*  @author  林江荣
*  @date    2024-11-07
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#ifndef LR_H
#define LR_H

#include <QHash>
#include <QSet>
#include <QString>
#include <QVector>

/*!
    @ClassName  Item
    @Description  LR(1)、LALR(1)项目
*/
class Item {
public:
    Item() = default;

    QString name;
    QVector<QString> rule;
    QSet<QString> next;
    int pos;

    // 判断Item之间核心是否一样
    bool haveSameCore(const Item &other) const {
        if (other.rule.size() != rule.size()) return false;
        for (auto symbol : rule)
            if (!other.rule.contains(symbol)) return false;
        return name == other.name && pos == other.pos;
    }

    // 重载相等运算符
    bool operator==(const Item &other) const {
        if (other.rule.size() != rule.size()) return false;
        for (auto symbol : rule)
            if (!other.rule.contains(symbol)) return false;
        return name == other.name && next == other.next && pos == other.pos;
    }

    // 构建Item对象的打印
    QString printItem() const {
        QString print = "[ " + name + " → ";
        for (int i = 0; i < rule.size(); ++i) {
            if (pos == i) print += ".";
            print += rule[i] + " ";
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
};

uint qHash(const Item &item);

/*!
    @ClassName State
    @Description LR(1)、LALR(1)项目集，即DFA的一个状态
*/
class State {
public:
    State() = default;
    QSet<Item> st;

    // 判断State核心是否一样
    bool haveSameCore(const State &other) const {
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

    // 重载相等运算符
    bool operator==(const State &other) const {
        if (st.size() != other.st.size()) return false;
        for (auto item : st)
            if (!other.st.contains(item)) return false;
        return true;
    }

    // 构建State对象的打印
    QString printState() {
        QString print;
        for (QSet<Item>::iterator i = st.begin(); i != st.end(); ++i) {
            print += i->printItem();
            if (i != st.end() - 1) print += "\n";
        }
        return print;
    }
};

uint qHash(const State &state);

/*!
    @ClassName  LR
    @Description LR(1)、LALR(1)数据结构
*/
class LR {
public:
    LR();

    int size;                                    // 状态数量
    QHash<State, int> stateHash;                 // 状态到编号的映射
    QHash<int, State> stateHashT;                // 编号到状态的映射
    QHash<int, QHash<QString, int>> changeHash;  // 邻接表
};

#endif  // LR_H
