#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    this->pyWin = new ShowWidget;
    ui->horizontalLayoutTip->addWidget(this->pyWin);
    this->pyWin->hide();

    this->listWin = new ListWidget(100);
    this->rdoDev = new Device;
    this->staPrm = new StationParam(100);

    connect(this->listWin, SIGNAL(gobackSignal()), this, SLOT(show()));
    connect(this->listWin, SIGNAL(channelNumSignal(int)),
            this, SLOT(dealChannelNumFromList(int)));

    this->fmState = FMSTOP;
    // initialize the information
    this->initInfo();

}

MainWidget::~MainWidget()
{
    delete ui;
    delete this->pyWin;
}

/*
 * initialize the information
*/
void MainWidget::initInfo()
{
    this->resize(800, 480);   // set the main window's size
    this->showFullScreen(); // show full screen, for a8
    //this->setCursor(Qt::BlankCursor); // hide the cursor, for a8

    this->mainBackgroundPath = ":/new/prefix1/image/mainBackground/3.bmp";

    // set the pushButton image
    ui->pushButtonAuto->setImage(":/new/prefix1/image/auto/1.png", ":/new/prefix1/image/auto/u1.png");
    ui->pushButtonPrevious->setImage(":/new/prefix1/image/previous/1.png", ":/new/prefix1/image/previous/u1.png");
    ui->pushButtonNext->setImage(":/new/prefix1/image/next/1.png", ":/new/prefix1/image/next/u1.png");
    ui->pushButtonManu->setImage(":/new/prefix1/image/manu/1.png", ":/new/prefix1/image/manu/u1.png");
    ui->pushButtonList->setImage(":/new/prefix1/image/list/1.png", ":/new/prefix1/image/list/u1.png");
    ui->pushButtonSave->setImage(":/new/prefix1/image/save/1.png", ":/new/prefix1/image/save/u1.png");
    ui->pushButtonBegin->setImage(":/new/prefix1/image/start/1.png", ":/new/prefix1/image/start/u1.png");


    this->curChannelFreq = 0;
    this->curChannelName = "";

    // default setting
    this->pressPoint.setX(0);
    this->pressPoint.setY(0);

    this->volumePercent = 100;
    this->isVolumeShow = false;
    this->volumeRangeStep = 10;

    this->updateButtons();
    this->updateChannelDisplay(this->curChannelName, this->curChannelFreq);

    this->staPrm->loadFile();
    this->channelListSync();
}

void MainWidget::updateButtons()
{
    bool buttonBeginEns = false,buttonNextEns = false,
        buttonPrevEns = false,buttonAutoEns = false,
        buttonManuEns = false,buttonListEns = false,
        buttonSaveEns = false;

    switch(this->fmState){
    case FMSTOP:
        buttonBeginEns = true;
        break;
    case FMPLAYING:
        buttonBeginEns =true;
        buttonNextEns = true;
        buttonPrevEns = true;
        buttonAutoEns = true;
        buttonManuEns = true;
        buttonListEns = true;
        break;
    case FMAUTOSCANING:
        buttonAutoEns = true;
        break;
    case FMMANUSCANING:
        buttonBeginEns = true;
        buttonManuEns = true;
        buttonSaveEns = true;
        buttonPrevEns = true;
        buttonNextEns = true;
        break;
    }

    ui->pushButtonBegin->setButtonEnable(buttonBeginEns);
    ui->pushButtonNext->setButtonEnable(buttonNextEns);
    ui->pushButtonPrevious->setButtonEnable(buttonPrevEns);
    ui->pushButtonAuto->setButtonEnable(buttonAutoEns);
    ui->pushButtonManu->setButtonEnable(buttonManuEns);
    ui->pushButtonList->setButtonEnable(buttonListEns);
    ui->pushButtonSave->setButtonEnable(buttonSaveEns);
}

/*
 * paint event
*/
void MainWidget::paintEvent(QPaintEvent *)
{
    QPainter paint(this);   // create a painter

    //paint a background
    paint.drawPixmap(0, 0, width(), height(), QPixmap(mainBackgroundPath));

    // if it needs draw volume
    if(true == this->isVolumeShow){
        QPen pen;
        pen.setColor(Qt::gray);
        pen.setWidth(2);
        paint.setPen(pen);
        paint.drawRect(VOLUME_SHOW_RECT_X, VOLUME_SHOW_RECT_Y, VOLUME_SHOW_WIDTH, VOLUME_SHOW_HEIGHT);

        QBrush brush;
        brush.setColor(Qt::gray);
        brush.setStyle(Qt::SolidPattern);
        paint.setBrush(brush);
        paint.drawRect(VOLUME_SHOW_RECT_X, VOLUME_SHOW_RECT_Y+(100-this->volumePercent)*VOLUME_SHOW_HEIGHT/100,
                       VOLUME_SHOW_WIDTH, this->volumePercent*VOLUME_SHOW_HEIGHT/100);

        paint.drawPixmap(70, 170, 70, 70,
                         QPixmap(":/new/prefix1/image/volume/1.png"));
        paint.drawText(150, 220, QString::number(this->volumePercent));
    }
}

/*
 * it is in the rect area or not
*/
bool MainWidget::isInRectArea(const QRect &rect, const QPoint &point)
{
    if (point.x() > rect.x() && point.x() < rect.x()+rect.width()
     && point.y() > rect.y() && point.y() < rect.y()+rect.height())
        return true;
    else
        return false;
}

/*
 * mouse press event
*/
void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(this->fmState == FMPLAYING){
        // save the pressed point
        this->pressPoint.setX(event->x());
        this->pressPoint.setY(event->y());
    }
}

/*
 * mouse move event
*/
void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->fmState == FMPLAYING){
        // slide screen, change volume
        QRect volumeSlideRect;
        volumeSlideRect.setRect(VOLUME_SLIDE_RECT_X, VOLUME_SLIDE_RECT_Y,
                                VOLUME_SLIDE_WIDTH, VOLUME_SLIDE_HEIGHT);
        QPoint curPoint;
        curPoint.setX(event->x());
        curPoint.setY(event->y());
        if (true == isInRectArea(volumeSlideRect, curPoint))
        {
            this->isVolumeShow = true;

            if(event->y()-pressPoint.y() > this->volumeRangeStep){   // down orientation
                this->volumePercent -= 10;
                if(this->volumePercent < 0)
                    this->volumePercent = 0;
                //set the volume of radio
                this->setVolume(this->volumePercent);
                //update the press point
                this->pressPoint.setX(event->x());
                this->pressPoint.setY(event->y());
            }else if (pressPoint.y()-event->y() > this->volumeRangeStep){   // up orientation
                this->volumePercent += 10;
                if(this->volumePercent > 100)
                    this->volumePercent = 100;
                //set the volume of radio
                this->setVolume(this->volumePercent);
                //update the press point
                this->pressPoint.setX(event->x());
                this->pressPoint.setY(event->y());
            }
            this->update();
        }
    }
}

/*
 * mouse release event
*/
void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->fmState == FMPLAYING){
        this->isVolumeShow = false;
        this->update();
    }
}


int MainWidget::channelListSync()
{
    int staNum = staPrm->getChannelNum();

    listWin->clearChannelInfo();
    qDebug()<<"current channel list number is "<<staNum;
    for(int i=1; i<=staNum; i++){
        listWin->addOneChannelInfo(staPrm->getAppointChannelName(i),staPrm->getAppointChannelFreq(i));
        qDebug()<<"===="<<staPrm->getAppointChannelName(i)<<"  "<<staPrm->getAppointChannelFreq(i);
    }
    return staNum;
}

void MainWidget::updateChannelDisplay(const QString name, const unsigned int freq)
{
    unsigned int actFreq = freq;

    if(freq>FM_END_FREQ || freq<FM_START_FREQ)
        actFreq = FM_START_FREQ;

    QString lcdNum;
    lcdNum = QString::number(actFreq/10) + "." + QString::number(actFreq%10);
    ui->labelChannelFreq->setText(lcdNum);

    int slidNum = (actFreq-FM_START_FREQ)*(ui->horizontalSlider->maximum()-ui->horizontalSlider->minimum())/(FM_END_FREQ - FM_START_FREQ);
    ui->horizontalSlider->setValue(slidNum);

    ui->labelChannelName->setText(this->curChannelName);
    this->update();
}


void MainWidget::on_pushButtonBegin_released()
{
    bool ret;

    if (this->fmState == FMSTOP){
        ret = rdoDev->fmPowerUp();    //open the fm radio
        if(ret == false)
            return;

        staPrm->rewindStation();        //rewind the channel to start
        this->curChannelFreq = staPrm->getCurChannelFreq();
        this->curChannelName = staPrm->getCurChannelName();

        this->setVolume(this->volumePercent);
        rdoDev->fmPlayFrq(this->curChannelFreq);

        this->fmState = FMPLAYING;
        this->update();
        this->updateButtons();
        this->pyWin->show();
        ui->pushButtonBegin->setImage(":/new/prefix1/image/stop/1.png");
    }else if(this->fmState == FMPLAYING){
        rdoDev->fmPowerDown();
        this->curChannelFreq = 0;
        this->curChannelName = "";
        this->fmState = FMSTOP;

        this->update();
        this->updateButtons();
        this->pyWin->hide();
        ui->pushButtonBegin->setImage(":/new/prefix1/image/start/1.png");
    }
    this->updateChannelDisplay(this->curChannelName, this->curChannelFreq);
}

void MainWidget::on_pushButtonPrevious_released()
{

    if(this->fmState == FMPLAYING){
        staPrm->channelShift(-1);
        this->curChannelFreq = staPrm->getCurChannelFreq();
        rdoDev->fmPlayFrq(this->curChannelFreq);
        this->curChannelName = staPrm->getCurChannelName();
    }else if(this->fmState == FMMANUSCANING){
        this->curChannelFreq = rdoDev->fmManuSeekDown();
        this->curChannelName = "";
    }
    this->updateChannelDisplay(this->curChannelName,this->curChannelFreq);
}

void MainWidget::on_pushButtonNext_released()
{
    if(this->fmState == FMPLAYING){
        staPrm->channelShift(1);
        this->curChannelFreq = staPrm->getCurChannelFreq();
        rdoDev->fmPlayFrq(this->curChannelFreq);
        this->curChannelName = staPrm->getCurChannelName();
    }else if(this->fmState == FMMANUSCANING){
        this->curChannelFreq = rdoDev->fmManuSeekUp();
        this->curChannelName = "";
    }
    this->updateChannelDisplay(this->curChannelName,this->curChannelFreq);
}

void MainWidget::on_pushButtonAuto_released()
{
    unsigned int staBuf[100];
    int staNum;
    if(this->fmState == FMPLAYING){
 //      this->fmState = FMAUTOSCANING;
 //       this->updateButtons();

        staNum = rdoDev->fmAutoSeek(staBuf, sizeof(staBuf)/sizeof(staBuf[0]));
        if(staNum < 0){
            qDebug()<<"[QT Debug]: auto seek failed!! ";
            return;
        }
        qDebug()<<"[QT Debug]: auto seek channel number is "<<staNum;
        staPrm->clearStation();
        staPrm->addStations(staBuf, staNum);        //add channels to station parameters

        this->channelListSync();

        this->fmState = FMPLAYING;
        this->updateButtons();
        //rewind the station and play the first station
        staPrm->rewindStation();
        this->curChannelFreq = staPrm->getCurChannelFreq();
        this->curChannelName = staPrm->getCurChannelName();
        rdoDev->fmPlayFrq(this->curChannelFreq);

        this->updateChannelDisplay(this->curChannelName,this->curChannelFreq);
    }
}

void MainWidget::on_pushButtonManu_released()
{

    if(this->fmState == FMPLAYING){
        this->curChannelFreq = 0;
        this->curChannelName = "";
        this->fmState = FMMANUSCANING;
        this->updateButtons();
    }
    else if(this->fmState == FMMANUSCANING){
        this->curChannelFreq = staPrm->getCurChannelFreq();
        this->curChannelName = staPrm->getCurChannelName();
        rdoDev->fmPlayFrq(this->curChannelFreq);
        this->fmState = FMPLAYING;
        this->updateButtons();
    }
    this->updateChannelDisplay(this->curChannelName, this->curChannelFreq);
}

void MainWidget::on_pushButtonSave_released()
{
    if(this->fmState == FMMANUSCANING){
        staPrm->addOneStation(this->curChannelFreq);
        this->channelListSync();
    }
}

void MainWidget::on_pushButtonList_released()
{
    this->hide();
    this->listWin->show();
    this->listWin->updateList();
    this->listWin->updateButton();
}

/*
 * deal the num form list window
*/
void MainWidget::dealChannelNumFromList(int num)
{
    staPrm->channelOrient(num);
    this->curChannelFreq = staPrm->getCurChannelFreq();
    this->curChannelName = staPrm->getCurChannelName();
    rdoDev->fmPlayFrq(this->curChannelFreq);

    this->updateChannelDisplay(this->curChannelName,this->curChannelFreq);

    this->show();
    this->updateButtons();
}

/*
 * set volume
 * volumeValue is a member, it signs volume value
*/
void MainWidget::setVolume(int percent)
{
    rdoDev->fmSetVolume(this->volumePercent);
}


