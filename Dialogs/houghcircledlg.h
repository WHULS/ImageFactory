#ifndef HOUGHCIRCLEDLG_H
#define HOUGHCIRCLEDLG_H

#include <QDialog>

namespace Ui {
class HoughCircleDlg;
}

class HoughCircleDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HoughCircleDlg(QWidget *parent = nullptr);
    ~HoughCircleDlg();

    // 变量
    double dp = 2;
    double minDist = 50;
    double param1 = 100;
    double param2 = 30;
    int minRadius = 1;
    int maxRadius = 30;

private:
    Ui::HoughCircleDlg *ui;

signals:
    void ParamsChanged(double dp, double minDist, double param1, double param2, int minRadius, int maxRadius);
private slots:
    void on_dp_slider_valueChanged(int value);
    void on_min_dist_slider_valueChanged(int value);
    void on_param1_slider_valueChanged(int value);
    void on_param2_slider_valueChanged(int value);
    void on_min_radius_slider_valueChanged(int value);
    void on_max_radius_slider_valueChanged(int value);
};

#endif // HOUGHCIRCLEDLG_H
