
#ifndef 	__SUNTIMER_H
#define 	__SUNTIMER_H

/*the sun date defination*/
typedef struct sun_date{
	unsigned int year;
	unsigned int month;
	unsigned int day;
}sun_date_t;

/*the sun time defination*/
typedef struct sun_time{
	unsigned int hour;		
	unsigned int minute;
	unsigned int second;
}sun_time_t;

typedef struct sun_time_len{
	unsigned long sec;
	unsigned long usec;
}sun_time_len_t;



typedef struct player_fulltime{
	sun_date_t  player_date;
	sun_time_t  player_time;
}player_fulltime_t;


#define DATE_IS_EMPTY(_date)		((_date).year == 0 && (_date).month == 0 && (_date).day == 0)
#define TIME_IS_EMPTY(_time)		((_time).hour == 0 && (_time).minute == 0 && (_time).second == 0)

#define FULTIM_IS_EMPTY(_fultim)		(DATE_IS_EMPTY((_fultim).player_date) && TIME_IS_EMPTY((_fultim).player_time))

#define set_time_empty(_time)		do{(_time)->hour = 0; (_time)->minute = 0; (_time)->second = 0;}while(0)
#define set_date_empty(_date)		do{(_date)->year = 0; (_date)->month = 0; (_date)->day = 0;}while(0)
#define set_time_full(_time)		do{(_time)->hour = 23; (_time)->minute = 59; (_time)->second = 59;}while(0)

#define time_printf(_ftime)	printf("(Time): %d : %d : %d\n",(_ftime).hour, (_ftime).minute, (_ftime).second)
#define date_printf(_fdate)	printf("(Date): %d - %d - %d\n",(_fdate).year, (_fdate).month, (_fdate).day)


extern sun_time_t sun_get_curtime(sun_time_t *ftm);
extern sun_date_t sun_get_curdate(sun_date_t *fdat);


extern void player_fulltime_init(player_fulltime_t *pfultim);

#define player_fulltime_clear(_pfultim)	\
	player_fulltime_init(_pfultim)

extern int is_time_out(player_fulltime_t cur_fultim, player_fulltime_t pot_fultim);
extern player_fulltime_t get_current_fulltime(void);
extern int set_target_fulltime(player_fulltime_t *p_fulltime, sun_date_t dat, sun_time_t tim);
extern int set_dead_fulltime(player_fulltime_t *p_fulltime, const player_fulltime_t cur_fulltime, const unsigned int days);
extern int update_target_fulltime(player_fulltime_t *p_fulltime, const unsigned int itv_day);
extern void __check_target_fulltime(player_fulltime_t *p_fulltime, const unsigned int itv_day);


#endif

