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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SampleData
{
public:
    QAction *read_control_point;
    QAction *open_image;
    QWidget *centralwidget;
    QListView *sampleDataList;
    QTableView *dataInfo;
    QMenuBar *menuBar;
    QMenu *menu;

    void setupUi(QMainWindow *SampleData)
    {
        if (SampleData->objectName().isEmpty())
            SampleData->setObjectName(QString::fromUtf8("SampleData"));
        SampleData->resize(570, 379);
        read_control_point = new QAction(SampleData);
        read_control_point->setObjectName(QString::fromUtf8("read_control_point"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/dingwei.png"), QSize(), QIcon::Normal, QIcon::Off);
        read_control_point->setIcon(icon);
        open_image = new QAction(SampleData);
        open_image->setObjectName(QString::fromUtf8("open_image"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        open_image->setIcon(icon1);
        centralwidget = new QWidget(SampleData);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sampleDataList = new QListView(centralwidget);
        sampleDataList->setObjectName(QString::fromUtf8("sampleDataList"));
        sampleDataList->setEnabled(true);
        sampleDataList->setGeometry(QRect(9, 9, 91, 341));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sampleDataList->sizePolicy().hasHeightForWidth());
        sampleDataList->setSizePolicy(sizePolicy);
        dataInfo = new QTableView(centralwidget);
        dataInfo->setObjectName(QString::fromUtf8("dataInfo"));
        dataInfo->setGeometry(QRect(110, 10, 451, 341));
        sizePolicy.setHeightForWidth(dataInfo->sizePolicy().hasHeightForWidth());
        dataInfo->setSizePolicy(sizePolicy);
        SampleData->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(SampleData);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 570, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        SampleData->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(open_image);
        menu->addAction(read_control_point);

        retranslateUi(SampleData);

        QMetaObject::connectSlotsByName(SampleData);
    } // setupUi

    void retranslateUi(QMainWindow *SampleData)
    {
        SampleData->setWindowTitle(QApplication::translate("SampleData", "\346\221\204\345\275\261\346\234\272\345\256\232\346\240\207", nullptr));
        read_control_point->setText(QApplication::translate("SampleData", "\350\257\273\345\217\226\346\216\247\345\210\266\347\202\271\346\226\207\344\273\266(&O)", nullptr));
        open_image->setText(QApplication::translate("SampleData", "\346\211\223\345\274\200\345\275\261\345\203\217 (&O)", nullptr));
        menu->setTitle(QApplication::translate("SampleData", "\347\233\270\346\234\272\345\256\232\346\240\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SampleData: public Ui_SampleData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLEDATA_H
