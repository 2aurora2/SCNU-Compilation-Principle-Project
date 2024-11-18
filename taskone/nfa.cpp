/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    nfa.cpp
*  @brief   NFA类的成员函数的具体实现
*
*  @author  林江荣
*  @date    2024-11-04
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/
#include "nfa.h"

/*!
    @Function       resizeMartix
    @Description   扩大NFA邻接矩阵的大小
    @Parameter   无
    @Return        无
    @Attention
*/
void NFA::resizeMatrix() {
    QVector<QVector<QString>> originalMatrix = matrix;

    maxStateNum += additionalState;
    matrix.resize(maxStateNum);
    for (int i = 0; i < maxStateNum; ++i) matrix[i].resize(maxStateNum);
    for (int i = 0; i < maxStateNum - additionalState; ++i)
        for (int j = 0; j < maxStateNum - additionalState; ++j)
            matrix[i][j] = originalMatrix[i][j];
}

/*!
    @Function
    @Description  NFA类构造函数，进行相关成员初始化
    @Parameter
    @Return
    @Attention
*/
NFA::NFA() {
    stateNum = 0;
    maxStateNum = 100;
    additionalState = 20;
    matrix.resize(100);
    for (int i = 0; i < matrix.size(); ++i) {
        matrix[i].resize(100);
    }
}

/*!
    @Function       addOperand
    @Description  当遇到新的操作数时NFA的变化
    @Parameter  操作数
    @Return     边，包含新增状态的指向
    @Attention
*/
Edge NFA::addOperand(QString operand) {
    if (stateNum + 2 >= maxStateNum) resizeMatrix();
    int s = stateNum, e = stateNum + 1;
    stateNum += 2;
    matrix[s][e] = operand;
    return Edge(s, e);
}

/*!
    @Function       conn
    @Description  正则表达式的连接运算符对应NFA的变化
    @Parameter  两个要连接的部分（一个部分由起始编号和终止编号构成）
    @Return 合并部分后新的起始编号与终止编号（以Edge的数据结构返回）
    @Attention
*/
Edge NFA::conn(Edge e1, Edge e2) {
    matrix[e1.en][e2.sn] = EPSILON;
    return Edge(e1.sn, e2.en);
}

/*!
    @Function       either
    @Description  正则表达式的或运算符对应NFA的变化
    @Parameter  两个要或运算的部分（一个部分由起始编号和终止编号构成）
    @Return 合并部分后新的起始编号与终止编号（以Edge的数据结构返回）
    @Attention
*/
Edge NFA::either(Edge e1, Edge e2) {
    if (stateNum + 2 >= maxStateNum) resizeMatrix();
    int s = stateNum, e = stateNum + 1;
    stateNum += 2;
    matrix[s][e1.sn] = EPSILON;
    matrix[s][e2.sn] = EPSILON;
    matrix[e1.en][e] = EPSILON;
    matrix[e2.en][e] = EPSILON;
    return Edge(s, e);
}

/*!
    @Function       closure
    @Description  正则表达式的闭包运算对应NFA的变化
    @Parameter  两个要进行闭包运算的部分（一个部分由起始编号和终止编号构成）
    @Return 合并部分后新的起始编号与终止编号（以Edge的数据结构返回）
    @Attention
*/
Edge NFA::closure(Edge e1) {
    if (stateNum + 2 >= maxStateNum) resizeMatrix();
    int s = stateNum, e = stateNum + 1;
    stateNum += 2;
    matrix[s][e1.sn] = EPSILON;
    matrix[s][e] = EPSILON;
    matrix[e1.en][e1.sn] = EPSILON;
    matrix[e1.en][e] = EPSILON;
    return Edge(s, e);
}

/*!
    @Function       positiveClosure
    @Description  正则表达式的正闭包运算对应NFA的变化
    @Parameter  两个要进行正闭包运算的部分（一个部分由起始编号和终止编号构成）
    @Return 合并部分后新的起始编号与终止编号（以Edge的数据结构返回）
    @Attention
*/
Edge NFA::positiveClosure(Edge e1) {
    if (stateNum + 2 >= maxStateNum) resizeMatrix();
    int s = stateNum, e = stateNum + 1;
    stateNum += 2;
    matrix[s][e1.sn] = EPSILON;
    matrix[e1.en][e1.sn] = EPSILON;
    matrix[e1.en][e] = EPSILON;
    return Edge(s, e);
}

/*!
    @Function       optional
    @Description  正则表达式的可选运算对应NFA的变化
    @Parameter  两个要进行可选运算的部分（一个部分由起始编号和终止编号构成）
    @Return 合并部分后新的起始编号与终止编号（以Edge的数据结构返回）
    @Attention
*/
Edge NFA::optional(Edge e1) {
    if (stateNum + 2 >= maxStateNum) resizeMatrix();
    int s = stateNum, e = stateNum + 1;
    stateNum += 2;
    matrix[s][e1.sn] = EPSILON;
    matrix[s][e] = EPSILON;
    matrix[e1.en][e] = EPSILON;
    return Edge(s, e);
}

/*!
    @Function       constructEdges
    @Description  通过遍历整个邻接矩阵构造出NFA存在的边
    @Parameter
    @Return
    @Attention
*/
void NFA::constructEdges() {
    for (int i = 0; i < stateNum; ++i)
        for (int j = 0; j < stateNum; ++j)
            if (matrix[i][j].size() != 0) edges[i][matrix[i][j]].insert(j);
}

/*!
    @Function       epsilonClosure
    @Description 求NFA某个状态编号集合中所有状态编号的ε闭包
    @Parameter 待求ε闭包的编号集合引用
    @Return
    @Attention
*/
void NFA::epsilonClosure(QSet<int> &s) {
    bool flag = false;
    QSet<int> tmpSet = s;
    while (!flag) {
        flag = true;
        QSet<int> tmp;
        for (int i : tmpSet) {
            QSet<int> nums = edges[i][EPSILON];
            for (int i : nums)
                if (!s.contains(i)) {
                    tmp.insert(i);
                    flag = false;
                }
        }
        s.unite(tmp);
        tmpSet = tmp;
    }
}
