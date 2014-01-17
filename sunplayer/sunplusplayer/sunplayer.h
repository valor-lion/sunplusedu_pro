

#ifndef 	__SUNPLAYER_H
#define 	__SUNPLAYER_H

#include "suntimer.h"
#include "sunsong.h"
#include "sunconfig.h"



typedef struct player_task{

	char *task_name;

	sun_config_t usr_cfg;

	play_mode_t	ply_mod;
	player_fulltime_t  tar_fultim;
	player_fulltime_t  dead_fultim;

	unsigned long 	itv_day;

//	media_name_t *md_name;		//reserved	
	struct list_head *md_name;
	unsigned long name_num;

	media_name_t *act_name;

	char *media_dir;

	char *def_path;

	void *private_data;
}player_task_t;

typedef int set_actsong_t(player_task_t *, char*);


extern player_task_t *player_task_create(char *name);
extern void player_task_put(player_task_t *ply_tsk);


extern int player_task_run(player_task_t *ply_tsk, play_mode_t mode, char *song_name, unsigned int days);
extern int set_player_task_tartime(player_task_t *ply_tsk, sun_time_t tim);
extern int set_player_task_itvdays(player_task_t *ply_tsk, const unsigned long itv_days);
extern int set_player_task_defsong(player_task_t *ply_tsk, char* def_path);
extern int set_player_task_songdir(player_task_t *ply_tsk, char *dir);


#endif


