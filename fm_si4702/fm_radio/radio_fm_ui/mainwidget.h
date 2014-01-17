#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QString>
#include <QPoint>
#include <QDebug>
#include <QRect>
#include <QFont>
#include <QPen>
#include <QBrush>
#include "device.h"
#include "showwidget.h"
#include "listwidget.h"
#include "stationparam.h"

#define VOLUME_SLIDE_RECT_X         740
#define VOLUME_SLIDE_RECT_Y         100
#define VOLUME_SLIDE_WIDTH          60
#define VOLUME_SLIDE_HEIGHT         240

#define VOLUME_SHOW_RECT_X         30
#define VOLUME_SHOW_RECT_Y         110
#define VOLUME_SHOW_WIDTH          20
#define VOLUME_SHOW_HEIGHT         210

#define CHANNEL_SLIDE_RECT_X        0
#define CHANNEL_SLIDE_RECT_Y        0
#define CHANNEL_SLIDE_WIDTH         0
#define CHANNEL_SLIDE_HEIGHT        0

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

    enum FmRadioState{FMSTOP, FMPLAYING, FMAUTOSCANING, FMMANUSCANING};

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void initInfo();                    // initialize the information 
    void needUpdateVolume();  // it needs show the volume
    void setVolume(int percent);         // set volume
    int  channelListSync();
    void updateChannelDisplay(const QString name, const unsigned int freq);
    void updateButtons();

protected:
    void paintEvent(QPaintEvent *);     // paint event
    void mousePressEvent(QMouseEvent *);    // mouse press event
    void mouseReleaseEvent(QMouseEvent *);  // mouse release event
    void mouseMoveEvent(QMouseEvent *);     // mouse move event
    bool isInRectArea(const QRect&, const QPoint&);    // it is in the rect area or not

private:
    Ui::MainWidget *ui;
    QString mainBackgroundPath;   // save background image path
    ShowWidget *pyWin;      // playing window
    ListWidget *listWin;    // list window
    Device *rdoDev;            //fm radio device
    StationParam *staPrm;

    FmRadioState fmState;

    QPoint pressPoint;      // the pressed start point

    unsigned int curChannelFreq;
    QString curChannelName;

    bool isVolumeShow;  // it is need update volume or not
    int volumePercent;        // the volum value
    int volumeRangeStep;


private slots:
    void on_pushButtonAuto_clicked();
    void on_pushButtonList_released();      // show channel list
    void on_pushButtonSave_released();      // save channel
    void on_pushButtonManu_released();      // manu seeking channel
    void on_pushButtonAuto_released();      // auto seeking channel
    void on_pushButtonPrevious_released();  // previou channel
    void on_pushButtonNext_released();      // next channel
    void on_pushButtonBegin_released();     // begin the channel
    void dealChannelNumFromList(int);       // deal the num form list window
};

#endif // MAINWIDGET_H
