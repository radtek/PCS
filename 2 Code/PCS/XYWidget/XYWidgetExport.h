/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYWidgetExport.h
 * @brief   动态库全局包含文件
 * @version 1.0
 * @note
 *  自定义控件导出
 * *****************************************************************************
 */

#ifndef XYTECH_XYWIDGETEXPORT_H
#define XYTECH_XYWIDGETEXPORT_H

#include <QtCore/qglobal.h>

#if defined(XYWIDGET_LIBRARY)
#define XYWIDGET_EXPORT Q_DECL_EXPORT
#else
#define XYWIDGET_EXPORT Q_DECL_IMPORT
#endif

#endif
