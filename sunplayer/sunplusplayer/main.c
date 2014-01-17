
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "suntimer.h"
#include "sunplayer.h"
#include "sunsong.h"
#include "main.h"


#define 		DEFAULT_SONG	"./source/default.mp3"
#define		DEFAULT_DIR		"./source/"

sun_time_t my_time = {
	.hour = 12,
	.minute = 10,
	.second = 0,
};

int main(void)
{
	int ret;
	sun_config_t *usr_cfg;
	player_task_t *play_task;

	//create a new sun config
	usr_cfg = sun_config_create();
	//create a new player task
	play_task = player_task_create("valor-lion_player");

	load_config_script(usr_cfg);
	//do the initials
	set_player_task_songdir(play_task, usr_cfg->media_path);
	set_player_task_defsong(play_task,DEFAULT_SONG);
	set_player_task_itvdays(play_task, usr_cfg->itv_days);
	set_player_task_tartime(play_task, usr_cfg->ply_time);

	//start the player task
	player_task_run(play_task, usr_cfg->ply_mode, NULL, usr_cfg->total_days);

	//free the player task alloced
	player_task_put(play_task);	

	sun_config_put(usr_cfg);
	return 0;
}





