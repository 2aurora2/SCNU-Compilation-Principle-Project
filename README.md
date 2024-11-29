<h1 align="center">编译原理课程项目任务</h1>

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
│	├─ minic
│	│	├─ minic_grammar.txt # mini-c文法规则
│	│	├─ minic_regex.txt # mini-c单词正则表达式
│	│	├─ minic_syntax_tree.txt # mini-c用于语法树生成的文法规则编码
│	│	└─ sample.mic # mini-c的测试源程序
│	└─ tiny
│	 	├─ sample.tny # tiny的测试源程序
│	 	├─ tiny_grammar.txt # tiny的文法规则
│	 	├─ tiny_inter_encode.txt # tiny用于中间代码生成的文法规则编码
│	 	├─ tiny_regex.txt # tiny的单词正则表达式
│	 	└─ tiny_syntax_tree.txt # tiny用于语法树生成的文法规则编码
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

