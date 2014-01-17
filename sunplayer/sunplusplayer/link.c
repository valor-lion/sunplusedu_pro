
/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				link.c  
  *Description:		
  *
  *Date and Edition:		2012-11-18  		v1.0
  *Author:				Valor Lion
  **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "link.h"


static void __list_add(struct list_head *new,struct list_head *prior, struct list_head *next);
static void __list_del(struct list_head * prior, struct list_head * next);


//初始链表第一个节点
void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prior = list;
}

//将new 插入到head 的后面
void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static void __list_add(struct list_head *new,struct list_head *prior, struct list_head *next)
{
	next->prior = new;
	new->next = next;
	new->prior = prior;
	prior->next = new;
}

//将new 插入到head 的前面
void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prior, head);
}

//删除一个节点
void list_del(struct list_head *entry)
{
	__list_del(entry->prior, entry->next);
}

static void __list_del(struct list_head * prior, struct list_head * next)
{
	next->prior = prior;
	prior->next = next;
}

//替换一个节点
void list_replace(struct list_head *old,struct list_head *new)
{
	new->next = old->next;
	new->next->prior = new;
	new->prior = old->prior;
	new->prior->next = new;
}

//将list 移动到head 的后面
void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prior, list->next);
	list_add(list, head);
}
//将list 移动到head 的前面
void list_move_tail(struct list_head *list, struct list_head *head)
{
	__list_del(list->prior, list->next);
	list_add_tail(list, head);
}

//判断一个链表是否为空
int list_empty(const struct list_head *head)
{
	return head->next == head;
}
//判断一个链表是否只有一个节点
int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prior);
}


