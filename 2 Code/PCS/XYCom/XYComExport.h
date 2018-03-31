/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYComExport.h
 * @brief   动态库全局包含文件
 * @version 1.0
 * @note
 *  COM控件导出
 * *****************************************************************************
 */

#ifndef XYTECH_XYCOMEXPORT_H
#define XYTECH_XYCOMEXPORT_H

#include <QtCore/qglobal.h>

#if defined(XYCOM_LIBRARY)
#define XYCOM_EXPORT Q_DECL_EXPORT
#else
#define XYCOM_EXPORT Q_DECL_IMPORT
#endif

#endif
