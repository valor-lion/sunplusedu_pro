/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				sunplayer.c  
  *Description:		
  *
  *Date and Edition:		2012-11-18  		v1.0
  *Author:				Valor Lion
  **************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "suntimer.h"
#include "sunplayer.h"
#include "sunsong.h"


#define PLAYER_DEBUG

#ifdef	PLAYER_DEBUG
#define 	Plyprintf(fmt,arg...)		printf(fmt,##arg)
#else
#define 	Plyprintf(fmt,arg...)		NULL
#endif


static int get_random_number(int range_num)
{
	 int val;

	if(range_num <= 0)
		return 0;
	 
	 srand((unsigned)time(NULL));
	 val = rand() % (range_num) + 1; 
	return val;
}


static int player_task_scansong(player_task_t *ply_tsk)
{
	if(ply_tsk == NULL)
		return -1;
	if(ply_tsk->media_dir == NULL){
		Plyprintf("[err]:<sunplayer> media_dir is empty!\n");
		return -1;
	}

	ply_tsk->md_name = sun_scan_media(ply_tsk->media_dir,&ply_tsk->name_num);

	return ply_tsk->name_num;
}

static void player_task_releasesong(player_task_t *ply_tsk)
{
	sun_release_media(ply_tsk->md_name);
	ply_tsk->md_name = NULL;
	ply_tsk->name_num = 0;
	
	return;
}



/* @ player task configure interface
  * @create and realse a player task, set the song_directory of task, set the path  of default 
  *song, set the target time to play at, set the itv_day of playing
  *
  * @added by			valor-lion
  */
player_task_t *player_task_create(char *name)
{
	player_task_t *one_task = NULL;
	int name_size;

	//malloc a new player task
	one_task = malloc(sizeof(player_task_t));
	if(one_task == NULL){
		Plyprintf("[err]:<sunplayer>  malloc task failed!\n");
		return NULL;
	}

	//config the task name
	name_size = strlen(name);
	one_task->task_name = malloc(name_size+1);
	if(one_task->task_name == NULL){
		Plyprintf("[err]:<sunplayer>  malloc task name failed!\n");
		return NULL;
	}
	memset(one_task->task_name, 0, name_size+1);
	//copy name to task
	memcpy(one_task->task_name,name,name_size);

	one_task->itv_day = 0;

	one_task->ply_mod = MANUAL_MOD;

	//init the configer fulltime and current fulltime
	player_fulltime_init(&one_task->tar_fultim);
	player_fulltime_init(&one_task->dead_fultim);

	//init the media name of task
	one_task->md_name = NULL;
	one_task->name_num = 0;

	//media_name_init(&one_task->act_name);
	one_task->act_name = NULL;

	one_task->media_dir = NULL;

	one_task->def_path = NULL;

	one_task->private_data = NULL;

	return one_task;
	
}

void player_task_put(player_task_t *ply_tsk)
{
	
	if(ply_tsk == NULL)
		return;

	//free the task name
	if(ply_tsk->task_name)
		free(ply_tsk->task_name);

	sun_release_media(ply_tsk->md_name);

	//free the active media name
	if(ply_tsk->act_name)
		media_name_put(ply_tsk->act_name);
	
	//free the media directory
	if(ply_tsk->media_dir)
		free(ply_tsk->media_dir);
	
	//free the media default path
	if(ply_tsk->def_path)
		free(ply_tsk->def_path);
	
	//free the player task
	free(ply_tsk);
	
	return;
}



int set_player_task_songdir(player_task_t *ply_tsk, char *dir)
{
	int dir_len;

	if(ply_tsk == NULL)
		return -1;

	//release the old media dir if it's set
	if(ply_tsk->media_dir)
		free(ply_tsk->media_dir);

	dir_len = strlen(dir);
	ply_tsk->media_dir = malloc(dir_len+1);
	if(ply_tsk->media_dir == NULL){
		Plyprintf("[err]:<sunplayer> malloc media dirpath failed!\n");
		return -1;
	}
	memset(ply_tsk->media_dir, 0, dir_len+1);
	memcpy(ply_tsk->media_dir, dir, dir_len);

	Plyprintf("[info]:<song directory setting> the song dir is %s\n",ply_tsk->media_dir);

	return 0;
	
}

int set_player_task_defsong(player_task_t *ply_tsk, char* def_path)
{
	unsigned int path_len;
	
	if(ply_tsk == NULL)
		return -1;

	if(ply_tsk->def_path)
		free(ply_tsk->def_path);

	//malloc space for def_path
	path_len = strlen(def_path);
	ply_tsk->def_path = malloc(path_len+1);
	if(ply_tsk->def_path == NULL){
		Plyprintf("[err]:<sunplayer>  set default path failed!\n");
		return -1;
	}

	memset(ply_tsk->def_path, 0, path_len+1);
	memcpy(ply_tsk->def_path, def_path, path_len);

	Plyprintf("[info]:<default song setting> the default song is %s\n",ply_tsk->def_path);
	
	return 0;
}

int set_player_task_tartime(player_task_t *ply_tsk, sun_time_t tim)
{
	sun_date_t dat;

	dat = sun_get_curdate(NULL);
	set_target_fulltime(&ply_tsk->tar_fultim, dat, tim);
	
	__check_target_fulltime(&ply_tsk->tar_fultim,ply_tsk->itv_day);

	return 0;
}

int set_player_task_itvdays(player_task_t *ply_tsk, const unsigned long itv_days)
{
	if(ply_tsk == NULL)
		return -1;
	ply_tsk->itv_day = itv_days;
	return itv_days;
}




/* @ player task run
  * @ start the sun player, we can set the total working time, if we set 0, then it will work
  *forever. we can also set a song to play and ignore the songs in directory, this function 
  *can be realize by setting the parameter of "song_name", usually we set "NULL"
  *
  * @added by			valor-lion
  */

static media_name_t *__get_player_task_songrdm(player_task_t * ply_tsk)
{
	int rd_num;
	media_name_t *md_name;

	//get a random value among the name_num 
	rd_num = get_random_number(ply_tsk->name_num);
	printf(".....%d.....%d\n",ply_tsk->name_num,rd_num);
	//get the song media
	md_name = sun_get_media(ply_tsk->md_name, rd_num);
	
	return md_name;
}

static inline int __dead_time_check(player_fulltime_t cur_fultim, player_fulltime_t dead_fultim)
{
	if(FULTIM_IS_EMPTY(dead_fultim))
		return 0;
	if(is_time_out(cur_fultim, dead_fultim))
		return 1;

	return 0;
}

int set_player_task_actsong(player_task_t *ply_tsk, char* name)
{
		
	if(ply_tsk == NULL)
		return -1;

	//release the old act_name
	if(ply_tsk->act_name)
		media_name_put(ply_tsk->act_name);

	//get a exist media name, from md_name list
	if(name == NULL){
		ply_tsk->act_name = __get_player_task_songrdm(ply_tsk);
		if(ply_tsk->act_name == NULL){
			Plyprintf("[warning]:<sunplayer> set active song failed!\n");
			return -1;
		}
		goto done;
	}

	//create a new media name
	ply_tsk->act_name = media_name_create(name, 0);
	if(ply_tsk->act_name == NULL){
		Plyprintf("[err]:<sunplayer> create media name failed!\n");
		return -1;
	}

done:
	return 0;
}

static int __player_task_run(player_task_t *ply_tsk, set_actsong_t *set_player_actsong, char *song_name)
{
	int ret;
	int scan_num;
	player_fulltime_t cur_fultim;

	/*do while run*/
	while(1){

		cur_fultim = get_current_fulltime();

		if(__dead_time_check(cur_fultim, ply_tsk->dead_fultim)){
			Plyprintf("[info]: player dead time arrived...\n");
			break;
		}
		
		if(is_time_out(cur_fultim, ply_tsk->tar_fultim)){
			//scan the song
			scan_num = player_task_scansong(ply_tsk);
			Plyprintf("[info]: the number of scansong is %d\n",scan_num);

			//set the player active song name and play the song
			ret = set_player_actsong(ply_tsk, song_name);
			if(ret < 0){	//if the name setting is fialed
				Plyprintf("[warning]: search failed,play the default song \n");
				sun_play_song(ply_tsk->def_path);
			}else{
				Plyprintf("[info]: the actsong name is <%s>\n",ply_tsk->act_name->song_name);
				sun_play_media(ply_tsk->media_dir,ply_tsk->act_name);
			}
			//release the song
			player_task_releasesong(ply_tsk);
			
			//task run mode check and deal
			if(ply_tsk->ply_mod == MANUAL_MOD)
				break;

			Plyprintf("[info]: update the next play time...\n");
			update_target_fulltime(&ply_tsk->tar_fultim, ply_tsk->itv_day);	
		
		}else{
			Plyprintf("[info]scanning.......\n");
			Plyprintf("\n......current time.....\n");
			date_printf(cur_fultim.player_date);
			time_printf(cur_fultim.player_time);
			
			Plyprintf("\n......target time.....\n");
			date_printf(ply_tsk->tar_fultim.player_date);
			time_printf(ply_tsk->tar_fultim.player_time);

		}
		sleep(5);
		
	}
	return 0;

}

int player_task_run(player_task_t *ply_tsk, play_mode_t mode, char *song_name, unsigned int days)
{
	player_fulltime_t cur_fultim;
	ply_tsk->ply_mod = mode;

	if(days == 0){
		player_fulltime_clear(&ply_tsk->dead_fultim);
	}else{
		cur_fultim = get_current_fulltime();
		set_dead_fulltime(&ply_tsk->dead_fultim, cur_fultim, days);
	}
	

	return __player_task_run(ply_tsk,set_player_task_actsong, song_name);
}



