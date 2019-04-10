#include "SelectCpDlg.h"
#include "ui_SelectCpDlg.h"

SelectCpDlg::SelectCpDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCpDlg)
{
    ui->setupUi(this);
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
