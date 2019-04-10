/********************************************************************************
** Form generated from reading UI file 'SelectCpDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTCPDLG_H
#define UI_SELECTCPDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SelectCpDlg
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton_ok;
    QLabel *label;
    QSpinBox *point_number;
    QPushButton *pushButton_exit;

    void setupUi(QDialog *SelectCpDlg)
    {
        if (SelectCpDlg->objectName().isEmpty())
            SelectCpDlg->setObjectName(QString::fromUtf8("SelectCpDlg"));
        SelectCpDlg->resize(155, 91);
        gridLayout = new QGridLayout(SelectCpDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_ok = new QPushButton(SelectCpDlg);
        pushButton_ok->setObjectName(QString::fromUtf8("pushButton_ok"));

        gridLayout->addWidget(pushButton_ok, 1, 1, 1, 1);

        label = new QLabel(SelectCpDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        point_number = new QSpinBox(SelectCpDlg);
        point_number->setObjectName(QString::fromUtf8("point_number"));
        point_number->setMaximum(59);

        gridLayout->addWidget(point_number, 0, 1, 1, 1);

        pushButton_exit = new QPushButton(SelectCpDlg);
        pushButton_exit->setObjectName(QString::fromUtf8("pushButton_exit"));

        gridLayout->addWidget(pushButton_exit, 1, 0, 1, 1);


        retranslateUi(SelectCpDlg);

        QMetaObject::connectSlotsByName(SelectCpDlg);
    } // setupUi

    void retranslateUi(QDialog *SelectCpDlg)
    {
        SelectCpDlg->setWindowTitle(QApplication::translate("SelectCpDlg", "Dialog", nullptr));
        pushButton_ok->setText(QApplication::translate("SelectCpDlg", "\347\241\256\345\256\232", nullptr));
        label->setText(QApplication::translate("SelectCpDlg", "\346\216\247\345\210\266\347\202\271\345\217\267\357\274\232", nullptr));
        pushButton_exit->setText(QApplication::translate("SelectCpDlg", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectCpDlg: public Ui_SelectCpDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTCPDLG_H
