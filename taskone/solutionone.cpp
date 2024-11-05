/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    solutionone.cpp
*  @brief   项目任务一解决方案类的实现
*
*  @author  林江荣
*  @date    2024-11-05
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "solutionone.h"

#include <QDebug>
#include <QStack>

#include "common.h"

/*!
    @Function       bracketsToOr
    @Description  removeSquareBrackets辅助函数-处理字符类运算符
    @Parameter  待处理的字符串
    @Return 处理完的字符串
    @Attention
*/
QString SolutionOne::bracketToOr(QString input) {
    int group = input.size() / 3;
    QString output;
    for (int i = 0; i < group; ++i) {
        int idx = 3 * i;
        for (char ch = input[idx].unicode(); ch <= input[idx + 2].unicode();
             ch++) {
            output.append(ch);
            output.append('|');
        }
    }
    return output.chopped(1);
}

inline bool isChar(QChar ch) {
    return (ch != '*' && ch != '+' && ch != '?' && ch != '|' && ch != '.' &&
            ch != '(' && ch != ')');
}

/*!
    @Function       addConnectSymbol
    @Description  预处理辅助函数-添加连接符
    @Parameter  待处理的字符串
    @Return 处理完的字符串
    @Attention
*/
QString SolutionOne::addConnectSymbol(QString input) {
    QString output;
    for (int i = 0; i < input.size();) {
        if (operators.contains(input[i]) || input[i] == '(' ||
            input[i] == ')') {
            output += input[i++];
            if (i < input.size()) {
                if ((input[i - 1] == ')' || input[i - 1] == '*' ||
                     input[i - 1] == '?' || input[i - 1] == '+') &&
                    (input[i] == '(' || isChar(input[i])))
                    output += '.';
            }
        } else if (input[i] == '\\') {
            output += input[i++];
            output += input[i++];
            if (i < input.size()) {
                if (input[i] == '(' || isChar(input[i])) output += '.';
            }
        } else {
            output += input[i++];
            if (i < input.size() && !operators.contains(input[i]) &&
                input[i] != ')')
                output.append('.');
        }
    }
    return output;
}

/*!
    @Function       regexToPostfix
    @Description  正则表达式转后缀表达式
    @Parameter    正则表达式
    @Return        后缀表达式（向量表示）
    @Attention
*/
QVector<Node> SolutionOne::regexToPostfix(QString regex) {
    QStack<QChar> stk;
    QVector<Node> output;
    struct Node tmp;
    for (int i = 0; i < regex.size();) {
        if (regex[i] == '(')
            stk.push(regex[i++]);
        else if (regex[i] == ')') {
            while (!stk.empty() && stk.top() != '(') {
                QChar ch = stk.top();
                tmp.flag = 2;
                tmp.ch = ch;
                output.push_back(tmp);
                stk.pop();
            }
            if (!stk.empty()) stk.pop();
            ++i;
        } else if (isChar(regex[i])) {
            tmp.num = "";
            if (regex[i] == '\\' && i + 1 < regex.size()) {
                ++i;
                tmp.flag = 1;
                tmp.num += regex[i++];
                output.push_back(tmp);
            } else {
                tmp.flag = 1;
                while (i < regex.size() && isChar(regex[i]))
                    tmp.num += regex[i++];
                output.push_back(tmp);
            }
        } else {
            if (regex[i] != '|' && regex[i] != '.') {
                tmp.flag = 2;
                tmp.ch = regex[i++];
                output.push_back(tmp);
            } else {
                if (stk.empty() || stk.top() == '(')
                    stk.push(regex[i++]);
                else {
                    while (!stk.empty() && stk.top() != '(') {
                        tmp.flag = 2;
                        tmp.ch = stk.top();
                        output.push_back(tmp);
                        stk.pop();
                    }
                    stk.push(regex[i++]);
                }
            }
        }
    }
    while (!stk.empty()) {
        tmp.flag = 2;
        tmp.ch = stk.top();
        output.push_back(tmp);
        stk.pop();
    }
    return output;
}

/*!
    @Function       postfixToNFA
    @Description  把某一个后缀表达式转为NFA
    @Parameter   后缀表达式postfix，NFA对象的引用
    @Return
    @Attention
*/
void SolutionOne::postfixToNFA(QVector<Node> postfix, NFA &nfa) {
    QStack<Edge> stateStk;
    for (auto item : postfix) {
        if (item.flag == 1) {
            Edge e = nfa.addOperand(item.num);
            stateStk.push(e);
        } else {
            if (item.ch == '*') {
                Edge e = stateStk.top();
                stateStk.pop();
                stateStk.push(nfa.closure(e));
            } else if (item.ch == '+') {
                Edge e = stateStk.top();
                stateStk.pop();
                stateStk.push(nfa.positiveClosure(e));
            } else if (item.ch == '?') {
                Edge e = stateStk.top();
                stateStk.pop();
                stateStk.push(nfa.optional(e));
            } else if (item.ch == '|') {
                Edge e1 = stateStk.top();
                stateStk.pop();
                Edge e2 = stateStk.top();
                stateStk.pop();
                stateStk.push(nfa.either(e1, e2));
            } else if (item.ch == '.') {
                Edge e1 = stateStk.top();
                stateStk.pop();
                Edge e2 = stateStk.top();
                stateStk.pop();
                stateStk.push(nfa.conn(e2, e1));
            }
        }
    }
    nfa.startNum = stateStk.top().sn;
    nfa.endNum = stateStk.top().en;
    // 存储所有操作数(包括EPLISON)
    for (int i = 0; i < nfa.stateNum; ++i)
        for (int j = 0; j < nfa.stateNum; ++j)
            if (nfa.matrix[i][j].size() > 0)
                nfa.operands.insert(nfa.matrix[i][j]);
}

/*!
    @Function
    @Description  SolutionOne构造函数
    @Parameter
    @Return
    @Attention
*/
SolutionOne::SolutionOne() {
    // 初始化运算符集合
    operators << QChar('*') << QChar('+') << QChar('?') << QChar('|')
              << QChar('.');
}

/*!
    @Function       removeSquareBrackets
    @Description  预处理辅助函数-消除正则表达式的中括号
    @Parameter  待处理的字符串
    @Return 处理完的字符串
    @Attention
*/
QString SolutionOne::removeSquareBrackets(QString initial) {
    QString processed;
    for (int i = 0; i < initial.size();) {
        if (initial[i] != '[') {
            if (initial[i] == '\\') processed += initial[i++];
            processed += initial[i++];
        } else {
            int j = i + 1;
            while (initial[j] != ']') ++j;
            processed += '(';
            processed += bracketToOr(initial.mid(i + 1, j - i - 1));
            processed += ')';
            i = j + 1;
        }
    }
    return processed;
}

/*!
    @Function       analyseRegex
    @Description    分析输入正则表达式：包括预处理、转NFA、NFA转DFA和DFA的最小化
    @Parameter  无
    @Return 无
    @Attention
*/
void SolutionOne::analyseRegex() {
    preProcessing();
    nfaProcessing();
    dfaProcessing();
    mindfaProcessing();
}

/*!
    @Function       preProcessing
    @Description
   正则表达式预处理：消除中括号、替换正则表达式、添加连接符、正则表达式转为后缀表达式
    @Parameter   无
    @Return        无
    @Attention
*/
void SolutionOne::preProcessing() {
    // 预处理中括号
    for (auto &regex : regexs) {
        int equalIndex = regex.indexOf('=');
        QString partAfterEqual = regex.mid(equalIndex + 1);
        QString processedPart = removeSquareBrackets(partAfterEqual);
        regex.replace(equalIndex + 1, partAfterEqual.length(), processedPart);
    }
    // 替换正则表达式
    int regexSize = regexs.size();
    for (int i = 0; i < regexSize; ++i) {
        int equalIndex = regexs[i].indexOf('=');
        QString left = regexs[i][0] == '_' ? regexs[i].mid(1, equalIndex - 1)
                                           : regexs[i].left(equalIndex);
        QString right = regexs[i].mid(equalIndex + 1);
        for (int j = i + 1; j < regexSize; ++j)
            if (regexs[j].contains(left)) regexs[j].replace(left, right);
    }
    // 添加连接符
    trans.clear();
    for (auto &regex : regexs) {
        if (regex[0] != '_') continue;
        int equalIndex = regex.indexOf('=');
        QString left = regex.mid(1, equalIndex - 1);
        QString right = regex.mid(equalIndex + 1);
        QString processedRight = addConnectSymbol(right);
        trans.insert(left, processedRight);
    }
    // 正则表达式转后缀表达式
    postfixs.clear();
    for (auto i = trans.begin(); i != trans.end(); ++i)
        postfixs.insert(i.key(), regexToPostfix(i.value()));
}

/*!
    @Function       nfaProcessing
    @Description  把读取的所有需要转为为NFA的正则表达式进行转换
    @Parameter 无
    @Return 无
    @Attention
*/
void SolutionOne::nfaProcessing() {
    nfas.clear();
    for (auto it = postfixs.begin(); it != postfixs.end(); ++it) {
        NFA nfa;
        postfixToNFA(it.value(), nfa);
        nfa.constructEdges();
        nfas.insert(it.key(), nfa);
    }
}

/*!
    @Function       dfaProcessing
    @Description  把已有的NFA均转为DFA
    @Parameter
    @Return
    @Attention
*/
void SolutionOne::dfaProcessing() {
    dfas.clear();
    for (auto it = nfas.begin(); it != nfas.end(); ++it) {
        DFA dfa;
        dfa.fromNFA(it.value());
        dfas.insert(it.key(), dfa);
    }
}

/*!
    @Function       dfaProcessing
    @Description  把已有的DFA转最小化
    @Parameter
    @Return
    @Attention
*/
void SolutionOne::mindfaProcessing() {
    mindfas.clear();
    for (auto it = dfas.begin(); it != dfas.end(); ++it) {
        DFA mindfa;
        mindfa.minDFA(it.value());
        mindfas.insert(it.key(), mindfa);
    }
}
