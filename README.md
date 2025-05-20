<h1 align="center">编译原理课程项目任务</h1>

>📍未完成选做部分（最终成绩69分，仅供参考）

>📍本项目基于Qt5开发，如编译本项目需保证Qt版本一致

## 项目说明

本项目是基于C++和Qt框架，使用qmake作为构建系统来创建的。它整合了Qt的核心模块、GUI模块，并且兼容Qt 5及以上版本，通过qmake的配置文件（.pro）来管理项目结构和构建过程。

## 项目运行

1. 方式一：双击当前目录下 byyl.pro 文件会自动使用 Qt Creator 打开该项目，重新编译项目运行即可本地运行
2. 方式二：打开 exe 目录，直接双击运行 byyl.exe 可执行文件也可直接启动本项目

## 目录说明

```bash
root
├─ README.md # 源代码说明文档
├─ byyl.pro
├─ byyl.pro.user
├─ main.cpp
├─ taskone # 项目任务一相关数据结构实现
│	├─ common.h # 项目任务一一些小型数据结构的定义
│	├─ dfa.cpp # DFA数据结构成员函数的实现
│	├─ dfa.h # DFA的数据结构定义
│	├─ nfa.cpp # NFA数据结构成员函数的实现
│	├─ nfa.h # NFA的数据结构定义
│	├─ solutionone.cpp # 任务一解决方案类的实现
│	└─ solutionone.h # 项目任务一解决方案类，将解决项目任务一的算法作为其成员项目
├─ tasktwo
│	├─ analysetable.cpp # LALR(1)分析表相关数据结构的成员函数实现
│	├─ analysetable.h # LALR(1)分析表相关的数据结构定义
│	├─ lr.cpp # lr.h头文件中部分数据结构成员函数的实现
│	├─ lr.h	# LR(1)、LALR(1)相关数据结构的定义
│	├─ macro.h # 项目任务二解决方案相关的宏定义
│	├─ solutiontwo.cpp # 项目任务二解决方案类的实现
│	├─ solutiontwo.h # 项目任务二解决方案类，将解决项目任务二的算法作为其成员
│	├─ syntaxtreenode.cpp # 语法树相关数据结构成员函数的实现
│	└─ syntaxtreenode.h # 语法树相关数据结构的定义
├─ test
│    ├─ minic
│    │    ├─ mini-c语言单词正则表达式.txt
│    │    ├─ mini-c语言文法规则.txt
│    │    ├─ mini-c语言文法规则语法树编码.txt
│    │    ├─ mini-c语言测试源程序.mic
│    │    ├─ mini-c语言测试源程序词法分析结果.lex
│    │    └─ mini-c语言生成的词法分析源程序结果.cpp
│    ├─ tiny
│    │    ├─ tiny语言单词正则表达式.txt
│    │    ├─ tiny语言文法规则.txt
│    │    ├─ tiny语言文法规则语法树编码.txt
│    │    ├─ tiny语言测试源程序.tny
│    │    ├─ tiny语言测试源程序词法分析结果.lex
│    │    └─ tiny语言生成的词法分析源程序结果.cpp
│    └─ unit
│         ├─ 单元测试单词正则表达式.txt
│         ├─ 单元测试文法规则.txt
│         ├─ 单元测试文法规则语法树编码.txt
│         ├─ 单元测试源程序.unit
│         ├─ 单元测试源程序词法分析结果.lex
│         └─ 单元测试生成的词法分析源程序结果.cpp
├─ util
│	├─ utils.cpp # Util类静态成员函数的具体实现
│	└─ utils.h	# 常用操作的函数封装为Util类的静态成员
├─ exe
│	├─ byyl.exe # 系统可执行文件
├─ widgets
│	├─ taskonewidget.cpp # 项目任务一窗口类实现
│	├─ taskonewidget.h # 项目任务一窗口类定义
│	├─ taskonewidget.ui # 项目任务一窗口类UI文件
│	├─ tasktwowidget.cpp # 项目任务二窗口类实现
│	├─ tasktwowidget.h # 项目任务二窗口类定义
│	└─ tasktwowidget.ui # 项目任务二窗口类UI文件
└─ windows
 	├─ mainwindow.cpp
 	├─ mainwindow.h
 	└─ mainwindow.ui
```

