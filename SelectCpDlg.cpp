#include "SelectCpDlg.h"
#include "ui_SelectCpDlg.h"

SelectCpDlg::SelectCpDlg(int cptNum, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCpDlg)
{
    ui->setupUi(this);
    ui->point_number->setValue(cptNum);
}

SelectCpDlg::~SelectCpDlg()
{
    delete ui;
}

void SelectCpDlg::on_pushButton_exit_clicked()
{
    done(-1);
}

void SelectCpDlg::on_pushButton_ok_clicked()
{
    done(ui->point_number->value());
}
