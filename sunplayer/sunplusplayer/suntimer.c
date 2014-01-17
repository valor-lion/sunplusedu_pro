
/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				suntimer.c  
  *Description:		
  *
  *Date and Edition:		2012-11-18  		v1.0
  *Author:				Valor Lion
  **************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "suntimer.h"


#define TIME_DEBUG

#ifdef	TIME_DEBUG
#define 	Tprintf(fmt,arg...)		printf(fmt,##arg)
#else
#define 	Tprintf(fmt,arg...)		NULL
#endif


/*get the current time and return the sun_time_t*/
sun_time_t sun_get_curtime(sun_time_t *ftm)
{
	time_t now;
	struct tm *tim;
	sun_time_t fm_time = {0};		//the time used by system
	
	// get the seconds since 1970
	if(time(&now) == (time_t)-1){
		Tprintf("[sunland time module]: get system time failed!\n");
		return fm_time;
	}
	tim = localtime(&now);

	fm_time.hour = tim->tm_hour;
	fm_time.minute = tim->tm_min;
	fm_time.second = tim->tm_sec;

	if(ftm != NULL)
		*ftm = fm_time;

	return fm_time;
}


/*get the current date, and return the sun_date_t*/
sun_date_t sun_get_curdate(sun_date_t *fdat)
{
	time_t now;
	struct tm *tim;
	sun_date_t fm_date = {0};		//the time used by system
	
	// get the seconds since 1970
	if(time(&now) == (time_t)-1){
		Tprintf("[sunland time module]: get system time failed!\n");
		return fm_date;
	}
	tim = localtime(&now);
	//set the fm_time value
	fm_date.year = tim->tm_year + 1900;
	fm_date.month = tim->tm_mon + 1;
	fm_date.day = tim->tm_mday;

	if(fdat != NULL)
		*fdat = fm_date;

	return fm_date;
}




/* @convert the time_t to sun_time or sun_date
  * @used by other functions inside
  * 
  * @added by		Valor-Lion
  */
static sun_time_t sun_mksuntime(time_t *t)
{
	struct tm *tim;
	sun_time_t fm_time = {0};		//the time used by system
	tim = gmtime(t);
//	printf("{the time zone is %s}\n",tim->tm_zone);			//debug message
		
	//set the fm_time value
	fm_time.hour = tim->tm_hour;
	fm_time.minute = tim->tm_min;
	fm_time.second = tim->tm_sec;

	return fm_time;
}

static sun_date_t sun_mksundate(time_t *t)
{
	struct tm *tim;
	sun_date_t fm_date = {0};

	tim = gmtime(t);
	//set the fm_time value
	fm_date.year = tim->tm_year + 1900;
	fm_date.month = tim->tm_mon + 1;
	fm_date.day = tim->tm_mday;

	return fm_date;
	
}



/* @convert sun_date or sun_time to time_t value
  * @the sun_date and sun_time is used by sun_fulltime, the player need sun_fulltime to check time
  * 
  * @added by		Valor-Lion
  */

/*make the sun date and time to time_t*/
static time_t sun_mktime(sun_date_t *fdt, sun_time_t *ftm)
{
	time_t t;
	struct tm tim = {0};

	if(fdt != NULL){
		tim.tm_year = fdt->year - 1900;
		tim.tm_mon = fdt->month - 1;
		tim.tm_mday = fdt->day;
	}
	if(ftm != NULL){
		tim.tm_hour = ftm->hour;
		tim.tm_min = ftm->minute;
		tim.tm_sec = ftm->second;
	}
	tim.tm_zone = "CST";
	t = mktime(&tim);
	return t;
}

static time_t sun_daterange_to_time_t(sun_date_t days)
{
	time_t t;
	struct tm tim = {0};

	tim.tm_year = 1970-1900;
	tim.tm_mon = 0;
	tim.tm_mday = days.day+1;
	tim.tm_zone = "CST";
	t = mktime(&tim);
	//printf("------%d----%d\n",days.day, t);
	return t;
}

/*make the sun date to time_t*/
static time_t sun_mktime_from_date(sun_date_t *fdt)
{
	time_t t;
	struct tm tim = {0};

	tim.tm_year = fdt->year - 1900;
	tim.tm_mon = fdt->month - 1;
	tim.tm_mday = fdt->day;
	tim.tm_zone = "CST";
	t = mktime(&tim);
	//printf("++++++%d\n",t);
	return t;
}
/*make the sun time to time_t*/
static time_t sun_mktime_from_time(sun_time_t *ftm)
{
	sun_date_t fm_date;
	time_t t;

	fm_date = sun_get_curdate(NULL);
	
	t = sun_mktime(&fm_date, ftm) - sun_mktime(&fm_date, NULL);
	return t;
}




/* @the sun time interface
  * @there are severial functions in used, we can compare sun_time and sun_date;
  *  we can also plus and minus sun_time or sun_date
  * @added by		Valor-Lion
  */
int  sun_time_compare(sun_time_t ftm_a, sun_time_t ftm_b)
{
	time_t a_tm, b_tm;
	int cmp;

	a_tm = sun_mktime_from_time(&ftm_a);
	b_tm = sun_mktime_from_time(&ftm_b);

	cmp = (int)difftime(a_tm, b_tm);			//compare the two time

#ifdef	CHECK_DATE_DEBUG 
	Tprintf("============================\n");
	Tprintf("a time\tb time\tdiff\n");
	Tprintf("%d\t%d\t%d\n",a_tm,b_tm,(int)cmp);
	Tprintf("============================\n");
#endif
	return cmp;
}

int sun_date_compare(sun_date_t fdt_a, sun_date_t fdt_b)
{
	time_t a_tm, b_tm;
	int cmp;

	a_tm = sun_mktime_from_date(&fdt_a);
	b_tm = sun_mktime_from_date(&fdt_b);

	cmp = (int)difftime(a_tm, b_tm);			//compare the tow date

#ifdef	CHECK_DATE_DEBUG 
	Tprintf("============================\n");
	Tprintf("a date\t\tb date\t\tdiff\n");
	Tprintf("%d\t%d\t%d\n",a_tm,b_tm,(int)cmp);
	Tprintf("============================\n");
#endif

	return cmp;
}

sun_time_t sun_time_plus(sun_time_t time_a, sun_time_t time_b)
{
	sun_time_t ftm;
	sun_date_t fdat;
	time_t t;

	t = sun_mktime_from_time(&time_a) + sun_mktime_from_time(&time_b);

	fdat = sun_mksundate(&t);
	if(fdat.day > 1){
		ftm.hour = 23;
		ftm.minute = 59;
		ftm.second = 59;
	}
	else
		ftm = sun_mksuntime(&t);
	return ftm;
}
sun_time_t sun_time_minus(sun_time_t time_a, sun_time_t time_b)
{
	sun_time_t ftm;
	time_t t;

	t = sun_time_compare(time_a, time_b);
	if(t < 0)
		set_time_empty(&ftm);
	else
		ftm = sun_mksuntime(&t);
	return ftm;
}

sun_date_t sun_date_plus(sun_date_t date_a, sun_date_t date_b)
{
	sun_date_t fdat;
	time_t t;

	t = sun_mktime_from_date(&date_a) + sun_daterange_to_time_t(date_b);
	//printf("the time_t is %d\n",t);
	fdat = sun_mksundate(&t);

	return fdat;
}

sun_date_t sun_date_minus(sun_date_t date_a, sun_date_t days)
{
	sun_date_t fdat;
	time_t t;

	t = sun_date_compare(date_a, days);
	
	fdat = sun_mksundate(&t);

	return fdat;
}



/* @player_fulltime dealing functions
  * @the configuration of struct sun_fulltime.
  * 	we can init the player_fulltime using "__fulltime_init";
  * 	we can check the player_fulltime is weather active compared with point time using "is_time_out";
  * 	get the current fulltime using "get_current_fulltime";
  * 	set current point time or next point time using "set_XXX_point_fulltime";
  * @ added by		Valor-Lion
  */


void player_fulltime_init(player_fulltime_t *pfultim)
{
	set_date_empty(&pfultim->player_date);
	set_time_empty(&pfultim->player_time);

	return;
}


int is_time_out(player_fulltime_t cur_fultim, player_fulltime_t pot_fultim)
{
	int tim_cmp;
	int flag = 0;
	
	tim_cmp = sun_date_compare(cur_fultim.player_date, pot_fultim.player_date);
	if(tim_cmp > 0){
		flag = 1;
	}else if(tim_cmp == 0){	
		tim_cmp = sun_time_compare(cur_fultim.player_time, pot_fultim.player_time);
		if(tim_cmp >= 0)
			flag = 1;
		else 
			flag = 0;
	}else{
		flag = 0;
	}
	
	return flag;
}

player_fulltime_t get_current_fulltime(void)
{
	player_fulltime_t cur_fultim = {
		.player_date = {0},
		.player_time = {0},

	};
	sun_date_t cur_date;
	sun_time_t cur_time;

	cur_date = sun_get_curdate(NULL);
	if(DATE_IS_EMPTY(cur_date))
		return cur_fultim;

	cur_time = sun_get_curtime(NULL);
	if(TIME_IS_EMPTY(cur_time))
		return cur_fultim;

	cur_fultim.player_date = cur_date;
	cur_fultim.player_time = cur_time;

	return cur_fultim;
}

void __check_target_fulltime(player_fulltime_t *p_fulltime, const unsigned int itv_day)
{
	player_fulltime_t cur_fultim;

	cur_fultim = get_current_fulltime();
	
	if(is_time_out(cur_fultim, *p_fulltime))
			update_target_fulltime(p_fulltime,itv_day);
}

int set_target_fulltime(player_fulltime_t *p_fulltime, sun_date_t dat, sun_time_t tim)
{
	sun_date_t cur_date;
	
	cur_date = sun_get_curdate(NULL);
	p_fulltime->player_date = cur_date;
	p_fulltime->player_time = tim;

	return 0;
}


static inline int __add_target_fulltime(player_fulltime_t *p_fulltime, const unsigned int days)
{
	sun_date_t step_date = {
		.year = 0,
		.month = 0,
		.day = days,
	};
	p_fulltime->player_date = sun_date_plus(p_fulltime->player_date, step_date);
	return 0;
}

int update_target_fulltime(player_fulltime_t *p_fulltime, const unsigned int itv_day)
{
	return __add_target_fulltime(p_fulltime, itv_day+1);
}

int set_dead_fulltime(player_fulltime_t *p_fulltime, const player_fulltime_t cur_fulltime, const unsigned int days)
{

	*p_fulltime = cur_fulltime;
	__add_target_fulltime(p_fulltime,days);
	
	Tprintf("......the dead fulltime......\n");
	date_printf(p_fulltime->player_date);
	time_printf(p_fulltime->player_time);

	return 0;
}


