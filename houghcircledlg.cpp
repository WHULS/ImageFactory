#include "houghcircledlg.h"
#include "ui_houghcircledlg.h"

HoughCircleDlg::HoughCircleDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HoughCircleDlg)
{
    ui->setupUi(this);
}

HoughCircleDlg::~HoughCircleDlg()
{
    delete ui;
}

void HoughCircleDlg::on_dp_slider_valueChanged(int value)
{
    this->dp = double(value);
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}

void HoughCircleDlg::on_min_dist_slider_valueChanged(int value)
{
    this->minDist = double(value);
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}

void HoughCircleDlg::on_param1_slider_valueChanged(int value)
{
    this->param1 = double(value);
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}

void HoughCircleDlg::on_param2_slider_valueChanged(int value)
{
    this->param2 = double(value);
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}

void HoughCircleDlg::on_min_radius_slider_valueChanged(int value)
{
    this->minRadius = value;
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}

void HoughCircleDlg::on_max_radius_slider_valueChanged(int value)
{
    this->maxRadius = value;
    emit ParamsChanged(dp, minDist, param1, param2, minRadius, maxRadius);
}
