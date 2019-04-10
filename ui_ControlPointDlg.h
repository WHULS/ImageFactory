/********************************************************************************
** Form generated from reading UI file 'ControlPointDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPOINTDLG_H
#define UI_CONTROLPOINTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPointDlg
{
public:
    QGridLayout *gridLayout;
    QTableWidget *controlPointList;

    void setupUi(QDialog *ControlPointDlg)
    {
        if (ControlPointDlg->objectName().isEmpty())
            ControlPointDlg->setObjectName(QString::fromUtf8("ControlPointDlg"));
        ControlPointDlg->resize(454, 300);
        gridLayout = new QGridLayout(ControlPointDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        controlPointList = new QTableWidget(ControlPointDlg);
        if (controlPointList->columnCount() < 4)
            controlPointList->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        controlPointList->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        controlPointList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        controlPointList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        controlPointList->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        controlPointList->setObjectName(QString::fromUtf8("controlPointList"));

        gridLayout->addWidget(controlPointList, 0, 0, 1, 1);


        retranslateUi(ControlPointDlg);

        QMetaObject::connectSlotsByName(ControlPointDlg);
    } // setupUi

    void retranslateUi(QDialog *ControlPointDlg)
    {
        ControlPointDlg->setWindowTitle(QApplication::translate("ControlPointDlg", "Dialog", nullptr));
        QTableWidgetItem *___qtablewidgetitem = controlPointList->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ControlPointDlg", "\346\216\247\345\210\266\347\202\271\345\217\267", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = controlPointList->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ControlPointDlg", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = controlPointList->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ControlPointDlg", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = controlPointList->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ControlPointDlg", "Z", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControlPointDlg: public Ui_ControlPointDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPOINTDLG_H
