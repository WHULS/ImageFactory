#ifndef MoravecDlg_H
#define MoravecDlg_H

#include <QDialog>

namespace Ui {
class MoravecDlg;
}

class MoravecDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MoravecDlg(QWidget *parent = nullptr);
    ~MoravecDlg();

    int factorSize = 9;
    int searchAreaSize = 9;

signals:
    void ParamsChanged(int factorSize, int searchAreaSize);

private slots:

    void on_factorSize_valueChanged(int arg1);

    void on_searchAreaSize_valueChanged(int arg1);

private:
    Ui::MoravecDlg *ui;
};

#endif // MoravecDlg_H
