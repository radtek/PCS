# ------------------------------------------------------------------------------
# Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
#
# Project: PCS
# Version: 1.0
# Brief:
#   开发环境Qt 5.9.4 MinGW
# ------------------------------------------------------------------------------
# 2018/04/01  WeiHeng       Create this file
#
# ------------------------------------------------------------------------------

# 引用Qt库
QT += core gui widgets xml sql network axcontainer serialport

# 生成类型
TEMPLATE = app

# 生成程序名
TARGET = PCS

# 引用宏
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT

# 生成目录
DESTDIR = $$PWD/../../bin

# 引用资源文件
RC_FILE = $$PWD/Resources/PCS.rc

# 引用Qt资源文件
RESOURCES += $$PWD/Resources/PCS.qrc

# 引用子目录
include(XYGlobal/XYGlobal.pri)
include(PCS.Main/PCS.Main.pri)


# 引用其他项目目录
INCLUDEPATH += $$PWD/../XYWidget
INCLUDEPATH += $$PWD/../XYCom

# 引用其他项目库
LIBS += $$PWD/../../bin/libXYWidget.a
LIBS += $$PWD/../../bin/libXYCom.a

# 增加预编译
#CONFIG += precompile_header
#PRECOMPILED_HEADER = PCS.Main/GlobalInc.h


