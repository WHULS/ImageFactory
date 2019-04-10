#ifndef SELECTCPDLG_H
#define SELECTCPDLG_H

#include <QDialog>

namespace Ui {
class SelectCpDlg;
}

class SelectCpDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCpDlg(QWidget *parent = nullptr);
    ~SelectCpDlg();

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_ok_clicked();

private:
    Ui::SelectCpDlg *ui;
};

#endif // SELECTCPDLG_H
