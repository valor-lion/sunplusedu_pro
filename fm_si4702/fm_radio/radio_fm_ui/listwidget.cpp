#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(unsigned int size, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);

    this->pageSize = 5;
    this->pageNum = size/this->pageSize;
    this->pageIndex = 0;

    this->infoSize = this->pageNum * this->pageSize;
    this->channelInfo = new QString[this->infoSize];
    this->infoAct = 0;

    connect(ui->list1, SIGNAL(released()), this, SLOT(dealPressList()));
    connect(ui->list2, SIGNAL(released()), this, SLOT(dealPressList()));
    connect(ui->list3, SIGNAL(released()), this, SLOT(dealPressList()));
    connect(ui->list4, SIGNAL(released()), this, SLOT(dealPressList()));
    connect(ui->list5, SIGNAL(released()), this, SLOT(dealPressList()));

    this->initInfo();
}

ListWidget::~ListWidget()
{
    delete ui;
    delete this->channelInfo;
}

void ListWidget::initInfo()
{
   this->showFullScreen();

   ui->pushButtonUp->setImage(":/new/prefix1/image/up/1.png", ":/new/prefix1/image/up/u1.png");
   ui->pushButtonDown->setImage(":/new/prefix1/image/down/1.png", ":/new/prefix1/image/down/u1.png");
   ui->pushButtonGoback->setImage(":/new/prefix1/image/goback/1.png", ":/new/prefix1/image/goback/u1.png");

   this->updateList();
   this->updateButton();
}

void ListWidget::on_pushButtonUp_released()
{
    this->pageIndex --;
    //menus check
    if(this->pageIndex < 0)
        this->pageIndex == 0;

    this->updateList();
    this->updateButton();
}


void ListWidget::on_pushButtonGoback_released()
{
    this->hide();
    emit this->gobackSignal();
}

void ListWidget::on_pushButtonDown_released()
{
    //deal the page increase
    if((this->pageIndex+1)*this->pageSize < this->infoAct)
        this->pageIndex++;

    //check the overflow
    if(this->pageIndex >= this->pageNum)
        this->pageIndex = this->pageNum - 1;

    this->updateList();
    this->updateButton();
}

void ListWidget::updateList()
{
    QString str[5] = {"", "", "", "", ""};
    QString info;

    int k = this->pageIndex;
    qDebug()<<"the page index is "<<k;

    for(int i=0; i<5; i++){
        info = channelInfo[i+k*5];
        if(info != "")
            str[i] = QString("%1. %2").arg(i+k*5+1).arg(info);
        else
            str[i] = "";
    }

    ui->list1->setText(str[0]);
    ui->list2->setText(str[1]);
    ui->list3->setText(str[2]);
    ui->list4->setText(str[3]);
    ui->list5->setText(str[4]);
}

void ListWidget::dealPressList()
{
    QLabel *label = (QLabel *)this->sender();

    if(NULL == label)
        return;

    if (0 != label->text().size())
    {
        emit this->channelNumSignal(QString(label->text().at(0)).toInt());
        this->hide();
    }
}

void ListWidget::updateButton()
{

    ui->pushButtonUp->setButtonEnable(true);
    ui->pushButtonDown->setButtonEnable(true);

    //up or down button enable check
    if(this->pageIndex == 0)
        ui->pushButtonUp->setButtonEnable(false);

    //up or down button enable check
    if((this->pageIndex+1) * this->pageSize >= this->infoAct)
        ui->pushButtonDown->setButtonEnable(false);

}

void ListWidget::clearChannelInfo()
{
    for(int i=0; i<this->infoAct; i++)
        this->channelInfo[i] = "";
    this->infoAct = 0;
    this->pageIndex = 0;
}


void ListWidget::addOneChannelInfo(const QString str, const unsigned int freq)
{
    QString showName;
    float chFreq = freq/(float)10;
    if(this->infoAct >= this->infoSize)
        return;
    showName = str + " -- " + QString::number(chFreq);
    this->channelInfo[this->infoAct] = showName;

    this->infoAct++;
}

int ListWidget::addMultChannelInfo(QString *str, unsigned int *freq, const int num)
{
    int i;
    QString showName;
    int addSize;

    if(this->infoAct + num > this->infoSize)
        addSize = this->infoSize - this->infoAct;

    for (i = 0; i < addSize; i++){
        showName = str[i] + " -- " + QString::number(freq[i]);
        this->channelInfo[infoAct+i] = showName;
    }
    this->infoAct += i;

    return addSize;
}



