#include "WidgetMainTabbed.h"
#include "ui_WidgetMainTabbed.h"

WidgetMainTabbed::WidgetMainTabbed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetMainTabbed)
    , tabBar(new QTabBar(this))
    , stackedWidget(new QStackedWidget(this))
{
    ui->setupUi(this);

    initialWidget();
}

WidgetMainTabbed::~WidgetMainTabbed()
{
    delete ui;
}

void WidgetMainTabbed::initialWidget()
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tabBar, 0);
    vLayout->addWidget(stackedWidget, 1);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    this->setLayout(vLayout);

    tabBar->setExpanding(false);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    tabBar->setIconSize(QSize(20, 20));
    tabBar->setFont(QFont(qApp->font().family(), 11));

    stackedWidget->setFrameStyle(QFrame::StyledPanel);

    connect(tabBar, SIGNAL(tabBarClicked(int)), this, SLOT(activeWidget(int)));
    connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(removeWidget(int)));
}

void WidgetMainTabbed::appendWidget(QWidget *widget, const QString &title, const QString &image)
{
    stackedWidget->addWidget(widget);
    stackedWidget->setCurrentWidget(widget);
    int index = tabBar->addTab(QIcon(image), title);
    tabBar->setTabData(index, QVariant::fromValue(widget));
    tabBar->setCurrentIndex(index);
}

void WidgetMainTabbed::activeWidget(QWidget *widget)
{
    for (int i = 0; i < tabBar->count(); ++i)
    {
        if (widget == tabBar->tabData(i).value<QWidget *>())
        {
            tabBar->setCurrentIndex(i);
            stackedWidget->setCurrentWidget(widget);
            return;
        }
    }
}

void WidgetMainTabbed::activeWidget(int index)
{
    QWidget *widget = tabBar->tabData(index).value<QWidget *>();
    stackedWidget->setCurrentWidget(widget);
}

void WidgetMainTabbed::removeWidget(int index)
{
    QWidget *widget = tabBar->tabData(index).value<QWidget *>();
    stackedWidget->removeWidget(widget);
    tabBar->removeTab(index);

    emit removeWidget(widget);
}
