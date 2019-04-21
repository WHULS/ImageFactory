#ifndef FORSTNERDLG_H
#define FORSTNERDLG_H

#include <QDialog>

namespace Ui {
class ForstnerDlg;
}

class ForstnerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ForstnerDlg(QWidget *parent = nullptr);
    ~ForstnerDlg();

private slots:
    void on_factorSize_valueChanged(int arg1);

    void on_searchAreaSize_valueChanged(int arg1);

    void on_tq_valueChanged(double arg1);

    void on_tw_f_valueChanged(double arg1);

signals:
    void ParamsChanged(int, int, double, double);

private:
    Ui::ForstnerDlg *ui;
    int factorSize = 9;
    int searchAreaSize = 9;
    double Tq = 0.5;
    double f = 0.5;
};

#endif // FORSTNERDLG_H
