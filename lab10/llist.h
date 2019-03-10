//llist.h
#ifndef LLIST_H
#define LLIST_H

#include <stdlib.h>
#include <string.h>
#include "tools.h"

typedef enum
{
	ByPointer, ByValue
} ListItemType;

typedef struct TLinkedListNode
{
	void *item;
	struct TLinkedListNode *nextNode;
} LinkedListNode;

typedef struct TLinkedList
{
	LinkedListNode *firstNode;
	Comparision *comparer;
	ListItemType type;
	size_t itemSize;
	size_t count;
} LinkedList;

LinkedList *llist_create(Comparision *comparer, const ListItemType type, const size_t itemSize);
void llist_add(LinkedList *list, const void *item);
void llist_addItem(LinkedList *list, void *item);
void llist_addValue(LinkedList *list, const void *valuePtr);
void llist_remove(LinkedList *list, const void *item);
void llist_removeItem(LinkedList *list, const void *item);
void llist_removeValue(LinkedList *list, const void *valuePtr);
bool llist_containsItem(const LinkedList *list, const void *item);
bool llist_containsValue(const LinkedList *list, const void *valuePtr);
void llist_rearrange(LinkedList *list, Comparision *comparer);
void llist_dispose(LinkedList *list);
void llist_clear(LinkedList* list);

#define llist_foreach(list, prefix) for (LinkedListNode* prefix##_node = list->firstNode; prefix##_node != NULL; prefix##_node = prefix##_node->nextNode)
//NOTE! use '_node->item' to get access to item

#endif