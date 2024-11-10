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
#include <functional>

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
                notEnd.insert(start);
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

/*!
    @Function       CLOSURE
    @Description  构建DFA的辅助函数-通过当前State已有的项目Item构建出完整的State
    @Parameter  包含初始Item的State
    @Return 构建出的完整State
    @Attention
*/
State SolutionTwo::CLOSURE(State I) {
    State J = I;
    while (true) {
        // 遍历当前项目集中的项目
        for (Item item : I.st) {
            // 下一个字符是非终结符
            if (item.pos < item.rule.size() &&
                notEnd.contains(item.rule[item.pos])) {
                QVector<QString> beta(item.rule.begin() + item.pos + 1,
                                      item.rule.end());
                int k = 0;
                QSet<QString> firstBeta;
                // 求First(β)
                for (; k < beta.size(); ++k) {
                    for (auto f : getFirst(beta[k]))
                        if (f != EPSILON) firstBeta.insert(f);
                    if (!getFirst(beta[k]).contains(EPSILON)) break;
                }
                // k == beta.size()
                // 说明First(β)存在EPSILON或者β为空则要把先行字符集添加
                // 至此求出First(βa)
                if (k == beta.size())
                    for (auto a : item.next) firstBeta.insert(a);
                // 构造新的LR(1)项目
                for (auto rtGrammar : formula[item.rule[item.pos]]) {
                    Item newItem;
                    newItem.name = item.rule[item.pos];
                    newItem.rule = rtGrammar;
                    for (auto fb : firstBeta) newItem.next.insert(fb);
                    newItem.pos = 0;
                    // 将新的项目添加到当前项目集，若有相同核心则合并Item
                    bool flag = false;
                    auto it = J.st.begin();
                    for (; it != J.st.end(); ++it)
                        if (newItem.haveSameCore(*it)) {
                            flag = true;
                            for (auto _next : it->next)
                                newItem.next.insert(_next);
                            break;
                        }
                    if (flag) J.st.remove(*it);
                    J.st.insert(newItem);
                }
            }
        }
        if (I == J) break;
        I = J;
    }
    return J;
}

/*!
    @Function       GOTO
    @Description  构建DFA的辅助函数-返回当前State通过X能转移得到的State
    @Parameter  已有DFA状态I，转移X
    @Return 转移得到的DFA状态
    @Attention
*/
State SolutionTwo::GOTO(State I, QString X) {
    State Inew;
    for (Item item : I.st) {
        if (item.pos >= item.rule.size() || item.rule[item.pos] != X)
            continue;         // 找到符合条件的转移
        Item newItem = item;  // 构造新文法规则，仅有pos不同
        newItem.pos++;
        Inew.st.insert(newItem);
    }
    return CLOSURE(Inew);
}

/*!
    @Function       buildLR1
    @Description  构造出LR(1)的DFA
    @Parameter  开始符号
    @Return LR对象
    @Attention
*/
LR SolutionTwo::buildLR1(QString start) {
    LR lr1;
    Item firstItem;  // 构建第一个文法规则
    firstItem.name = start;
    firstItem.rule = formula[start][0];
    firstItem.next = QSet<QString>({END_FLAG});
    firstItem.pos = 0;
    State firstState;  // 构建初态
    firstState.st.insert(firstItem);
    firstState = CLOSURE(firstState);  // 装入第一条文法规则后求闭包
    lr1.stateHash[firstState] = lr1.size;
    lr1.stateHashT[lr1.size++] = firstState;

    // dfs构建LR1的函数，使用function，C++11特性
    std::function<void(State)> dfs = [&](State faState) {
        int faId = lr1.stateHash[faState];  // 获取状态对应编号
        QVector<QString> changeMethods;
        // 获取当前State的移进项有哪些
        for (auto faItem : faState.st)
            if (faItem.pos < faItem.rule.size())  // 移进项
                changeMethods.push_back(faItem.rule[faItem.pos]);
        for (QString changeMethod : changeMethods) {  // 深搜每一个转移
            State sonState = GOTO(faState, changeMethod);
            if (sonState.st.empty()) continue;
            if (lr1.stateHash.contains(sonState)) {
                int sonId = lr1.stateHash[sonState];
                lr1.changeHash[faId][changeMethod] = sonId;
            } else {
                lr1.stateHash[sonState] = lr1.size;
                lr1.stateHashT[lr1.size++] = sonState;
                int sonId = lr1.stateHash[sonState];
                lr1.changeHash[faId][changeMethod] = sonId;
                dfs(sonState);
            }
        }
    };

    dfs(firstState);
    return lr1;
}

/*!
    @Function       buildLALR1
    @Description  根据LR(1)进行合并同心项处理得到LALR(1)
    @Parameter  LR(1)对象
    @Return LALR(1)对象
    @Attention
*/
LR SolutionTwo::buildLALR1(LR lr1) {
    LR lalr1;
    int* p = new int[lr1.size];  // 并查集(合并同心项)
    for (int i = 0; i < lr1.size; i++) p[i] = i;
    std::function<int(int)> find = [&](int x) {
        if (p[x] != x) p[x] = find(p[x]);
        return p[x];
    };
    for (int i = 0; i < lr1.size - 1; i++) {  // 状态合并
        for (int j = i + 1; j < lr1.size; j++)
            if (lr1.stateHashT[i].haveSameCore(
                    lr1.stateHashT[j])) {  // 判断同心项(合并并查集)
                p[find(j)] = find(i);
            }
    }
    QHash<int, int> cntChangeSet;  // 最终合并完的所有集合根结点→LALR1状态映射
    int idx = 0;
    for (int i = 0; i < lr1.size; i++)
        if (!cntChangeSet.count(find(i))) cntChangeSet[find(i)] = idx++;
    lalr1.size = cntChangeSet.size();  // LALR1的状态数
    QHash<int, State> revlalr1StateHash;  // 构建LALR1的反向哈希：编号->状态
    for (int i = 0; i < lr1.size; i++) {
        // find(i)是i划分到的集合的根节点编号，cntChangeSet[find(i)]则是i对应LALR1中集合的编号
        if (!revlalr1StateHash.count(
                cntChangeSet[find(i)])) {  // 第一次出现的LALR1状态
            revlalr1StateHash[cntChangeSet[find(i)]] = lr1.stateHashT[i];
        } else {
            QSet<Item> mergeState;
            for (auto item : lr1.stateHashT[i].st) {  // 遍历两个状态
                for (auto lalrItem :
                     revlalr1StateHash[cntChangeSet[find(i)]].st) {
                    if (item.haveSameCore(
                            lalrItem)) {  // 两个状态中的规则同心则合并
                        for (auto t : item.next) lalrItem.next.insert(t);
                        mergeState.insert(lalrItem);
                    }
                }
            }
            revlalr1StateHash[cntChangeSet[find(i)]].st = mergeState;
        }
    }
    for (auto it = revlalr1StateHash.begin(); it != revlalr1StateHash.end();
         ++it) {  // 利用反向哈希构建正向哈希：状态->编号
        lalr1.stateHash[it.value()] = it.key();
    }
    lalr1.stateHashT = revlalr1StateHash;
    for (int i = 0; i < lr1.size; i++) {  // 构造LALR1转移集
        for (auto it = lr1.changeHash[i].begin(); it != lr1.changeHash[i].end();
             ++it) {
            auto changeMethod = it.key();
            lalr1.changeHash[cntChangeSet[find(i)]][changeMethod] =
                cntChangeSet[find(lr1.changeHash[i][changeMethod])];
        }
    }
    return lalr1;
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
    notEnd.clear();
    first.clear();
    follow.clear();
    formula.clear();

    saveFormula(input);
    initFirst();
    initFollow();
    lr1 = buildLR1(start);
    lalr1 = buildLALR1(lr1);
    QMessageBox::information(nullptr, "提示", "文法分析完成！",
                             QMessageBox::Ok);
}
