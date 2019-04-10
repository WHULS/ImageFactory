/********************************************************************************
** Form generated from reading UI file 'SampleData.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLEDATA_H
#define UI_SAMPLEDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SampleData
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QListView *sampleDataList;
    QTableView *dataInfo;
    QMenuBar *menubar;

    void setupUi(QMainWindow *SampleData)
    {
        if (SampleData->objectName().isEmpty())
            SampleData->setObjectName(QString::fromUtf8("SampleData"));
        SampleData->resize(346, 302);
        centralwidget = new QWidget(SampleData);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        sampleDataList = new QListView(centralwidget);
        sampleDataList->setObjectName(QString::fromUtf8("sampleDataList"));

        gridLayout->addWidget(sampleDataList, 0, 0, 1, 1);

        dataInfo = new QTableView(centralwidget);
        dataInfo->setObjectName(QString::fromUtf8("dataInfo"));

        gridLayout->addWidget(dataInfo, 0, 1, 1, 1);

        SampleData->setCentralWidget(centralwidget);
        menubar = new QMenuBar(SampleData);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 346, 23));
        SampleData->setMenuBar(menubar);

        retranslateUi(SampleData);

        QMetaObject::connectSlotsByName(SampleData);
    } // setupUi

    void retranslateUi(QMainWindow *SampleData)
    {
        SampleData->setWindowTitle(QApplication::translate("SampleData", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SampleData: public Ui_SampleData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLEDATA_H
