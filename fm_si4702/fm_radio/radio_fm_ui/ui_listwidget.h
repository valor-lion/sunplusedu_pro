/********************************************************************************
** Form generated from reading UI file 'listwidget.ui'
**
** Created: Wed May 8 15:37:33 2013
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTWIDGET_H
#define UI_LISTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <mylist.h>
#include <mypushbutton.h>

QT_BEGIN_NAMESPACE

class Ui_ListWidget
{
public:
    QGridLayout *gridLayout;
    MyPushButton *pushButtonUp;
    QSpacerItem *verticalSpacer_2;
    MyPushButton *pushButtonGoback;
    QSpacerItem *verticalSpacer;
    MyPushButton *pushButtonDown;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    MyList *list1;
    MyList *list2;
    MyList *list3;
    MyList *list4;
    MyList *list5;

    void setupUi(QWidget *ListWidget)
    {
        if (ListWidget->objectName().isEmpty())
            ListWidget->setObjectName(QString::fromUtf8("ListWidget"));
        ListWidget->resize(800, 480);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(170, 170, 127, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        ListWidget->setPalette(palette);
        ListWidget->setAutoFillBackground(true);
        gridLayout = new QGridLayout(ListWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButtonUp = new MyPushButton(ListWidget);
        pushButtonUp->setObjectName(QString::fromUtf8("pushButtonUp"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonUp->sizePolicy().hasHeightForWidth());
        pushButtonUp->setSizePolicy(sizePolicy);
        pushButtonUp->setMinimumSize(QSize(7, 70));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/image/up/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonUp->setIcon(icon);
        pushButtonUp->setIconSize(QSize(50, 50));

        gridLayout->addWidget(pushButtonUp, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(67, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 1, 1, 1);

        pushButtonGoback = new MyPushButton(ListWidget);
        pushButtonGoback->setObjectName(QString::fromUtf8("pushButtonGoback"));
        sizePolicy.setHeightForWidth(pushButtonGoback->sizePolicy().hasHeightForWidth());
        pushButtonGoback->setSizePolicy(sizePolicy);
        pushButtonGoback->setMinimumSize(QSize(7, 70));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/image/goback/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonGoback->setIcon(icon1);
        pushButtonGoback->setIconSize(QSize(50, 50));

        gridLayout->addWidget(pushButtonGoback, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(67, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 1, 1, 1);

        pushButtonDown = new MyPushButton(ListWidget);
        pushButtonDown->setObjectName(QString::fromUtf8("pushButtonDown"));
        sizePolicy.setHeightForWidth(pushButtonDown->sizePolicy().hasHeightForWidth());
        pushButtonDown->setSizePolicy(sizePolicy);
        pushButtonDown->setMinimumSize(QSize(70, 70));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/image/down/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonDown->setIcon(icon2);
        pushButtonDown->setIconSize(QSize(50, 50));

        gridLayout->addWidget(pushButtonDown, 4, 1, 1, 1);

        frame = new QFrame(ListWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        list1 = new MyList(frame);
        list1->setObjectName(QString::fromUtf8("list1"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(199, 199, 149, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        list1->setPalette(palette1);
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(14);
        list1->setFont(font);
        list1->setAutoFillBackground(true);

        verticalLayout->addWidget(list1);

        list2 = new MyList(frame);
        list2->setObjectName(QString::fromUtf8("list2"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        list2->setPalette(palette2);
        list2->setFont(font);
        list2->setAutoFillBackground(true);

        verticalLayout->addWidget(list2);

        list3 = new MyList(frame);
        list3->setObjectName(QString::fromUtf8("list3"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        list3->setPalette(palette3);
        list3->setFont(font);
        list3->setAutoFillBackground(true);

        verticalLayout->addWidget(list3);

        list4 = new MyList(frame);
        list4->setObjectName(QString::fromUtf8("list4"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        list4->setPalette(palette4);
        list4->setFont(font);
        list4->setAutoFillBackground(true);

        verticalLayout->addWidget(list4);

        list5 = new MyList(frame);
        list5->setObjectName(QString::fromUtf8("list5"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        list5->setPalette(palette5);
        list5->setFont(font);
        list5->setAutoFillBackground(true);

        verticalLayout->addWidget(list5);


        gridLayout->addWidget(frame, 0, 0, 5, 1);


        retranslateUi(ListWidget);

        QMetaObject::connectSlotsByName(ListWidget);
    } // setupUi

    void retranslateUi(QWidget *ListWidget)
    {
        ListWidget->setWindowTitle(QApplication::translate("ListWidget", "ListWidget", 0, QApplication::UnicodeUTF8));
        pushButtonUp->setText(QString());
        pushButtonGoback->setText(QString());
        pushButtonDown->setText(QString());
        list1->setText(QApplication::translate("ListWidget", "1.unname1 87.5", 0, QApplication::UnicodeUTF8));
        list2->setText(QApplication::translate("ListWidget", "2.unname2 97.5", 0, QApplication::UnicodeUTF8));
        list3->setText(QApplication::translate("ListWidget", "3.unname3 88.5", 0, QApplication::UnicodeUTF8));
        list4->setText(QApplication::translate("ListWidget", "4.unname4 95.5", 0, QApplication::UnicodeUTF8));
        list5->setText(QApplication::translate("ListWidget", "5.unname5 100.5", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ListWidget: public Ui_ListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTWIDGET_H
