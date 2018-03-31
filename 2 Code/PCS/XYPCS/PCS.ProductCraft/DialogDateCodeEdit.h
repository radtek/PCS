#ifndef DIALOGDATECODEEDIT_H
#define DIALOGDATECODEEDIT_H

#include <QDialog>

namespace Ui
{
class DialogDateCodeEdit;
}

class DialogDateCodeEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDateCodeEdit(QWidget *parent = Q_NULLPTR);
    virtual ~DialogDateCodeEdit();

    void setDateFormula(const QString &formula);

    QString getDateFormula() const;
private:
    Ui::DialogDateCodeEdit *ui;
};

#endif // DIALOGDATECODEEDIT_H
