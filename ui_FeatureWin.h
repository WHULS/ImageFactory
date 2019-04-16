/********************************************************************************
** Form generated from reading UI file 'FeatureWin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FEATUREWIN_H
#define UI_FEATUREWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FeatureWin
{
public:
    QAction *moravec_fetch;
    QAction *forstner_fetch;
    QAction *harris_fetch;
    QAction *least_square;
    QAction *correlation_index;
    QAction *open_image;
    QAction *clear_image;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *Image_Left;
    QLabel *Image_Right;
    QTableView *Information_Table;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;

    void setupUi(QMainWindow *FeatureWin)
    {
        if (FeatureWin->objectName().isEmpty())
            FeatureWin->setObjectName(QString::fromUtf8("FeatureWin"));
        FeatureWin->resize(592, 410);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FeatureWin->sizePolicy().hasHeightForWidth());
        FeatureWin->setSizePolicy(sizePolicy);
        moravec_fetch = new QAction(FeatureWin);
        moravec_fetch->setObjectName(QString::fromUtf8("moravec_fetch"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/M.png"), QSize(), QIcon::Normal, QIcon::Off);
        moravec_fetch->setIcon(icon);
        forstner_fetch = new QAction(FeatureWin);
        forstner_fetch->setObjectName(QString::fromUtf8("forstner_fetch"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/images/F.png"), QSize(), QIcon::Normal, QIcon::Off);
        forstner_fetch->setIcon(icon1);
        harris_fetch = new QAction(FeatureWin);
        harris_fetch->setObjectName(QString::fromUtf8("harris_fetch"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/images/H.png"), QSize(), QIcon::Normal, QIcon::Off);
        harris_fetch->setIcon(icon2);
        least_square = new QAction(FeatureWin);
        least_square->setObjectName(QString::fromUtf8("least_square"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/images/quxian.png"), QSize(), QIcon::Normal, QIcon::Off);
        least_square->setIcon(icon3);
        correlation_index = new QAction(FeatureWin);
        correlation_index->setObjectName(QString::fromUtf8("correlation_index"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/images/DNA.png"), QSize(), QIcon::Normal, QIcon::Off);
        correlation_index->setIcon(icon4);
        open_image = new QAction(FeatureWin);
        open_image->setObjectName(QString::fromUtf8("open_image"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/images/image.png"), QSize(), QIcon::Normal, QIcon::Off);
        open_image->setIcon(icon5);
        clear_image = new QAction(FeatureWin);
        clear_image->setObjectName(QString::fromUtf8("clear_image"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        clear_image->setIcon(icon6);
        centralwidget = new QWidget(FeatureWin);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        Image_Left = new QLabel(centralwidget);
        Image_Left->setObjectName(QString::fromUtf8("Image_Left"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(Image_Left->sizePolicy().hasHeightForWidth());
        Image_Left->setSizePolicy(sizePolicy1);
        Image_Left->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Image_Left);

        Image_Right = new QLabel(centralwidget);
        Image_Right->setObjectName(QString::fromUtf8("Image_Right"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Image_Right->sizePolicy().hasHeightForWidth());
        Image_Right->setSizePolicy(sizePolicy2);
        Image_Right->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(Image_Right);


        verticalLayout->addLayout(horizontalLayout);

        Information_Table = new QTableView(centralwidget);
        Information_Table->setObjectName(QString::fromUtf8("Information_Table"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(Information_Table->sizePolicy().hasHeightForWidth());
        Information_Table->setSizePolicy(sizePolicy3);
        Information_Table->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(Information_Table);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        FeatureWin->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FeatureWin);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 592, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        FeatureWin->setMenuBar(menubar);

        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menu->addAction(moravec_fetch);
        menu->addAction(forstner_fetch);
        menu->addAction(harris_fetch);
        menu_2->addAction(correlation_index);
        menu_2->addAction(least_square);
        menu_3->addAction(open_image);
        menu_3->addAction(clear_image);

        retranslateUi(FeatureWin);

        QMetaObject::connectSlotsByName(FeatureWin);
    } // setupUi

    void retranslateUi(QMainWindow *FeatureWin)
    {
        FeatureWin->setWindowTitle(QApplication::translate("FeatureWin", "MainWindow", nullptr));
        moravec_fetch->setText(QApplication::translate("FeatureWin", "Moravec (&M)", nullptr));
        forstner_fetch->setText(QApplication::translate("FeatureWin", "Forster (&F)", nullptr));
        harris_fetch->setText(QApplication::translate("FeatureWin", "Harris (&H)", nullptr));
        least_square->setText(QApplication::translate("FeatureWin", "\346\234\200\345\260\217\344\272\214\344\271\230 (&M)", nullptr));
        correlation_index->setText(QApplication::translate("FeatureWin", "\347\233\270\345\205\263\347\263\273\346\225\260 (&R)", nullptr));
        open_image->setText(QApplication::translate("FeatureWin", "\346\211\223\345\274\200 (&O)", nullptr));
        clear_image->setText(QApplication::translate("FeatureWin", "\346\270\205\347\251\272 (&D)", nullptr));
        Image_Left->setText(QApplication::translate("FeatureWin", "L", nullptr));
        Image_Right->setText(QApplication::translate("FeatureWin", "R", nullptr));
        menu->setTitle(QApplication::translate("FeatureWin", "\347\202\271\347\211\271\345\276\201\346\217\220\345\217\226", nullptr));
        menu_2->setTitle(QApplication::translate("FeatureWin", "\345\275\261\345\203\217\345\214\271\351\205\215", nullptr));
        menu_3->setTitle(QApplication::translate("FeatureWin", "\345\233\276\345\203\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FeatureWin: public Ui_FeatureWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FEATUREWIN_H
