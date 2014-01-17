/********************************************************************************
** Form generated from reading UI file 'mainwidget.ui'
**
** Created: Wed May 8 15:37:33 2013
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_H
#define UI_MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include <mypushbutton.h>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QGridLayout *gridLayout_2;
    MyPushButton *pushButtonList;
    QSpacerItem *horizontalSpacer;
    QFrame *frameTip;
    QHBoxLayout *horizontalLayoutTip;
    QSpacerItem *horizontalSpacer_2;
    MyPushButton *pushButtonSave;
    QFrame *frameLeft;
    QFrame *frameRight;
    QFrame *frame_6;
    QSpacerItem *horizontalSpacer_4;
    QLabel *labelChannelFreq;
    QSpacerItem *horizontalSpacer_5;
    QFrame *frame;
    QSpacerItem *horizontalSpacer_6;
    QFrame *frameMiddle;
    QGridLayout *gridLayout;
    QLabel *labelLeft;
    QLabel *labelRight;
    QSpacerItem *horizontalSpacer_3;
    QSlider *horizontalSlider;
    QSpacerItem *horizontalSpacer_7;
    QFrame *frame_8;
    QLabel *labelChannelName;
    QFrame *frame_9;
    QFrame *frame_7;
    QFrame *frameDown;
    QHBoxLayout *horizontalLayout;
    MyPushButton *pushButtonAuto;
    QFrame *frame_2;
    MyPushButton *pushButtonPrevious;
    QFrame *frame_3;
    MyPushButton *pushButtonBegin;
    QFrame *frame_4;
    MyPushButton *pushButtonNext;
    QFrame *frame_5;
    MyPushButton *pushButtonManu;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(800, 480);
        gridLayout_2 = new QGridLayout(MainWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, -1, 0, 0);
        pushButtonList = new MyPushButton(MainWidget);
        pushButtonList->setObjectName(QString::fromUtf8("pushButtonList"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButtonList->sizePolicy().hasHeightForWidth());
        pushButtonList->setSizePolicy(sizePolicy);
        pushButtonList->setMaximumSize(QSize(66666, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/image/list/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonList->setIcon(icon);
        pushButtonList->setIconSize(QSize(50, 50));

        gridLayout_2->addWidget(pushButtonList, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(164, 55, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        frameTip = new QFrame(MainWidget);
        frameTip->setObjectName(QString::fromUtf8("frameTip"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frameTip->sizePolicy().hasHeightForWidth());
        frameTip->setSizePolicy(sizePolicy1);
        frameTip->setMinimumSize(QSize(0, 90));
        frameTip->setMaximumSize(QSize(400, 70));
        frameTip->setFrameShape(QFrame::NoFrame);
        frameTip->setFrameShadow(QFrame::Raised);
        frameTip->setLineWidth(0);
        horizontalLayoutTip = new QHBoxLayout(frameTip);
        horizontalLayoutTip->setSpacing(6);
        horizontalLayoutTip->setContentsMargins(11, 11, 11, 11);
        horizontalLayoutTip->setObjectName(QString::fromUtf8("horizontalLayoutTip"));

        gridLayout_2->addWidget(frameTip, 0, 2, 2, 1);

        horizontalSpacer_2 = new QSpacerItem(164, 55, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 3, 1, 1);

        pushButtonSave = new MyPushButton(MainWidget);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));
        sizePolicy.setHeightForWidth(pushButtonSave->sizePolicy().hasHeightForWidth());
        pushButtonSave->setSizePolicy(sizePolicy);
        pushButtonSave->setMinimumSize(QSize(0, 0));
        pushButtonSave->setMaximumSize(QSize(800, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/image/save/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonSave->setIcon(icon1);
        pushButtonSave->setIconSize(QSize(50, 50));

        gridLayout_2->addWidget(pushButtonSave, 0, 4, 1, 1);

        frameLeft = new QFrame(MainWidget);
        frameLeft->setObjectName(QString::fromUtf8("frameLeft"));
        frameLeft->setFrameShape(QFrame::NoFrame);
        frameLeft->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(frameLeft, 1, 0, 7, 1);

        frameRight = new QFrame(MainWidget);
        frameRight->setObjectName(QString::fromUtf8("frameRight"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frameRight->sizePolicy().hasHeightForWidth());
        frameRight->setSizePolicy(sizePolicy2);
        frameRight->setFrameShape(QFrame::NoFrame);
        frameRight->setFrameShadow(QFrame::Raised);

        gridLayout_2->addWidget(frameRight, 1, 4, 7, 1);

        frame_6 = new QFrame(MainWidget);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(frame_6->sizePolicy().hasHeightForWidth());
        frame_6->setSizePolicy(sizePolicy3);
        frame_6->setMinimumSize(QSize(0, 20));
        frame_6->setMaximumSize(QSize(16777215, 10));
        frame_6->setFrameShape(QFrame::NoFrame);
        frame_6->setFrameShadow(QFrame::Raised);
        frame_6->setLineWidth(0);

        gridLayout_2->addWidget(frame_6, 2, 1, 1, 3);

        horizontalSpacer_4 = new QSpacerItem(164, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 3, 1, 1, 1);

        labelChannelFreq = new QLabel(MainWidget);
        labelChannelFreq->setObjectName(QString::fromUtf8("labelChannelFreq"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(labelChannelFreq->sizePolicy().hasHeightForWidth());
        labelChannelFreq->setSizePolicy(sizePolicy4);
        labelChannelFreq->setMinimumSize(QSize(0, 70));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font.setPointSize(22);
        labelChannelFreq->setFont(font);
        labelChannelFreq->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(labelChannelFreq, 3, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(164, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 3, 3, 1, 1);

        frame = new QFrame(MainWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy3.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy3);
        frame->setMinimumSize(QSize(0, 10));
        frame->setMaximumSize(QSize(16777215, 10));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(0);

        gridLayout_2->addWidget(frame, 4, 1, 1, 3);

        horizontalSpacer_6 = new QSpacerItem(139, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 5, 1, 1, 1);

        frameMiddle = new QFrame(MainWidget);
        frameMiddle->setObjectName(QString::fromUtf8("frameMiddle"));
        frameMiddle->setMinimumSize(QSize(0, 0));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        QBrush brush1(QColor(170, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        frameMiddle->setPalette(palette);
        frameMiddle->setAutoFillBackground(false);
        frameMiddle->setFrameShape(QFrame::NoFrame);
        frameMiddle->setFrameShadow(QFrame::Raised);
        frameMiddle->setLineWidth(0);
        gridLayout = new QGridLayout(frameMiddle);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelLeft = new QLabel(frameMiddle);
        labelLeft->setObjectName(QString::fromUtf8("labelLeft"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\232\266\344\271\246"));
        font1.setPointSize(8);
        labelLeft->setFont(font1);

        gridLayout->addWidget(labelLeft, 2, 0, 1, 1);

        labelRight = new QLabel(frameMiddle);
        labelRight->setObjectName(QString::fromUtf8("labelRight"));
        labelRight->setFont(font1);

        gridLayout->addWidget(labelRight, 2, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(291, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 1, 1, 1);

        horizontalSlider = new QSlider(frameMiddle);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setEnabled(false);
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(horizontalSlider->sizePolicy().hasHeightForWidth());
        horizontalSlider->setSizePolicy(sizePolicy5);
        horizontalSlider->setMinimumSize(QSize(0, 30));
        horizontalSlider->setFocusPolicy(Qt::StrongFocus);
        horizontalSlider->setMinimum(0);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);
        horizontalSlider->setTickPosition(QSlider::TicksAbove);
        horizontalSlider->setTickInterval(3);

        gridLayout->addWidget(horizontalSlider, 1, 1, 1, 1);


        gridLayout_2->addWidget(frameMiddle, 5, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(139, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 5, 3, 1, 1);

        frame_8 = new QFrame(MainWidget);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setMinimumSize(QSize(0, 0));
        frame_8->setFrameShape(QFrame::NoFrame);
        frame_8->setFrameShadow(QFrame::Raised);
        frame_8->setLineWidth(0);

        gridLayout_2->addWidget(frame_8, 6, 1, 1, 1);

        labelChannelName = new QLabel(MainWidget);
        labelChannelName->setObjectName(QString::fromUtf8("labelChannelName"));
        sizePolicy4.setHeightForWidth(labelChannelName->sizePolicy().hasHeightForWidth());
        labelChannelName->setSizePolicy(sizePolicy4);
        labelChannelName->setMinimumSize(QSize(0, 60));
        QFont font2;
        font2.setPointSize(12);
        labelChannelName->setFont(font2);
        labelChannelName->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(labelChannelName, 6, 2, 1, 1);

        frame_9 = new QFrame(MainWidget);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        frame_9->setMinimumSize(QSize(0, 0));
        frame_9->setFrameShape(QFrame::NoFrame);
        frame_9->setFrameShadow(QFrame::Raised);
        frame_9->setLineWidth(0);

        gridLayout_2->addWidget(frame_9, 6, 3, 1, 1);

        frame_7 = new QFrame(MainWidget);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        sizePolicy4.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy4);
        frame_7->setMinimumSize(QSize(0, 60));
        frame_7->setFrameShape(QFrame::NoFrame);
        frame_7->setFrameShadow(QFrame::Raised);
        frame_7->setLineWidth(0);

        gridLayout_2->addWidget(frame_7, 7, 1, 1, 3);

        frameDown = new QFrame(MainWidget);
        frameDown->setObjectName(QString::fromUtf8("frameDown"));
        frameDown->setMinimumSize(QSize(0, 80));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush2(QColor(111, 111, 111, 150));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        frameDown->setPalette(palette1);
        frameDown->setAutoFillBackground(true);
        frameDown->setFrameShape(QFrame::StyledPanel);
        frameDown->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frameDown);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonAuto = new MyPushButton(frameDown);
        pushButtonAuto->setObjectName(QString::fromUtf8("pushButtonAuto"));
        sizePolicy.setHeightForWidth(pushButtonAuto->sizePolicy().hasHeightForWidth());
        pushButtonAuto->setSizePolicy(sizePolicy);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/image/auto/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonAuto->setIcon(icon2);
        pushButtonAuto->setIconSize(QSize(50, 50));
        pushButtonAuto->setAutoRepeat(false);

        horizontalLayout->addWidget(pushButtonAuto);

        frame_2 = new QFrame(frameDown);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMouseTracking(true);
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        frame_2->setLineWidth(0);

        horizontalLayout->addWidget(frame_2);

        pushButtonPrevious = new MyPushButton(frameDown);
        pushButtonPrevious->setObjectName(QString::fromUtf8("pushButtonPrevious"));
        sizePolicy.setHeightForWidth(pushButtonPrevious->sizePolicy().hasHeightForWidth());
        pushButtonPrevious->setSizePolicy(sizePolicy);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/image/previous/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonPrevious->setIcon(icon3);
        pushButtonPrevious->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButtonPrevious);

        frame_3 = new QFrame(frameDown);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setMouseTracking(true);
        frame_3->setFrameShape(QFrame::NoFrame);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_3->setLineWidth(0);

        horizontalLayout->addWidget(frame_3);

        pushButtonBegin = new MyPushButton(frameDown);
        pushButtonBegin->setObjectName(QString::fromUtf8("pushButtonBegin"));
        sizePolicy.setHeightForWidth(pushButtonBegin->sizePolicy().hasHeightForWidth());
        pushButtonBegin->setSizePolicy(sizePolicy);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/image/start/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonBegin->setIcon(icon4);
        pushButtonBegin->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButtonBegin);

        frame_4 = new QFrame(frameDown);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMouseTracking(true);
        frame_4->setFrameShape(QFrame::NoFrame);
        frame_4->setFrameShadow(QFrame::Raised);
        frame_4->setLineWidth(0);

        horizontalLayout->addWidget(frame_4);

        pushButtonNext = new MyPushButton(frameDown);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));
        sizePolicy.setHeightForWidth(pushButtonNext->sizePolicy().hasHeightForWidth());
        pushButtonNext->setSizePolicy(sizePolicy);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/image/next/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonNext->setIcon(icon5);
        pushButtonNext->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButtonNext);

        frame_5 = new QFrame(frameDown);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setMouseTracking(true);
        frame_5->setFrameShape(QFrame::NoFrame);
        frame_5->setFrameShadow(QFrame::Raised);
        frame_5->setLineWidth(0);

        horizontalLayout->addWidget(frame_5);

        pushButtonManu = new MyPushButton(frameDown);
        pushButtonManu->setObjectName(QString::fromUtf8("pushButtonManu"));
        sizePolicy.setHeightForWidth(pushButtonManu->sizePolicy().hasHeightForWidth());
        pushButtonManu->setSizePolicy(sizePolicy);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/image/manu/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButtonManu->setIcon(icon6);
        pushButtonManu->setIconSize(QSize(50, 50));

        horizontalLayout->addWidget(pushButtonManu);


        gridLayout_2->addWidget(frameDown, 8, 0, 1, 5);


        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QApplication::translate("MainWidget", "MainWidget", 0, QApplication::UnicodeUTF8));
        pushButtonList->setText(QString());
        pushButtonSave->setText(QString());
        labelChannelFreq->setText(QString());
        labelLeft->setText(QApplication::translate("MainWidget", "87.5", 0, QApplication::UnicodeUTF8));
        labelRight->setText(QApplication::translate("MainWidget", "108", 0, QApplication::UnicodeUTF8));
        labelChannelName->setText(QString());
        pushButtonAuto->setText(QString());
        pushButtonPrevious->setText(QString());
        pushButtonBegin->setText(QString());
        pushButtonNext->setText(QString());
        pushButtonManu->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_H
