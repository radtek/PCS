# ------------------------------------------------------------------------------
# Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
# All rights reserved.
#
# Project: XYCom
# Version: 1.0
# Brief:
#   QtBarTender     BarTender条码打印库
# ------------------------------------------------------------------------------
# 2018/04/01  WeiHeng       Create this file
#
# ------------------------------------------------------------------------------

# 引用Qt库
QT += core axcontainer

# 输出类型
TEMPLATE = lib

# 生成程序名
TARGET = XYCom

# 引用宏
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += XYCOM_LIBRARY

# 生成目录
DESTDIR = $$PWD/../../bin

# 引用源文件
SOURCES += \
    QtBarTender.cpp                     \

# 引用头文件
HEADERS += \
    XYComExport.h                       \
    QtBarTender.h                       \
