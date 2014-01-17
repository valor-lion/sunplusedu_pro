#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QDebug>

#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <QDebug>
#include "si4702.h"

#define FM_START_FREQ                   875
#define FM_END_FREQ                     1080

#define FM_START_VOLUME                 0
#define FM_END_VALUME                   15

#define FM_DEV_PATH     "/dev/fm_si4702"


enum MutState{MUTERR = 0, NOMUTE, MUTE};
enum AreaType{AREA_CHINA = 0, AREA_JAPAN_WIDE, AREA_JAPAN};

class Device
{

private:
    QString     devPath;
    int         devFd;

    int         manuSeekCurVal;

public:

public:
                Device();

public:
    bool        fmPowerUp();
    bool        fmPowerDown();
    int         fmSetVolume(unsigned int volPercent);
    MutState    fmSetMute(MutState arg);
    int         fmAutoSeek(unsigned int *pList, unsigned int list_len);
    AreaType    fmSetArea(AreaType area);
    int         fmPlayFrq(unsigned int list);
    int         fmManuSeekUp(void);
    int         fmManuSeekDown(void);


};

#endif // DEVICE_H
