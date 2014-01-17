#include "device.h"

Device::Device()
{
    manuSeekCurVal = FM_START_FREQ;
    devPath = FM_DEV_PATH;

}
bool Device::fmPowerUp()
{
    this->devFd = open(FM_DEV_PATH,O_RDWR);
    if(this->devFd < 0){
        printf("file %s open failed!\n",FM_DEV_PATH);
        return false;
    }
    return true;
}
bool Device::fmPowerDown()
{
    close(this->devFd);
    qDebug()<<"SI4702 power up and initial done!\n";
    return true;
}

/*set the volume of si4702,volume rang:0-16,return the value of setting*/
int Device::fmSetVolume(unsigned int volPercent)
{
    int volume;

    volume = volPercent*(FM_END_VALUME-FM_START_VOLUME)/100;

    //设置si4702模块音量
    if(ioctl(devFd,SI4702_SET_VOLUME,volume))
        return -1;

    qDebug()<<"SI4702 volume is "<<volPercent;
    return volPercent;
}

MutState Device::fmSetMute(MutState arg)
{
    int val = 0;

    if(arg == MUTE)
        val = 1;
    if(ioctl(devFd,SI4702_SET_MUTE,val) < 0)
        return MUTERR;

    qDebug()<<"SI4702 set mute state "<<val;
    return arg;
}
int Device::fmAutoSeek(unsigned int *pList, unsigned int list_len)
{
    struct si4702_channel_list chan_list;
    int chan_num;
    int ret;
    //自动搜台，搜索完毕会保存到内核中定义的列表中，需要用户进行读取
    ret = ioctl(devFd,SI4702_AUTO_SEEK,&chan_num);
    if(ret < 0 || !chan_num)
        return -1;

    //读取内核中电台列表信息
    ret = read(devFd,&chan_list,sizeof(chan_list));
    if(ret != sizeof(chan_list))
        return -1;

    if(list_len > chan_list.index)
        list_len = chan_list.index;
    memcpy(pList, chan_list.list, list_len*sizeof(*pList));

    return list_len;
}
AreaType Device::fmSetArea(AreaType area)
{

    return area;
}

int Device::fmPlayFrq(unsigned int freq)
{
    int wrLen;

    wrLen = write(devFd,&freq,sizeof(freq));
    if(wrLen != sizeof(freq)){
        return -1;
    }
    return wrLen;
}


int Device::fmManuSeekUp()
{
    int retVal;

    retVal = ioctl(devFd, SI4702_SEEK_NEXT, &manuSeekCurVal);
    if(retVal)
        return -1;
    return manuSeekCurVal;
}

int Device::fmManuSeekDown()
{
    int retVal;

    retVal = ioctl(devFd, SI4702_SEEK_PREV, &manuSeekCurVal);
    if(retVal)
        return -1;
    return manuSeekCurVal;
}



