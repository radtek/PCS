/*!
 * *****************************************************************************
 * Copyright (c) 2018 Nanjing Xuanyong Techology Co.,Ltd
 * All rights reserved.
 *
 * @file    XYNavListView.cpp
 *
 * @example
 *
 * @brief
 *  需要使用xyNavListView::readData载入XML
 *  XML格式为：
 *      <?xml version="1.0" encoding="UTF-8"?>
 *      <root>
 *          <layout>
 *              <Node label="label" collapse="0" image=":/image"></Node>
 *          </layout>
 *      </root>
 *
 * @date 2018-04-01 @author zhuangming          @note create this file
 * *****************************************************************************
 */

#include "XYNavListView.h"

XYNavListView::XYNavListView(QWidget *parent)
    : QListView(parent)
    , model(new XYNavModel(this))
    , delegate(new XYNavDelegate(this))
    , infoVisible(true)
    , lineVisible(true)
    , iconColorBg(false)
    , iconStyle(XYNavListView::IconStyle_Cross)
    , colorView(QColor(239, 241, 250))
    , colorLine(QColor(214, 216, 224))
    , colorParentNormal(QColor(247, 249, 255))
    , colorParentSelected(QColor(133, 153, 216))
    , colorParentHover(QColor(209, 216, 240))
    , colorChildNormal(QColor(239, 241, 250))
    , colorChildSelected(QColor(133, 153, 216))
    , colorChildHover(QColor(209, 216, 240))
    , colorTextNormal(QColor(58, 58, 58))
    , colorTextSelected(QColor(255, 255, 255))
    , colorTextHover(QColor(59, 59, 59))
    , fontParent(QFont("Microsoft Yahei", 11))
    , fontChild(QFont("Microsoft Yahei", 10))

{
    setStyleSheet(QString("QListView{background-color:rgb(239, 241, 250);}"));
    setMouseTracking(true);
    connect(this, SIGNAL(doubleClicked(QModelIndex)), model, SLOT(collapse(QModelIndex)));
}

XYNavListView::~XYNavListView()
{
    delete delegate;
    delete model;
}

void XYNavListView::readData(const QString &xmlPath)
{
    model->readData(xmlPath);
    this->setModel(model);
    this->setItemDelegate(delegate);
}

void XYNavListView::setInfoVisible(bool infoVisible)
{
    this->infoVisible = infoVisible;
}

void XYNavListView::setLineVisible(bool lineVisible)
{
    this->lineVisible = lineVisible;
}

void XYNavListView::setIconColorBg(bool iconColorBg)
{
    this->iconColorBg = iconColorBg;
}

void XYNavListView::setIconStyle(IconStyle style)
{
    this->iconStyle = style;
}

void XYNavListView::setColorView(const QColor &colorView)
{
    this->colorView = colorView;
}

void XYNavListView::setColorLine(const QColor &colorLine)
{
    this->colorLine = colorLine;
}

void XYNavListView::setColorParent(const QColor &colorParentNormal, const QColor &colorParentSelected, const QColor &colorParentHover)
{
    this->colorParentNormal = colorParentNormal;
    this->colorParentSelected = colorParentSelected;
    this->colorParentHover = colorParentHover;
}

void XYNavListView::setColorChild(const QColor &colorChildNormal, const QColor &colorChildSelected, const QColor &colorChildHover)
{
    this->colorChildNormal = colorChildNormal;
    this->colorChildSelected = colorChildSelected;
    this->colorChildHover = colorChildHover;
}

void XYNavListView::setColorText(const QColor &colorTextNormal, const QColor &colorTextSelected, const QColor &colorTextHover)
{
    this->colorTextNormal = colorTextNormal;
    this->colorTextSelected = colorTextSelected;
    this->colorTextHover = colorTextHover;
}

// -----------------------------------------------------------------------------

XYNavModel::XYNavModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

XYNavModel::~XYNavModel()
{
    for (auto parent : treeNode)
    {
        for (auto child : parent->children)
        {
            delete child;
        }

        parent->children.clear();
        delete parent;
    }

    treeNode.clear();
    listNode.clear();
}

void XYNavModel::readData(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    QDomDocument doc;

    if (!doc.setContent(&file, false))
    {
        file.close();
        return;
    }
    file.close();

    treeNode.clear();
    listNode.clear();

    QDomNode root = doc.documentElement().firstChildElement("layout");
    QDomNodeList children = root.childNodes();

    for (int i = 0; i != children.count(); ++i)
    {
        QDomElement nodeInfo = children.at(i).toElement();

        TreeNode *node = new TreeNode;
        node->label = nodeInfo.attribute("label");
        node->info = nodeInfo.attribute("info");
        node->image = nodeInfo.attribute("image");
        node->collapse = nodeInfo.attribute("collapse").toInt();
        node->child = false;
        node->last = true;

        QDomNodeList secondLevel = nodeInfo.childNodes();

        for (int j = 0; j != secondLevel.count(); ++j)
        {
            QDomElement secNodeInfo = secondLevel.at(j).toElement();

            TreeNode *secNode = new TreeNode;
            secNode->label = secNodeInfo.attribute("label");
            secNode->info = secNodeInfo.attribute("info");
            secNode->image = secNodeInfo.attribute("image");
            secNode->collapse = false;
            secNode->child = true;
            secNode->last = (j == secondLevel.count() - 1) &&
                            (i != children.count() - 1);

            node->children.push_back(secNode);
        }

        treeNode.push_back(node);
    }

    refreshList();
    beginResetModel();
    endResetModel();
}

int XYNavModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return listNode.size();
}

QVariant XYNavModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= listNode.size() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return listNode.at(index.row())->label;
    }
    else if (role == Qt::UserRole)
    {
        return QVariant::fromValue(listNode.at(index.row())->treeNode);
    }

    return QVariant();
}

void XYNavModel::refreshList()
{
    listNode.clear();

    for (auto parent : treeNode)
    {
        ListNode *node = new ListNode;
        node->label = parent->label;
        node->treeNode = parent;

        listNode.push_back(node);

        if (parent->collapse)
        {
            continue;
        }

        for (auto child : parent->children)
        {
            ListNode *node = new ListNode;
            node->label = child->label;
            node->treeNode = child;
            node->treeNode->last = false;

            listNode.push_back(node);
        }

        if (!listNode.empty())
        {
            listNode.last()->treeNode->last = true;
        }
    }
}

void XYNavModel::collapse(const QModelIndex &index)
{
    TreeNode *node = listNode.at(index.row())->treeNode;

    if (node->children.size() == 0)
    {
        return;
    }

    node->collapse = !node->collapse;

    refreshList();

    if (!node->collapse)
    {
        beginInsertRows(QModelIndex(), index.row() + 1, index.row() + node->children.size());
        endInsertRows();
    }
    else
    {
        beginRemoveRows(QModelIndex(), index.row() + 1, index.row() + node->children.size());
        endRemoveRows();
    }
}

// -----------------------------------------------------------------------------

XYNavDelegate::XYNavDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , nav(qobject_cast<XYNavListView *>(parent))
{
}

XYNavDelegate::~XYNavDelegate()
{
}

QSize XYNavDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    XYNavModel::TreeNode *node = index.data(Qt::UserRole).value<XYNavModel::TreeNode *>();

    if (!node->child)
    {
        return QSize(50, 38);
    }
    else
    {
        return QSize(50, 31);
    }
}

void XYNavDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing);

    XYNavModel::TreeNode *node = index.data(Qt::UserRole).value<XYNavModel::TreeNode *>();

    //-----------------------------绘制背景-----------------------------------//
    QColor colorBackground;

    if (!node->child)
    {
        if (option.state & QStyle::State_Selected)
        {
            colorBackground = nav->getColorParentSelected();
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            colorBackground = nav->getColorParentHover();
        }
        else
        {
            colorBackground = nav->getColorParentNormal();
        }
    }
    else
    {
        if (option.state & QStyle::State_Selected)
        {
            colorBackground = nav->getColorChildSelected();
        }
        else if (option.state & QStyle::State_MouseOver)
        {
            colorBackground = nav->getColorChildHover();
        }
        else
        {
            colorBackground = nav->getColorChildNormal();
        }
    }

    painter->fillRect(option.rect, colorBackground);

    //-----------------------------绘制伸缩图片-----------------------------------//

    if (node->children.size() != 0)
    {
        QPixmap pix(18, 16);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);
        int penWidth = 2;

        //根据采用的背景色判断
        QColor icoColorSelected;
        QColor icoColorNormal;
        QColor icoColorHover;

        if (nav->getIconColorBg())
        {
            icoColorSelected = nav->getColorParentNormal();
            icoColorNormal = nav->getColorParentSelected();
            icoColorHover = nav->getColorParentNormal();
        }
        else
        {
            icoColorSelected = nav->getColorTextSelected();
            icoColorNormal = nav->getColorTextNormal();
            icoColorHover = nav->getColorTextHover();
        }

        if (nav->getIconStyle() == XYNavListView::IconStyle_Cross)
        {
            p.setBrush(Qt::NoBrush);

            if (option.state & QStyle::State_Selected)
            {
                p.setPen(QPen(icoColorSelected, penWidth));
            }
            else if (option.state & QStyle::State_MouseOver)
            {
                p.setPen(QPen(icoColorHover, penWidth));
            }
            else
            {
                p.setPen(QPen(icoColorNormal, penWidth));
            }

            //绘制+-线条图片
            if (node->collapse)
            {
                p.drawLine(QPointF(8, 8), QPointF(18, 8));
                p.drawLine(QPointF(12, 4), QPointF(12, 12));
            }
            else
            {
                p.drawLine(QPointF(8, 8), QPointF(18, 8));
            }
        }
        else if (nav->getIconStyle() == XYNavListView::IconStyle_Triangle)
        {
            p.setPen(Qt::NoPen);

            if (option.state & QStyle::State_Selected)
            {
                p.setBrush(icoColorSelected);
            }
            else if (option.state & QStyle::State_MouseOver)
            {
                p.setBrush(icoColorHover);
            }
            else
            {
                p.setBrush(icoColorNormal);
            }

            QVector<QPointF> points;

            //绘制三角形图片
            if (node->collapse)
            {
                points.append(QPointF(6, 4));
                points.append(QPointF(6, 12));
                points.append(QPointF(16, 8));
            }
            else
            {
                points.append(QPointF(6, 4));
                points.append(QPointF(18, 4));
                points.append(QPointF(12, 10));
            }

            p.drawPolygon(points);
        }

        QPixmap img(pix);
        QRect targetRect = option.rect;
        targetRect.setWidth(16);
        targetRect.setHeight(16);
        QPoint c = option.rect.center();
        c.setX(option.rect.width() - 28);
        targetRect.moveCenter(c);
        painter->drawPixmap(targetRect, img, img.rect());
    }

    //-----------------------------绘制图标-----------------------------------//

    if (!node->image.isEmpty())
    {
        QSize size;
        int margin;
        if (node->children.size() != 0)
        {
            size.setHeight(24);
            size.setWidth(24);
            margin = 16;
        }
        else
        {
            size.setHeight(20);
            size.setWidth(20);
            margin = 36;
        }

        QPixmap pix(node->image);
        QPixmap img = pix.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect target = option.rect;
        QPoint center = option.rect.center();

        target.setSize(size);
        center.setX(target.x() + margin);
        target.moveCenter(center);
        painter->drawPixmap(target, img, img.rect());
    }
    //-----------------------------绘制条目文字-----------------------------------//

    QColor colorText;

    if (option.state & QStyle::State_Selected)
    {
        colorText = nav->getColorTextSelected();
    }
    else if (option.state & QStyle::State_MouseOver)
    {
        colorText = nav->getColorTextHover();
    }
    else
    {
        colorText = nav->getColorTextNormal();
    }

    painter->setPen(QPen(colorText));

    //-----------------------------绘制文字离左边的距离-----------------------------------//

    int margin = 0;
    QFont font;

    if (!node->child)
    {
        margin = 35;
        font = nav->getFontParent();
    }
    else
    {
        margin = 55;
        font = nav->getFontChild();
    }

    QRect rect = option.rect;
    rect.setWidth(rect.width() - margin);
    rect.setX(rect.x() + margin);

    painter->setFont(font);
    painter->drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, index.data(Qt::DisplayRole).toString());

    //-----------------------------绘制分隔符线条-----------------------------------//

    if (nav->getLineVisible())
    {
        painter->setPen(QPen(nav->getColorLine(), 1));

        if (!node->child || (node->child && node->last))
        {
            painter->drawLine(QPointF(option.rect.x(), option.rect.y() + option.rect.height()),
                              QPointF(option.rect.x() + option.rect.width(), option.rect.y() + option.rect.height()));
        }
    }
}
