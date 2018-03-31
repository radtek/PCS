#include "XYCustomButton.h"

ChildButton::ChildButton(QWidget *parent)
    : QPushButton(parent)
    , m_index(0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setStyleSheet(R"(QPushButton{ border:none; padding-left:10px; padding-right:10px; color:#0000ff; }
                  QPushButton:hover{ color:#00ff00; } QPushButton:pressed{ color:#ff0000; })");

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ChildButton::~ChildButton()
{

}

void ChildButton::setButtonText(const QString &text)
{
    setText(text);
}

void ChildButton::slotClicked()
{
    emit signalClicked(m_index);
}


XYCustomButton::XYCustomButton(QWidget *parent)
    : QWidget(parent)
    , m_row(0)
    , m_count(0)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
}

XYCustomButton::~XYCustomButton()
{

}

void XYCustomButton::addButtons(const QStringList &items)
{
    QHBoxLayout *hLayout = dynamic_cast<QHBoxLayout *>(layout());

    for (int i = 0; i < items.size(); ++i)
    {
        ChildButton *button = new ChildButton;

        m_count += i;
        button->setIndex(m_count);
        button->setButtonText(items.at(i));

        hLayout->addWidget(button);

        connect(button, SIGNAL(signalClicked(int)), this, SLOT(slotButtonClicked(int)));
    }
}

void XYCustomButton::slotButtonClicked(int index)
{
    emit signalButtonClicked(m_row, index);
}
