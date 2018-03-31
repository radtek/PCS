#include "GlobalData.h"

void MessageBox_Information(const QString &text)
{
    QMessageBox::information(nullptr, "提示", text);
}

void MessageBox_Warning(const QString &text)
{
    QMessageBox::warning(nullptr, "警告", text);
}
void MessageBox_Critical(const QString &text)
{
    QMessageBox::critical(nullptr, "错误", text);
}

bool MessageBox_Question(const QString &text)
{
    return QMessageBox::Yes == QMessageBox::question(nullptr, "询问", text);
}

int MessageBox_Question3(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Cancel, "取消");
    msgBox.setButtonText(QMessageBox::Yes, "是");
    msgBox.setButtonText(QMessageBox::No, "否");
    msgBox.setWindowTitle("询问");
    msgBox.setText(text);
    return msgBox.exec();
}

void MessageBox_Critical1(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, "是");
    msgBox.setWindowTitle("错误");
    msgBox.setText(text);
    msgBox.exec();
}
