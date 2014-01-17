#ifndef STATIONPARAM_H
#define STATIONPARAM_H

#include <QString>
#include <QDebug>
#include <QFile>

class StationParam
{

private:
    unsigned int *freqBuf;
    QString *nameBuf;
    unsigned int bufSize;
    unsigned int bufAct;
    unsigned int bufIndex;

    QString fileName;
 //   unsigned int curFrequency;

public:
    StationParam(unsigned int size = 100);
    ~StationParam();

public:
    void clearStation();
    void rewindStation();
    void addOneStation(unsigned int freq, QString name="");
    int addStations(unsigned int *freq, unsigned int size);
    unsigned int channelShift(int val);
    unsigned int channelOrient(unsigned int num);
    unsigned int getCurChannelFreq();
    unsigned int getAppointChannelFreq(unsigned int num);
    int getChannelNum();

    QString getCurChannelName();
    QString getAppointChannelName(unsigned int num);

    void saveFile();
    void loadFile();

};

#endif // STATIONPARAM_H
