#include "stationparam.h"


StationParam::StationParam(unsigned int size)
{
    this->bufSize = size;
    this->freqBuf = new unsigned int[this->bufSize];
    this->nameBuf = new QString[this->bufSize];
    this->bufAct = 0;
    this->bufIndex = 0;
    this->fileName = "./savefile/fm_list";

    this->loadFile();
}

StationParam::~StationParam()
{
    delete this->freqBuf;
    delete this->nameBuf;
}

void StationParam::clearStation()
{
    for(unsigned int i=0; i<this->bufAct; i++){
        freqBuf[i] = 0;
        nameBuf[i] = "";
    }
    this->bufAct = 0;
    this->bufIndex = 0;
}
void StationParam::rewindStation()
{
    this->bufIndex = 0;
}

void StationParam::addOneStation(unsigned int freq, QString name)
{
    if(this->bufAct >= this->bufSize)
        return;

    int chNum = this->bufAct + 1;
    if(name == "")
        name = "channel list " + QString::number(chNum);

    nameBuf[this->bufAct] = name;
    freqBuf[this->bufAct] = freq;
    this->bufAct++;

    this->saveFile();
}

int StationParam::addStations(unsigned int *freq, unsigned int size)
{
    QString name;
    unsigned int num;
    if(size + this->bufAct > this->bufSize)
        size = this->bufSize - this->bufAct;
    for(unsigned int i=0; i<size; i++){
        num = this->bufAct + i + 1;
        name = "station list" + QString::number(num);
        nameBuf[this->bufAct+i] = name;
        freqBuf[this->bufAct+i] = freq[i];
    }
    this->bufAct += size;

    this->saveFile();

    return size;
}

unsigned int StationParam::channelShift(int val)
{
    int tmpIndex = this->bufIndex;
    tmpIndex += val;

    if(tmpIndex >= 0 && tmpIndex < this->bufAct)
        this->bufIndex = tmpIndex;

    qDebug()<<"++++++++++++"<<this->bufIndex;
    return this->freqBuf[this->bufIndex];
}
unsigned int StationParam::channelOrient(unsigned int num)
{
    if(num > 0 && num <= this->bufAct)
        this->bufIndex = num-1;

    qDebug()<<"++++++++++++"<<this->bufIndex;
    return this->freqBuf[this->bufIndex];
}

unsigned int StationParam::getCurChannelFreq()
{
    return freqBuf[this->bufIndex];
}

unsigned int StationParam::getAppointChannelFreq(unsigned int num)
{
    int index = num-1;
    return freqBuf[index];
}

QString StationParam::getCurChannelName()
{
    return nameBuf[this->bufIndex];
}

QString StationParam::getAppointChannelName(unsigned int num)
{
    int index = num-1;
    return nameBuf[index];
}

int StationParam::getChannelNum()
{
    return this->bufAct;
}

void StationParam::saveFile()
{
    QFile file(this->fileName);

    if(file.open(QIODevice::WriteOnly)){
        QTextStream writeData(&file);
        for(int i=0; i<this->bufAct; i++)
            writeData<<i<<"$$"<<this->nameBuf[i]<<"$$"<<this->freqBuf[i]<<"\n";
    }
    file.close();
}

void StationParam::loadFile()
{
    QFile file(this->fileName);
    QString dataLine;
    int index;
    if(file.open(QIODevice::ReadOnly)){
        QTextStream readData(&file);
        while(readData.atEnd() == false){
            dataLine = readData.readLine();
            if(dataLine.size() != 1){
                index = dataLine.section("$$",0,0).toInt();
                if(index >= this->bufSize)
                    return;
                this->nameBuf[index] = dataLine.section("$$",1,1);
                this->freqBuf[index] = dataLine.section("$$",2,2).toInt();
            }
        }
        this->bufAct = index + 1;
    }
    file.close();
}

