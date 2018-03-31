/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 *
 * @file    XYNavListView.h
 * @brief   折叠式导航栏
 * @version 1.0
 * @note
 *  xyNavListView   视图
 *  xyNavModel      模型
 *  xyNavDelegate   代理
 * *****************************************************************************
 */

#ifndef XYTECH_XYNAVLISTVIEW_H
#define XYTECH_XYNAVLISTVIEW_H

#include "XYWidgetExport.h"

#include <QAbstractListModel>
#include <QListView>
#include <QStyledItemDelegate>

class XYNavModel;
class XYNavDelegate;

class XYWIDGET_EXPORT XYNavListView : public QListView
{
    Q_OBJECT

public:
    enum IconStyle
    {
        IconStyle_Cross = 0,       //十字形状
        IconStyle_Triangle = 1,    //三角形状
    };

public:
    explicit XYNavListView(QWidget *parent);
    virtual ~XYNavListView();

public Q_SLOTS:
    //读取xml文件数据
    void readData(const QString &xmlPath);

    //设置是否显示提示信息
    void setInfoVisible(bool infoVisible);

    //设置是否显示间隔线条
    void setLineVisible(bool lineVisible);

    //设置伸缩图片是否采用背景色
    void setIconColorBg(bool iconColorBg);

    //设置伸缩图片样式
    void setIconStyle(IconStyle iconStyle);

    //设置各种前景色背景色选中色
    void setColorView(const QColor &colorView);
    void setColorLine(const QColor &colorLine);
    void setColorParent(const QColor &colorParentNormal, const QColor &colorParentSelected, const QColor &colorParentHover);
    void setColorChild(const QColor &colorChildNormal, const QColor &colorChildSelected, const QColor &colorChildHover);
    void setColorText(const QColor &colorTextNormal, const QColor &colorTextSelected, const QColor &colorTextHover);

private:
    XYNavModel *model;
    XYNavDelegate *delegate;

    bool infoVisible;       //是否显示提示信息
    bool lineVisible;       //是否显示分割线条
    bool iconColorBg;       //伸缩图片是否使用颜色
    IconStyle iconStyle;    //图标样式

    QColor colorView;    //背景颜色
    QColor colorLine;    //线条颜色

    QColor colorParentNormal;      //父节点正常背景色
    QColor colorParentSelected;    //父节点选中背景色
    QColor colorParentHover;       //父节点悬停背景色

    QColor colorChildNormal;      //子节点正常背景色
    QColor colorChildSelected;    //子节点选中背景色
    QColor colorChildHover;       //子节点悬停背景色

    QColor colorTextNormal;      //正常文字颜色
    QColor colorTextSelected;    //选中文字颜色
    QColor colorTextHover;       //悬停文字颜色

    QFont fontParent;
    QFont fontChild;

public:
    inline bool getInfoVisible() const
    {
        return infoVisible;
    }

    inline bool getLineVisible() const
    {
        return lineVisible;
    }

    inline bool getIconColorBg() const
    {
        return iconColorBg;
    }

    inline IconStyle getIconStyle() const
    {
        return iconStyle;
    }

    inline QColor getColorLine() const
    {
        return colorLine;
    }

    inline QColor getColorParentNormal() const
    {
        return colorParentNormal;
    }

    inline QColor getColorParentSelected() const
    {
        return colorParentSelected;
    }

    inline QColor getColorParentHover() const
    {
        return colorParentHover;
    }

    inline QColor getColorChildNormal() const
    {
        return colorChildNormal;
    }

    inline QColor getColorChildSelected() const
    {
        return colorChildSelected;
    }

    inline QColor getColorChildHover() const
    {
        return colorChildHover;
    }

    inline QColor getColorTextNormal() const
    {
        return colorTextNormal;
    }

    inline QColor getColorTextSelected() const
    {
        return colorTextSelected;
    }

    inline QColor getColorTextHover() const
    {
        return colorTextHover;
    }

    inline QFont getFontParent() const
    {
        return fontParent;
    }

    inline QFont getFontChild() const
    {
        return fontChild;
    }
};

class XYWIDGET_EXPORT XYNavModel : public QAbstractListModel
{
    Q_OBJECT

public:
    struct TreeNode
    {
        bool child;
        bool collapse;
        bool last;
        QString label;
        QString info;
        QString image;
        QList<TreeNode *> children;
    };

    struct ListNode
    {
        QString label;
        TreeNode *treeNode;
    };

public:
    explicit XYNavModel(QObject *parent);
    virtual ~XYNavModel();

protected:
    virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

private:
    void refreshList();

private:
    QVector<TreeNode *> treeNode;
    QVector<ListNode *> listNode;

public Q_SLOTS:
    void readData(const QString &filename);
    void collapse(const QModelIndex &index);
};

Q_DECLARE_METATYPE(XYNavModel::TreeNode *)

class XYWIDGET_EXPORT XYNavDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit XYNavDelegate(QObject *parent);
    virtual ~XYNavDelegate();

protected:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    XYNavListView *nav;
};

#endif
