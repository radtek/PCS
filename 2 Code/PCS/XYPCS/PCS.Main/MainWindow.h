#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
public slots:
    void navbarClicked(const QModelIndex &index);
    void windowRemoved(QWidget *widget);

    QWidget *createSubWindow(const QString &info);

private:
    void initialWindow();
    void initialManager();

private:
    Ui::MainWindow *ui;

    QMap<QString, QWidget *> widgetMap;
};

#endif    // MAINWINDOW_H
