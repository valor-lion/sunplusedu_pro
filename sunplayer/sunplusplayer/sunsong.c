/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				sunsong.c  
  *Description:		
  *
  *Date and Edition:		2012-11-18  		v1.0
  *Author:				Valor Lion
  **************************************************************/
  
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "sunsong.h"
#include "link.h"


#define SONG_DEBUG

#ifdef	SONG_DEBUG
#define 	Sgprintf(fmt,arg...)		printf(fmt,##arg)
#else
#define 	Sgprintf(fmt,arg...)		NULL
#endif


static char *buf_music_ext[]={
	[0] = ".mp3",
	[1] = ".wma",
	[2] = ".ape",
};

static TYPE *container_of(NODE *pb)
{
	TYPE * ret;
	NODE *mptr = pb;
	ret = (TYPE *)((char *)mptr - offsetof(TYPE, list));
	
	return ret;
}

static void inline __pinrt_node(NODE *nod)
{
	TYPE *view;
	view = container_of(nod);
	printf("%d\t\t%s\t\t%d\n",view->id,view->song_name,view->name_len);
}

NODE * insert(NODE * head,NODE * new)
{
	if(head == NULL){ 
		INIT_LIST_HEAD(new);
		head=new;
	}else{
		list_add(new,head);
	}
	return head;
}

NODE * delnode(NODE * head,int num)
{
	NODE *pb = head;
	TYPE *temp;
	
	if(head==NULL){ 
		goto end;
	}
	//search each node of the list
	do{
		temp = container_of(pb);
		pb=pb->next;
		if(temp->id == num)
			break;
	}while(pb!=head);
	//if we find the node we want
	if(temp->id == num){ 
		if(pb==head)
			head=pb->next;
		
		list_del(pb);
	}
end:
	return head;
}

NODE * search(NODE * head,int num)
{
	NODE *pb = head;
	TYPE *temp;
	
	if(head==NULL){ 
		goto end;
	}
	do{
		temp = container_of(pb);
		if(temp->id == num)
			break;
		pb=pb->next;	
	}while(pb!=head);
	if(temp->id == num)
		return pb;
	else
		printf("The node not been found!\n");
end:
	return NULL;
}

void move_node(NODE * head,int old,int new)
{
	NODE *pold,*pnew;
	
	pold = search(head,old);
	pnew = search(head,new);
	
	list_move(pold,pnew);
}

NODE * swap_node(NODE * head)
{
	NODE *mnod,*pf,*pb;
	
	pb = pf = head;
	if(list_is_singular(head))
		return head;
	if(pf->prior == pb)
		list_move(pf->prior,pb);	
	
	pf = pf->prior->prior;
	while(pf != pb){
		mnod = pf->next;
		list_move(mnod,pb);
		pb = mnod;
		pf = pf->prior;
	}
	
	return head;
}	

void free_node(NODE *head)
{
	NODE *pb = head;
	TYPE *temp;
	
	if(head==NULL){ 
		goto end;
	}

	do{
		temp = container_of(pb);
		media_name_put(temp);
		pb=pb->next;
	}while(pb!=head);

end:
	return;
}

void print(NODE * head)
{
	NODE * pb = head;
	Sgprintf("\n======================the list is=====================\n");
	printf("ID\t\tname\t\t\t\tlength\n");
	if (pb == NULL){
		printf("List is empty!!\n");
		return;
	}
	
	do{
		__pinrt_node(pb);
		pb=pb->next;
	}while(pb != head);
	printf("\n");
}



media_name_t *media_name_create(char *name, int id)
{
	media_name_t *pmd_name;
	unsigned long name_size;

	pmd_name = malloc(sizeof(media_name_t));
	if(pmd_name == NULL){
		Sgprintf("[err]:<sunsong>  malloc media name failed!\n");
		return NULL;
	}

	name_size = strlen(name);
	pmd_name->song_name = malloc(name_size + 1);
	if(pmd_name->song_name == NULL){
		Sgprintf("[err]:<sunsong>  malloc song name in media failed!\n");
		return NULL;
	}

	memset(pmd_name->song_name, 0 ,name_size+1);
	memcpy(pmd_name->song_name, name, name_size);
	pmd_name->name_len = name_size;

	pmd_name->id = id;

	return pmd_name;
		
}


void media_name_init(media_name_t *pmd_name)
{
	pmd_name->song_name = NULL;
	pmd_name->name_len = 0;
	return;
}

void media_name_put(media_name_t *pmd_name)
{
	if(pmd_name == NULL)
		return;
	
	if(pmd_name->song_name){
		free(pmd_name->song_name);
		pmd_name->song_name = NULL;
	}
	pmd_name->name_len = 0;

	free(pmd_name);
	
	return;
}


static int __song_name_check(char * song_name)
{
	int i;
	int flag = 0;
	int ext_kinds;
	char *p_pot;

	p_pot = strchr(song_name, '.');

	if(p_pot == NULL)
		return 0;

	ext_kinds = sizeof(buf_music_ext)/sizeof(buf_music_ext[0]);
	for(i=0; i<ext_kinds; i++){
		if(!strcmp(p_pot,buf_music_ext[i])){
			flag = 1;
			break;
		}
	}
	return flag;
}

struct list_head *sun_scan_media(char *dir_path, unsigned long *count)
{
	DIR *dir;
	struct dirent* ptr;
	struct list_head *list = NULL;
	media_name_t *sg_name;
	unsigned long sg_count = 0;

	//open the direction
	dir=opendir(dir_path);
	if(dir==NULL)
		perror("dir open");

	while((ptr=readdir(dir))!=NULL)
	{	

		printf("++++++++%s\n",ptr->d_name);
		if(ptr->d_name[0]=='.')
			continue;
		//it 's weather a song file such as "xxx.mp3 or xxx.wma"
		if(!__song_name_check(ptr->d_name))
			continue;

		//save the song file
		sg_name = media_name_create(ptr->d_name, sg_count+1);
		list = insert(list, &sg_name->list);
		sg_count++;

	}
	//close the direction
	if(closedir(dir)==-1)
		perror("dir close");

	//set the song count
	*count = sg_count;
	Sgprintf("[info]: the song number is %d\n",sg_count);
	print(list);		//debug
	
	return list;
}

media_name_t* sun_get_media(struct list_head *head, int id)
{
	NODE *nod;	
	media_name_t *sg_name;

	nod = search(head, id);
	if(nod == NULL){
		Sgprintf("[warning]:<sunsong>  there is no song you wanted!\n");
		return NULL;
	}
	sg_name = container_of(nod);

	return sg_name;
}

void sun_release_media(struct list_head *head)
{
	free_node(head);
}


int sun_play_song(char *song_path)
{
	pid_t sun_pid;

	printf("==========start playing song <%s>==========\n",song_path);

	sun_pid = fork();
	if(sun_pid < 0){
		perror("fork failed!\n");
	}
	if(sun_pid == 0){
		execl("./mplayer","mplayer","-ac","mad","-slave","-quiet", song_path, NULL);
	}else{
		usleep(1000);
		waitpid(sun_pid,NULL,0);
		Sgprintf("==========finish playing song==========\n");
	}

	return 0;
}

int sun_play_media(char *song_dir, media_name_t *song_media)
{
	int dir_len = strlen(song_dir);
	int song_len = song_media->name_len;
	char *song_path;

	song_path = malloc(dir_len+song_len+1);
	if(song_path == NULL){
		Sgprintf("[err]: <sunsong> malloc song path failed!\n");
		return -1;
	}
	memset(song_path,0,dir_len+song_len+1);
	sprintf(song_path,"%s%s",song_dir,song_media->song_name);
	
	sun_play_song(song_path);

	free(song_path);
	return 0;
}



