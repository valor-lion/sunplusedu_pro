#ifndef __linker_h__
#define __linker_h__


#define offsetof(_TYPE, _LIST) ((int)&((_TYPE *)0)->_LIST)

//从当前节点开始遍历链表
#if 0
#define list_for_each_entry_from(pos, head, member) 			\
	for (; prefetch(pos->member.next), &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))
#endif

typedef struct list_head
{
	struct list_head *prior;
	struct list_head *next;
}NODE;



extern void INIT_LIST_HEAD(struct list_head *list);
extern void list_add(struct list_head *new, struct list_head *head);
extern void list_add_tail(struct list_head *new, struct list_head *head);
extern void list_del(struct list_head *entry);
extern void list_replace(struct list_head *old,struct list_head *new);
extern void list_move(struct list_head *list, struct list_head *head);
extern void list_move_tail(struct list_head *list, struct list_head *head);
extern int list_empty(const struct list_head *head);
extern int list_is_singular(const struct list_head *head);



#endif
