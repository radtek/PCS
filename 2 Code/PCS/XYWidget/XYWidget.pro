# ------------------------------------------------------------------------------
# Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
# All rights reserved.
#
# Project: XYWidget
# Version: 1.0
# Brief:
#
# ------------------------------------------------------------------------------
# 2018/04/01  WeiHeng       Create this file
#
# ------------------------------------------------------------------------------

# 引用Qt库
QT += widgets sql xml

# 输出类型
TEMPLATE = lib

# 生成程序名
TARGET = XYWidget

# 引用宏
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += XYWIDGET_LIBRARY

# 生成目录
DESTDIR = $$PWD/../../bin

# 引用源文件
SOURCES += \
    XYSqlQueryModel.cpp \
    XYNavListView.cpp \
    XYLabel.cpp \
    XYComboBox.cpp \
    XYCustomLabel.cpp \
    XYCustomButton.cpp


# 引用头文件
HEADERS += \
    XYWidgetExport.h                    \
    XYSqlQueryModel.h \
    XYNavListView.h \
    XYLabel.h \
    XYComboBox.h \
    XYCustomLabel.h \
    XYCustomButton.h

