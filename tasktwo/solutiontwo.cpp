/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    solutiontwo.cpp
*  @brief   项目任务二解决方案类的实现
*
*  @author  林江荣
*  @date    2024-11-07
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "solutiontwo.h"

#include <QDebug>
#include <QMessageBox>

/*!
    @Function       printGrammar
    @Description  打印出当前文法规则用于开发时调试
    @Parameter    文法规则
    @Return
    @Attention
*/
void printGrammar(const QHash<QString, QVector<QVector<QString>>>& grammar) {
    QHashIterator<QString, QVector<QVector<QString>>> i(grammar);
    while (i.hasNext()) {
        i.next();
        QString nonTerminal = i.key();
        const QVector<QVector<QString>>& productionRules = i.value();
        qDebug() << "Non-terminal:" << nonTerminal;
        for (int j = 0; j < productionRules.size(); ++j) {
            const QVector<QString>& rule = productionRules[j];
            QString ruleStr;
            for (int k = 0; k < rule.size(); ++k) {
                ruleStr += rule[k];
                if (k < rule.size() - 1) {
                    ruleStr += " ";
                }
            }
            qDebug() << "->" << ruleStr;
        }
    }
}

/*!
    @Function       saveFormula
    @Description  将输入文法保存为产生式对应数据结构
    @Parameter   用户上传的文法文件内容
    @Return
    @Attention
*/
void SolutionTwo::saveFormula(QString input) {
    QStringList lines = input.split("\n", QString::SkipEmptyParts);
    for (int i = 0; i < lines.size(); ++i) {
        QStringList tokens = lines[i].split(' ');
        QString left = tokens[0];
        notEnd.insert(left);
        QVector<QString> right;
        if (!tokens.contains("|")) {
            if (i == 0) start = tokens[0];
            right.clear();
            for (int j = 2; j < tokens.size(); ++j) right.append(tokens[j]);
            formula[left].append(right);
        } else {
            if (i == 0) {
                start = tokens[i] + "\'";
                right.clear();
                right.append(left);
                formula[start].append(right);
            }
            right.clear();
            for (int j = 2; j < tokens.size(); ++j)
                if (tokens[j] == "|") {
                    formula[left].append(right);
                    right.clear();
                } else
                    right.append(tokens[j]);
            formula[left].append(right);
        }
    }
}

/*!
    @Function       getFirst
    @Description
   获取某个符号的First集合，对于终结符，返回其自身的集合，对于非终结符，返回first[symbol]
    @Parameter  符号：终结符或非终结符
    @Return
    @Attention
*/
QSet<QString> SolutionTwo::getFirst(QString symbol) {
    if (notEnd.contains(symbol)) return first[symbol];
    return QSet<QString>{symbol};
}

/*!
    @Function       initFirst
    @Description    构造first集合
    @Parameter
    @Return
    @Attention
*/
void SolutionTwo::initFirst() {
    // flag标记所有First集合是否发生变化，如果没变化说明求解First集合完毕
    bool flag = true;
    while (flag) {
        flag = false;
        QHashIterator<QString, QVector<QVector<QString>>> i(formula);
        while (i.hasNext()) {
            i.next();
            QString key = i.key();
            const QVector<QVector<QString>>& raws = i.value();
            for (auto& raw : raws) {
                int cur = 0;
                for (; cur < raw.size(); ++cur) {
                    // 获取遍历到的右部规则当前字符的First集合
                    QSet<QString> firstOfCur = getFirst(raw[cur]);
                    // 把该First集合中的非EPSILON且不在First[key]里面的字符新插入First[key]
                    for (QString symbol : firstOfCur) {
                        if (symbol == EPSILON || first[key].contains(symbol))
                            continue;
                        first[key].insert(symbol);
                        flag = true;
                    }
                    // 如果右部规则当前字符不包含EPSILON，则当前规则的first求解完毕，跳出循环
                    if (!firstOfCur.contains(EPSILON)) break;
                }
                // 如果遍历到最后了，说明右部规则的最后一个字符的first集合也包括EPSILON，则需要把EPSILON也插入first[key](当first[key]不存在EPSILON时)
                if (cur == raw.size() && !first[key].contains(EPSILON)) {
                    first[key].insert(EPSILON);
                    flag = true;
                }
            }
        }
    }
}

/*!
    @Function       initFollow
    @Description  构造非终结符的FOLLOW集合
    @Parameter
    @Return
    @Attention
*/
void SolutionTwo::initFollow() {
    follow[start].insert(END_FLAG);  // 文法开始符号的Follow集合包括$
    // 标记所有Follow集合是否发生变化，如果没变化说明求解Follow集合完毕
    bool flag = true;
    while (flag) {
        flag = false;
        QHashIterator<QString, QVector<QVector<QString>>> i(formula);
        while (i.hasNext()) {
            i.next();
            QString key = i.key();
            const QVector<QVector<QString>>& raws = i.value();
            for (auto& raw : raws) {
                for (int index = 0; index < raw.size(); ++index) {
                    if (!notEnd.contains(raw[index])) continue;
                    // 找当前非终结符后面整体的FIRST
                    QSet<QString> firstOfBehind;
                    int cur = index + 1;
                    for (; cur < raw.size(); cur++) {
                        QSet<QString> firstOfCur = getFirst(raw[cur]);
                        for (auto f : firstOfCur) firstOfBehind.insert(f);
                        firstOfBehind.remove(EPSILON);
                        if (!firstOfCur.contains(EPSILON)) break;
                    }
                    for (auto symbol : firstOfBehind) {
                        if (follow[raw[index]].contains(symbol)) continue;
                        follow[raw[index]].insert(symbol);
                        flag = true;
                    }
                    if (cur == raw.size()) {
                        QSet<QString> followOfCur = follow[key];
                        for (auto symbol : followOfCur) {
                            if (follow[raw[index]].contains(symbol)) continue;
                            follow[raw[index]].insert(symbol);
                            flag = true;
                        }
                    }
                }
            }
        }
    }
}

SolutionTwo::SolutionTwo() {}

/*!
    @Function       analyseGrammar
    @Description  文法分析的主程序
    @Parameter  用户上传的文法文件内容
    @Return
    @Attention
*/
void SolutionTwo::analyseGrammar(QString input) {
    // TODO: 每次重新分析之前都要清空之前的
    saveFormula(input);
    initFirst();
    initFollow();
    //    printGrammar(formula);
    QMessageBox::information(nullptr, "提示", "文法分析完成！",
                             QMessageBox::Ok);
}
