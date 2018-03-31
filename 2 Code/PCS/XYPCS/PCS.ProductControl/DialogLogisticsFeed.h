#ifndef DIALOGMATERIALFEED_H
#define DIALOGMATERIALFEED_H

#include <QDialog>
#include <QtCore>

namespace Ui
{
class DialogLogisticsFeed;
}

class DialogLogisticsFeed : public QDialog
{
    Q_OBJECT
public:
    enum class TableHeader
    {
        MaterialID,
        MaterialName,
        MaterialBatch,
        PackageBatch,
        BatchQuantity,
        BatchState,
    };

public:
    explicit DialogLogisticsFeed(QWidget *parent = 0);
    virtual ~DialogLogisticsFeed();

public slots:
    void slotFeed();
    void slotReturn();
    void slotReturnAll();

private:
    void initialWidget();

private:
    Ui::DialogLogisticsFeed *ui;

    QMap<TableHeader, QString> tableHeaderMap;
};

#endif    // DIALOGMATERIALFEED_H
