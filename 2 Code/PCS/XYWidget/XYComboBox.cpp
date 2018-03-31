#include "XYComboBox.h"


XYComboBox::XYComboBox(QWidget *parent)
    : QComboBox(parent)
    , enablePopup(true)
{
}

XYComboBox::~XYComboBox()
{

}

void XYComboBox::showPopup()
{
    emit clickPopup();
    emit clickPopup(this);

    if (enablePopup)
        QComboBox::showPopup();
}
