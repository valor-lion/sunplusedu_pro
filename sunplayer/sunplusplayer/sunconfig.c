
/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				sunconfig.c  
  *Description:		
  *
  *Date and Edition:		2012-11-18  		v1.0
  *Author:				Valor Lion
  **************************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sunconfig.h"
#include "suntimer.h"

#define HEAD_SONG_PATH	"SONG_PATH"
#define HEAD_PLAY_TIME	"PLAY_TIME"
#define HEAD_PLAY_MODE	"PLAY_MODE"
#define HEAD_PLAY_ITVDAYS	"PLAY_ITVDAYS"
#define HEAD_PLAY_TOTALDAYS	"PLAY_TOTALDAYS"


#define SCRIPT_DIR		"./script/"
#define USER_CFG_FILE	"suncfg"
#define MSG_SHOW_FILE	"sunmsg"

#define STRTOK_PRM		" \t"
#define COMMENT_SYMBOL	'#'
#define HEAD_AND_CONTENT	" :"


#define CONFIG_DEBUG

#ifdef	CONFIG_DEBUG
#define 	Cfgprintf(fmt,arg...)		printf(fmt,##arg)
#else
#define 	Cfgprintf(fmt,arg...)		NULL
#endif




/* @ sun_config create or realse interface
  * @create a sun_config and realase a sun_config
  *
  * @added by			valor-lion
  */

sun_config_t* sun_config_create(void)
{
	sun_config_t *cfg;

	cfg = malloc(sizeof(sun_config_t));
	if(cfg == NULL){
		Cfgprintf("[err]: malloc sun config failed!\n");
		return NULL;
	}

	cfg->itv_days = 0;
	cfg->total_days = 0;
	cfg->media_path = NULL;
	cfg->ply_mode = AUTO_MOD;
	set_time_empty(&cfg->ply_time);
	
	return cfg;
}

void sun_config_put(sun_config_t *cfg)
{
	if(cfg == NULL)
		return;
	
	if(cfg->media_path)
		free(cfg->media_path);

	free(cfg);
}

static char* get_begin_of_content(char *buf)
{
	while((*buf == ' ' || *buf == '\t'))
		buf++;
	return buf;
}
static char *clear_wrap_of_content(char *buf)
{
	char* point;

	point = strchr(buf, '\n');
	if(point)
		*point = '\0';

	point = strchr(buf, '\r');
	if(point)
		*point = '\0';

	return buf;
}





/* @ analyse the config message
  * @ read the config script, get the message needed, then set the song path,
  *play time, itv day, play mode, total days
  *
  * @ added by		valor-lion
  */

static inline int __config_number_check(char *buf)
{
	char *p = buf;
	int flag_act = 1;

	while(*p){
		if(!(*p>='0'  && *p<='9')){
			flag_act = 0;
			break;
		}
		p++;
	}
	return flag_act;
}

static  int __song_path_analysis(char *buf, sun_config_t *cfg)
{
	int len = strlen(buf);

	cfg->media_path = malloc(len+2);
	if(cfg->media_path == NULL)
		return -1;

	memset(cfg->media_path, 0, len+1);
	memcpy(cfg->media_path, buf, len);

	if(cfg->media_path[len-1] != '/')
		cfg->media_path[len] = '/';
	
	return 0;
}

static  int __play_time_analysis(char *buf, sun_config_t *cfg)
{
	char *c_hour;
	char *c_min;
	char *c_sec;

	int flag_act = 0;

	c_hour = strtok(buf,"-");
	if(c_hour){
		if(__config_number_check(c_hour))
			cfg->ply_time.hour = atoi(c_hour);
		else
			flag_act = -1;
	}
		
	c_min = strtok(NULL,"-");
	if(c_min){
		if(__config_number_check(c_min))
			cfg->ply_time.minute = atoi(c_min);
		else
			flag_act = -1;
	}

	c_sec = strtok(NULL,"-");
	if(c_sec){
		if(__config_number_check(c_sec))
			cfg->ply_time.second = atoi(c_sec);
		else
			flag_act = -1;
	}
	
//	Cfgprintf("hour is %s, mint is %s, sec is %s\n",c_hour,c_min,c_sec);

	return flag_act;
}

static  int __itv_days_analysis(char *buf, sun_config_t *cfg)
{
	if(__config_number_check(buf)){
		cfg->itv_days = atoi(buf);
		return 0;
	}else
		return -1;
}

static  int __play_mode_analysis(char *buf, sun_config_t *cfg)
{
	if(!strcmp(buf, "auto"))
		cfg->ply_mode = AUTO_MOD;
	else if(!strcmp(buf, "manual"))
		cfg->ply_mode = MANUAL_MOD;
	else 
		return -1;

	return 0;
}

static  int __total_days_analysis(char *buf, sun_config_t *cfg)
{
	if(__config_number_check(buf)){
		cfg->total_days = atoi(buf);
		return 0;
	}else
		return -1;
}


static int line_content_analysis(char *buf, sun_config_t *cfg)
{
	char *head = buf;
	char *content;

	head = strtok(buf, HEAD_AND_CONTENT);
	content = strtok(NULL, HEAD_AND_CONTENT);

	if(content == NULL)
		return 0;
//	Cfgprintf("(head)--%s\n(content)--%s\n",head,content);

	if(!strcmp(head,HEAD_SONG_PATH)){
		Cfgprintf("[info]: load song_path....%s\n",content);
		if(__song_path_analysis(content, cfg) < 0)
			Cfgprintf("[warning]: <config script> song path has error!\n");
	}else if(!strcmp(head,HEAD_PLAY_TIME)){
		Cfgprintf("[info]: load play_time....%s\n",content);
		if(__play_time_analysis(content, cfg) < 0)
			Cfgprintf("[warning]: <config script> play time has error!\n");
	}else if(!strcmp(head,HEAD_PLAY_ITVDAYS)){
		Cfgprintf("[info]: load play_itvdays....%s\n",content);
		if(__itv_days_analysis(content, cfg) < 0)
			Cfgprintf("[warning]: <config script> itv day has error!\n");
	}else if(!strcmp(head,HEAD_PLAY_MODE)){
		Cfgprintf("[info]: load play_mode....%s\n",content);
		if(__play_mode_analysis(content, cfg) < 0)
			Cfgprintf("[warning]: <config script> play mode has error!\n");
	}else if(!strcmp(head,HEAD_PLAY_TOTALDAYS)){
		Cfgprintf("[info]: load play_totaldays....%s\n",content);
		if(__total_days_analysis(content, cfg) < 0)
			Cfgprintf("[warning]: <config script> total days has error!\n");
	}

	return 0;
	
}

int load_config_script(sun_config_t *cfg)
{
	int ret;
	char cfg_file[50] = {0};
	FILE *pf_cfg;
	char buf[1024];
	char *content_start;

	if(cfg == NULL)
		return -1;

	sprintf(cfg_file,"%s%s",SCRIPT_DIR,USER_CFG_FILE);
	pf_cfg = fopen(cfg_file,"r");
	if(pf_cfg==NULL){
		perror("config file open failed!\n");
		return -1;
	}
	
	while(!feof(pf_cfg)){
		memset(buf, 0, sizeof(buf));
		if(fgets(buf, sizeof(buf), pf_cfg) == NULL)
			break;
		
		//clear the wrap of a line
		clear_wrap_of_content(buf);
		
		//get the beginning of active date
		content_start = get_begin_of_content(buf);

		//if the line is empty
		if(strlen(content_start) == 0)
			continue;
		//if the line is comment
		if(content_start[0] == COMMENT_SYMBOL)
			continue;

//		Cfgprintf("-----%s+++\n",content_start);
		ret = line_content_analysis(content_start, cfg);
		if(ret < 0){
			//err deal
		}
		
	}
	
	fclose(pf_cfg);

	return 0;
}




