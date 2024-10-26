<h1 align="center">编译原理课程项目任务</h1>

## 项目说明

本项目是基于C++和Qt框架，使用qmake作为构建系统来创建的。它整合了Qt的核心模块、GUI模块，并且兼容Qt 5及以上版本，通过qmake的配置文件（.pro）来管理项目结构和构建过程。

## 项目运行

1. 方式一：双击 byyl.pro 文件会自动使用 Qt Creator 打开该项目，重新编译项目运行即可本地运行
2. 方式二：……

## 目录说明

```bash
root
├─ .gitignore
├─ README.md	# 项目说明
├─ byyl.pro		
├─ main.cpp
├─ util		# 常用操作封装为Util接口类的相关代码实现
│    ├─ utils.cpp
│    └─ utils.h
└─ windows	# windows主窗口相关实现代码及UI文件
       ├─ mainwindow.cpp
       ├─ mainwindow.h
       └─ mainwindow.ui
```

