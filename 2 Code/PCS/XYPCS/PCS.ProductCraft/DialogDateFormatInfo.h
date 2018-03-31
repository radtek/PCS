#ifndef DIALOGDATEFORMATINFO_H
#define DIALOGDATEFORMATINFO_H

#include <QDialog>

namespace Ui
{
class DialogDateFormatInfo;
}

class DialogDateFormatInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDateFormatInfo(QWidget *parent = Q_NULLPTR);
    virtual ~DialogDateFormatInfo();

private:
    Ui::DialogDateFormatInfo *ui;
};

#endif // DIALOGDATEFORMATINFO_H
