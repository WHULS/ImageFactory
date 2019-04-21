/********************************************************************************
** Form generated from reading UI file 'ForstnerDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORSTNERDLG_H
#define UI_FORSTNERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ForstnerDlg
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *factorSize;
    QLabel *label_2;
    QSpinBox *searchAreaSize;
    QLabel *label_3;
    QDoubleSpinBox *tq;
    QLabel *label_4;
    QDoubleSpinBox *tw_f;

    void setupUi(QDialog *ForstnerDlg)
    {
        if (ForstnerDlg->objectName().isEmpty())
            ForstnerDlg->setObjectName(QString::fromUtf8("ForstnerDlg"));
        ForstnerDlg->resize(170, 116);
        gridLayout = new QGridLayout(ForstnerDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ForstnerDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        factorSize = new QSpinBox(ForstnerDlg);
        factorSize->setObjectName(QString::fromUtf8("factorSize"));
        factorSize->setFocusPolicy(Qt::NoFocus);
        factorSize->setMinimum(3);
        factorSize->setMaximum(101);
        factorSize->setSingleStep(2);
        factorSize->setValue(9);

        gridLayout->addWidget(factorSize, 0, 1, 1, 1);

        label_2 = new QLabel(ForstnerDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        searchAreaSize = new QSpinBox(ForstnerDlg);
        searchAreaSize->setObjectName(QString::fromUtf8("searchAreaSize"));
        searchAreaSize->setFocusPolicy(Qt::NoFocus);
        searchAreaSize->setMinimum(3);
        searchAreaSize->setMaximum(101);
        searchAreaSize->setSingleStep(2);
        searchAreaSize->setValue(9);

        gridLayout->addWidget(searchAreaSize, 1, 1, 1, 1);

        label_3 = new QLabel(ForstnerDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        tq = new QDoubleSpinBox(ForstnerDlg);
        tq->setObjectName(QString::fromUtf8("tq"));
        tq->setFocusPolicy(Qt::NoFocus);
        tq->setDecimals(3);
        tq->setMinimum(0.500000000000000);
        tq->setMaximum(0.750000000000000);
        tq->setSingleStep(0.030000000000000);

        gridLayout->addWidget(tq, 2, 1, 1, 1);

        label_4 = new QLabel(ForstnerDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        tw_f = new QDoubleSpinBox(ForstnerDlg);
        tw_f->setObjectName(QString::fromUtf8("tw_f"));
        tw_f->setFocusPolicy(Qt::NoFocus);
        tw_f->setDecimals(3);
        tw_f->setMinimum(0.500000000000000);
        tw_f->setMaximum(1.500000000000000);
        tw_f->setSingleStep(0.100000000000000);

        gridLayout->addWidget(tw_f, 3, 1, 1, 1);


        retranslateUi(ForstnerDlg);

        QMetaObject::connectSlotsByName(ForstnerDlg);
    } // setupUi

    void retranslateUi(QDialog *ForstnerDlg)
    {
        ForstnerDlg->setWindowTitle(QApplication::translate("ForstnerDlg", "Dialog", nullptr));
        label->setText(QApplication::translate("ForstnerDlg", "\347\233\256\346\240\207\345\214\272", nullptr));
        label_2->setText(QApplication::translate("ForstnerDlg", "\346\220\234\347\264\242\345\214\272", nullptr));
        label_3->setText(QApplication::translate("ForstnerDlg", "Tq(0.5-0.75)", nullptr));
        label_4->setText(QApplication::translate("ForstnerDlg", "f(0.5-1.5)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForstnerDlg: public Ui_ForstnerDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORSTNERDLG_H
