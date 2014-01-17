
#ifndef 	__SUNSONG_H
#define 	__SUNSONG_H

#include "link.h"

typedef struct media_name{
	struct list_head list;
	int id;
	char *song_name;
	unsigned long name_len;
}media_name_t;

typedef struct media_name  TYPE;


extern media_name_t *media_name_create(char *name,int id);
extern void media_name_init(media_name_t *pmd_name);
extern void media_name_put(media_name_t *pmd_name);


extern struct list_head *sun_scan_media(char *dir_path, unsigned long *count);
extern media_name_t* sun_get_media(struct list_head *head, int id);
extern void sun_release_media(struct list_head *head);
extern int sun_play_song(char *song_path);
extern int sun_play_media(char *song_dir, media_name_t *song_media);

#endif

