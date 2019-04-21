#include "ForstnerDlg.h"
#include "ui_ForstnerDlg.h"

ForstnerDlg::ForstnerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForstnerDlg)
{
    ui->setupUi(this);
}

ForstnerDlg::~ForstnerDlg()
{
    delete ui;
}

void ForstnerDlg::on_factorSize_valueChanged(int arg1)
{
    this->factorSize = arg1;
    emit ParamsChanged(factorSize, searchAreaSize, Tq, f);
}

void ForstnerDlg::on_searchAreaSize_valueChanged(int arg1)
{
    this->searchAreaSize = arg1;
    emit ParamsChanged(factorSize, searchAreaSize, Tq, f);
}

void ForstnerDlg::on_tq_valueChanged(double arg1)
{
    this->Tq = arg1;
    emit ParamsChanged(factorSize, searchAreaSize, Tq, f);
}

void ForstnerDlg::on_tw_f_valueChanged(double arg1)
{
    this->f = arg1;
    emit ParamsChanged(factorSize, searchAreaSize, Tq, f);
}
