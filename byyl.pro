QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/windows  \
    $$PWD/util

SOURCES += \
    main.cpp \
    taskone/dfa.cpp \
    taskone/nfa.cpp \
    taskone/solutionone.cpp \
    tasktwo/analysetable.cpp \
    tasktwo/lr.cpp \
    tasktwo/solutiontwo.cpp \
    tasktwo/syntaxtreenode.cpp \
    widgets/taskonewidget.cpp \
    widgets/tasktwowidget.cpp \
    windows/mainwindow.cpp \
    util/utils.cpp

HEADERS += \
    taskone/common.h \
    taskone/dfa.h \
    taskone/nfa.h \
    taskone/solutionone.h \
    tasktwo/analysetable.h \
    tasktwo/lr.h \
    tasktwo/macro.h \
    tasktwo/solutiontwo.h \
    tasktwo/syntaxtreenode.h \
    widgets/taskonewidget.h \
    widgets/tasktwowidget.h \
    windows/mainwindow.h \
    util/utils.h

FORMS += \
    widgets/taskonewidget.ui \
    widgets/tasktwowidget.ui \
    windows/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
