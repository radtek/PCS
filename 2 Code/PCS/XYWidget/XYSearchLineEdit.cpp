#include "XYSearchLineEdit.h"

XYSearchLineEdit::XYSearchLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    this->initForm();
    this->initConnect();
    this->translator();
}

XYSearchLineEdit::~XYSearchLineEdit()
{
}

void XYSearchLineEdit::initForm()
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    m_tbnSearch = new QToolButton(this);

    m_tbnSearch->setAutoRaise(true);
    m_tbnSearch->setCursor(Qt::PointingHandCursor);
    m_tbnSearch->setStyleSheet("QToolButton{border-image:url(:/search_dark); background:transparent;}");

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_tbnSearch);

    //设置文本边框，防止文本覆盖按钮
    setTextMargins(0, 0, 20, 0);
    setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setContentsMargins(2, 2, 4, 0);
    this->setLayout(m_mainLayout);

    setStyleSheet("height:24px; border:1px; border-radius:6px; background-color:#E1DDDD");
}

void XYSearchLineEdit::initConnect()
{
    connect(m_tbnSearch, SIGNAL(clicked(bool)), this, SLOT(slotSendSearchContent()));
    connect(this, SIGNAL(returnPressed()), this, SLOT(slotSendSearchContent()));
}

//翻译控件
void XYSearchLineEdit::translator()
{
    setPlaceholderText(QString("请输入要查找的内容"));
    setToolTip(QString("查找内容"));
    m_tbnSearch->setToolTip(QString("查找"));
}

void XYSearchLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);

    this->clear();
    this->setPlaceholderText("");
    setStyleSheet("height:24px; border:1px; border-radius:6px; background-color:#FFFFFF");
    m_tbnSearch->setStyleSheet("QToolButton{border-image:url(:search_dark); background:transparent;}");
}

void XYSearchLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    setPlaceholderText(QString("请输入要查找的内容"));
    setStyleSheet("height:24px; border:1px; border-radius:6px; background-color:#CCC7C7");
    m_tbnSearch->setStyleSheet("QToolButton{border-image:url(:search_dark); background:transparent;}");
}

void XYSearchLineEdit::slotSendSearchContent()
{
    QString str = "";
    if (this->text().isEmpty())
        str = this->placeholderText();
    else
        str = this->text();

    emit signalSearchContent(str);
}
