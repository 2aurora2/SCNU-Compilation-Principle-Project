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

#include <utils.h>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
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
void SolutionOne::postfixToNFA(QVector<Node> postfix, NFA& nfa) {
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
    QMessageBox::information(nullptr, "提示", "正则表达式分析完成！",
                             QMessageBox::Ok);
    generateCode();
}

/*!
    @Function       analyseCode
    @Description   对用户上传的源代码进行词法分析并返回结果给窗口对象展示
    @Parameter
    @Return
    @Attention
*/
QVector<QPair<QString, QString>> SolutionOne::analyseCode(QString code) {
    Util::SaveFile(code, "sample.txt");
    QString samplePath = "sample.txt";
    QString tokenizerPath = "tokenizer.cpp";

    QString sampleAbsolutePath = QFileInfo(samplePath).absoluteFilePath();
    QString tokenAbsolutePath = QFileInfo(tokenizerPath).absoluteFilePath();

    // 创建进程编译分词器以及调用分词器进行分词
    QProcess process;
    process.start("g++", QStringList()
                             << "-o" << "tokenizer" << "tokenizer.cpp");
    if (!process.waitForFinished())
        qDebug() << "Compilation failed:" << process.readAllStandardError();
    process.start("./tokenizer");
    if (!process.waitForFinished())
        qDebug() << "Execution failed:" << process.readAllStandardError();

    // 分词结果返回
    QVector<QPair<QString, QString>> pairs;
    QDir currentDir = QDir::current();
    QString filePath = currentDir.filePath("sample.lex");
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // 读取第一行的映射信息，将映射信息存储到 QMap 中
        QString mappingLine = in.readLine();
        QStringList mappingElements =
            mappingLine.split(" ", QString::SkipEmptyParts);
        QMap<QString, QString> idToTypeMap;
        for (int i = 0; i < mappingElements.size(); i += 2) {
            QString id = mappingElements.at(i);
            QString type = mappingElements.at(i + 1);
            idToTypeMap[id] = type;
        }
        while (!in.atEnd()) {
            QString line = in.readLine();
            line = line.trimmed();
            QStringList elements = line.split(" ", QString::SkipEmptyParts);
            if (elements.size() < 2) continue;
            QString tokenId = elements.at(0);
            QString tokenValue = elements.at(1);
            // 根据 tokenId 查找对应的 tokenType
            if (idToTypeMap.contains(tokenId)) {
                QString tokenType = idToTypeMap[tokenId];
                pairs.append(qMakePair(tokenType, tokenValue));
            } else
                qDebug() << "Unknown token ID:" << tokenId;
        }
        // 关闭文件
        file.close();
    } else
        qDebug() << "Unable to open the file for reading.";
    return pairs;
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
    for (auto& regex : regexs) {
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
    for (auto& regex : regexs) {
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

/*!
    @Function       mergeRanges
    @Description  封装字符范围合并的函数
    @Parameter   某个DFA状态的各个转移情况
    @Return        合并结果
    @Attention
*/
QMap<QPair<QChar, QChar>, int> mergeRanges(
    const QHash<QString, int>& transitions) {
    QMap<QPair<QChar, QChar>, int> rangeMap;
    // 定义需要合并的字符范围（按优先级从大到小排序）
    QList<QPair<QChar, QChar>> ranges = {
        qMakePair('0', '9'),  // '0'-'9'
        qMakePair('1', '9'),  // '1'-'9'
        qMakePair('a', 'z'),  // 'a'-'z'
        qMakePair('A', 'Z')   // 'A'-'Z'
    };
    for (const auto& range : ranges) {
        QChar start = range.first;
        QChar end = range.second;
        int targetState = -1;  // 初始化为无效状态
        bool isRangeValid = true;
        // 检查当前范围内的所有字符是否具有相同的目标状态
        for (int unicode = start.unicode(); unicode <= end.unicode();
             ++unicode) {
            QChar ch(unicode);
            QString key(ch);
            if (transitions.contains(key)) {
                if (targetState == -1) {
                    targetState = transitions[key];  // 记录第一个字符的目标状态
                } else if (transitions[key] != targetState) {
                    isRangeValid = false;  // 如果目标状态不一致，则范围无效
                    break;
                }
            } else {
                isRangeValid = false;  // 如果字符不在转移条件中，则范围无效
                break;
            }
        }
        // 如果范围有效，则检查是否已经被更大的范围覆盖
        if (isRangeValid) {
            bool isCovered = false;
            for (auto it = rangeMap.begin(); it != rangeMap.end(); ++it) {
                QChar existingStart = it.key().first;
                QChar existingEnd = it.key().second;
                if (start.unicode() >= existingStart.unicode() &&
                    end.unicode() <= existingEnd.unicode()) {
                    isCovered = true;  // 当前范围已被更大的范围覆盖
                    break;
                }
            }
            if (!isCovered) rangeMap.insert(range, targetState);
        }
    }
    // 添加未合并的单个字符
    for (auto it = transitions.begin(); it != transitions.end(); ++it) {
        QString key = it.key();
        if (key.length() == 1) {  // 确保是单个字符
            QChar ch = key.at(0);
            bool isMerged = false;
            // 检查字符是否已经被合并到某个范围中
            for (const auto& range : rangeMap.keys())
                if (ch.unicode() >= range.first.unicode() &&
                    ch.unicode() <= range.second.unicode()) {
                    isMerged = true;
                    break;
                }
            // 如果字符未被合并，则单独添加到 rangeMap 中
            if (!isMerged) rangeMap.insert(qMakePair(ch, ch), it.value());
        }
    }

    return rangeMap;
}

/*!
    @Function       generateCode
    @Description    生成可进行词法分析的源程序
    @Parameter
    @Return
    @Attention
*/
void SolutionOne::generateCode() {
    QVector<QString> types;
    QVector<DFA> min_dfa_list;
    if (mindfas.keys().contains("keyword")) {
        types.append("keyword");
        min_dfa_list.append(mindfas["keyword"]);
    }
    for (auto it = mindfas.begin(); it != mindfas.end(); ++it) {
        if (it.key() == "keyword") continue;
        types.append(it.key());
        min_dfa_list.append(it.value());
    }
    // 头文件
    code = "#include <iostream>\n";
    code += "#include <fstream>\n";
    code += "#include <string>\n";
    code += "#include <cctype>\n";
    code += "#include <unordered_map>\n";
    code += "using namespace std;\n\n";
    // 全局变量
    code += "ifstream src_file(\"sample.txt\", ios::in);\n";
    code += "ofstream res_file(\"sample.lex\", ios::out | ios::trunc);\n";
    code += "int read_pos;\n";
    code += "string token;\n";
    code += "string buffer;\n";
    code += "string buf_suc;\n";
    code += "string tok_suc;\n";
    code += "string buf_err;\n\n";
    // 添加 types 的映射
    code += "std::unordered_map<std::string, int> types_map = {\n";
    for (int i = 0; i < types.size(); ++i) {
        code += "\t{\"" + types[i] + "\", " + QString::number(i) + "}";
        if (i != types.size() - 1) code += ",\n";
    }
    code += "\n};\n\n";
    // 跳过空白字符函数
    code += "void ignoreSpace() {\n";
    code += "\tchar ch;\n";
    code += "\twhile(src_file.get(ch)) {\n";
    code += "\t\tread_pos++;\n";
    code += "\t\tif(ch == '\\n') read_pos++;\n";
    code += "\t\tif(!isspace(ch)) {\n";
    code += "\t\t\tread_pos--;\n";
    code += "\t\t\tsrc_file.unget();\n";
    code += "\t\t\tbreak;\n";
    code += "\t\t}\n";
    code += "\t}\n";
    code += "}\n\n";
    // 为每个最小化DFA定义一个match函数
    for (int i = 0; i < min_dfa_list.size(); ++i) {
        code += "bool match_" + types[i] + "() {\n";
        code += "\tint state = " + QString::number(min_dfa_list[i].startNum) +
                ";\n";
        code += "\tchar ch;\n";
        code += "\twhile((ch = src_file.peek()) != EOF) {\n";
        code += "\t\tswitch(state) {\n";
        for (int j = 1; j <= min_dfa_list[i].stateNum; ++j) {
            code += "\t\tcase " + QString::number(j) + ":\n";
            // 获取当前状态的转移条件
            QHash<QString, int> transitions = min_dfa_list[i].G[j];
            // 合并字符范围
            QMap<QPair<QChar, QChar>, int> rangeMap = mergeRanges(transitions);
            // 生成字符范围的 if 语句
            for (auto it = rangeMap.begin(); it != rangeMap.end(); ++it) {
                QChar start = it.key().first;
                QChar end = it.key().second;
                int targetState = it.value();
                if (start != end) {
                    // 字符范围
                    code += "\t\t\tif(ch >= '" + QString(start) +
                            "' && ch <= '" + QString(end) + "') {\n";
                    code += "\t\t\t\tstate = " + QString::number(targetState) +
                            ";\n";
                    code += "\t\t\t\tbuffer += ch;\n";
                    code += "\t\t\t\tsrc_file.get(ch);\n";
                    code += "\t\t\t\tbreak;\n";
                    code += "\t\t\t}\n";
                }
            }
            // 生成单个字符的 switch 语句
            code += "\t\t\tswitch(ch) {\n";
            for (auto it = rangeMap.begin(); it != rangeMap.end(); ++it) {
                QChar start = it.key().first;
                QChar end = it.key().second;
                int targetState = it.value();

                if (start == end) {
                    // 单个字符
                    code += "\t\t\tcase '" + QString(start) + "':\n";
                    code += "\t\t\t\tstate = " + QString::number(targetState) +
                            ";\n";
                    code += "\t\t\t\tbuffer += ch;\n";
                    code += "\t\t\t\tsrc_file.get(ch);\n";
                    code += "\t\t\t\tbreak;\n";
                }
            }
            code += "\t\t\tdefault:\n";
            if (min_dfa_list[i].endStates.contains(j)) {
                code += "\t\t\t\ttoken = \"" + types[i] + "\";\n";
                code += "\t\t\t\treturn true;\n";
            } else
                code += "\t\t\t\treturn false;\n";
            code += "\t\t\t}\n";
            code += "\t\t\tbreak;\n";
        }
        code += "\t\t}\n";
        code += "\t}\n";
        code += "\tif(state == " +
                QString::number(*min_dfa_list[i].endStates.begin()) + " ) {\n";
        code += "\t\ttoken = \"" + types[i] + "\";\n";
        code += "\t\treturn true;\n";
        code += "\t};\n";
        code += "\treturn false;\n";
        code += "}\n\n";
    }
    // 主函数
    code += "int main(void) {\n";
    // 保存token类型编码
    code += "\tres_file << ";
    for (int i = 0; i < types.size(); ++i) {
        code +=
            "\"" + QString::number(i) + "\" << \" \" << \"" + types[i] + "\"";
        if (i != types.size() - 1) code += "<< \" \" << ";
    }
    code += "<< endl;\n";
    code += "\tchar ch;\n";
    code += "\tignoreSpace();\n";
    code += "\twhile((ch = src_file.peek()) != EOF) {\n";
    code += "\t\ttok_suc.clear();\n";
    code += "\t\tbuf_suc.clear();\n";
    for (int i = 0; i < min_dfa_list.size(); ++i) {
        code += "\t\tif(!match_" + types[i] + "()) buf_err = buffer;\n";
        code += "\t\telse if(buffer.size() > buf_suc.size()) {\n";
        code += "\t\t\tbuf_suc = buffer;\n";
        code += "\t\t\ttok_suc = token;\n";
        code += "\t\t}\n";
        code += "\t\tbuffer.clear();\n";
        code += "\t\tsrc_file.seekg(read_pos, ios::beg);\n";
    }
    code += "\t\tif(buf_suc.size() == 0){ \n";
    code += "\t\t\tres_file << \"UNKNOWN: \" << buf_err << endl;\n";
    code += "\t\t\texit(1);\n";
    code += "\t\t}\n";
    code += "\t\tres_file << types_map[tok_suc] << \" \" << buf_suc << endl;\n";
    code += "\t\tread_pos += buf_suc.size();\n";
    code += "\t\tsrc_file.seekg(read_pos, ios::beg);\n";
    code += "\t\tignoreSpace();\n";
    code += "\t}\n";
    code += "\treturn 0;\n";
    code += "}";

    Util::SaveFile(code, "tokenizer.cpp");
}
