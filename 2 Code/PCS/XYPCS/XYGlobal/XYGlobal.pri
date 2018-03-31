# ------------------------------------------------------------------------------
# Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
# All rights reserved.
#
# Project: XYGlobal
# Version: 1.0
# Brief:
#   XYLogManager 用于日志管理
#   XYSqlManager 用于数据库连接管理，NamingSQL管理
# ------------------------------------------------------------------------------
# 2018/04/01  WeiHeng       Create this file
#
# ------------------------------------------------------------------------------

# 将本目录增加到包含目录
INCLUDEPATH += $$PWD

# 头文件
HEADERS += \
    $$PWD/XYLogManager.h                    \
    $$PWD/XYSqlManager.h

# 源文件
SOURCES += \
    $$PWD/XYLogManager.cpp                  \
    $$PWD/XYSqlManager.cpp
