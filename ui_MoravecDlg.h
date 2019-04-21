/********************************************************************************
** Form generated from reading UI file 'MoravecDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MORAVECDLG_H
#define UI_MORAVECDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_MoravecDlg
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *factorSize;
    QSpinBox *searchAreaSize;

    void setupUi(QDialog *MoravecDlg)
    {
        if (MoravecDlg->objectName().isEmpty())
            MoravecDlg->setObjectName(QString::fromUtf8("MoravecDlg"));
        MoravecDlg->resize(146, 64);
        gridLayout = new QGridLayout(MoravecDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(MoravecDlg);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(MoravecDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        factorSize = new QSpinBox(MoravecDlg);
        factorSize->setObjectName(QString::fromUtf8("factorSize"));
        factorSize->setFocusPolicy(Qt::NoFocus);
        factorSize->setReadOnly(false);
        factorSize->setMinimum(3);
        factorSize->setMaximum(101);
        factorSize->setSingleStep(2);
        factorSize->setValue(9);

        gridLayout->addWidget(factorSize, 0, 1, 1, 1);

        searchAreaSize = new QSpinBox(MoravecDlg);
        searchAreaSize->setObjectName(QString::fromUtf8("searchAreaSize"));
        searchAreaSize->setFocusPolicy(Qt::NoFocus);
        searchAreaSize->setReadOnly(false);
        searchAreaSize->setMinimum(3);
        searchAreaSize->setMaximum(101);
        searchAreaSize->setSingleStep(2);
        searchAreaSize->setValue(9);

        gridLayout->addWidget(searchAreaSize, 3, 1, 1, 1);


        retranslateUi(MoravecDlg);

        QMetaObject::connectSlotsByName(MoravecDlg);
    } // setupUi

    void retranslateUi(QDialog *MoravecDlg)
    {
        MoravecDlg->setWindowTitle(QApplication::translate("MoravecDlg", "Dialog", nullptr));
        label->setText(QApplication::translate("MoravecDlg", "\345\205\264\350\266\243\345\200\274\347\252\227\345\217\243", nullptr));
        label_2->setText(QApplication::translate("MoravecDlg", "\345\200\231\351\200\211\347\202\271\347\252\227\345\217\243", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MoravecDlg: public Ui_MoravecDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MORAVECDLG_H
