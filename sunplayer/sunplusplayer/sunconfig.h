
#ifndef	__SUNCONFIG_H
#define	__SUNCONFIG_H

#include "suntimer.h"

typedef enum play_mode{
	MANUAL_MOD = 0,
	AUTO_MOD = 1,
}play_mode_t;


typedef struct sun_config{
	char *media_path;
	sun_time_t ply_time;
	play_mode_t ply_mode;
	unsigned long itv_days;
	unsigned long total_days;
}sun_config_t;


extern sun_config_t* sun_config_create(void);
extern void sun_config_put(sun_config_t *cfg);
extern int load_config_script(sun_config_t *cfg);


#endif



