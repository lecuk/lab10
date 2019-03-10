//llist.c
#include "llist.h"

static int noComparision(const void* item1, const void* item2)
{
	return 0;
}

void llist_add(LinkedList *list, void *item)
{
	switch (list->type)
	{
	case ByPointer:
		llist_addItem(list, item);
		break;
	case ByValue:
		llist_addValue(list, item);
		break;
	}
}

void llist_addItem(LinkedList *list, void *item)
{
	LinkedListNode *node = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	if (node)
	{
		node->nextNode = NULL;
		node->item = item;
		LinkedListNode *current = list->firstNode;
		LinkedListNode *previous = NULL;
		while (current && list->comparer(item, current->item) <= 0)
		{
			previous = current;
			current = current->nextNode;
		}
		if (previous)
		{
			previous->nextNode = node;
		}
		else
		{
			list->firstNode = node;
		}
		node->nextNode = current;
		list->count++;
	}
}

void llist_addValue(LinkedList *list, const void *valuePtr)
{
	void *item = malloc(list->itemSize);
	if (item)
	{
		memcpy(item, valuePtr, list->itemSize);
		llist_addItem(list, item);
	}
}

void llist_remove(LinkedList *list, const void *item)
{
	switch (list->type)
	{
	case ByPointer:
		llist_removeItem(list, item);
		break;
	case ByValue:
		llist_removeValue(list, item);
		break;
	}
}
void llist_removeItem(LinkedList *list, const void *item)
{
	LinkedListNode *current = list->firstNode;
	if (current)
	{
		LinkedListNode *previous = NULL;
		while (current && current->item != item)
		{
			previous = current;
			current = current->nextNode;
		}
		if (previous)
		{
			previous->nextNode = current->nextNode;
		}
		else
		{
			list->firstNode = current->nextNode;
		}
		free(current);
		list->count--;
	}
}

void llist_removeValue(LinkedList *list, const void *valuePtr)
{
	LinkedListNode *current = list->firstNode;
	if (current)
	{
		LinkedListNode *previous = NULL;
		while (current && !memcmp(current->item, valuePtr, list->itemSize))
		{
			previous = current;
			current = current->nextNode;
		}
		if (previous)
		{
			previous->nextNode = current->nextNode;
		}
		else
		{
			list->firstNode = current->nextNode;
		}
		free(current);
		list->count--;
	}
}

bool llist_containsItem(const LinkedList *list, const void *item)
{
	LinkedListNode *current = list->firstNode;
	while (current)
	{
		if (current->item == item) return 1;
		current = current->nextNode;
	}
	return 0;
}

bool llist_containsValue(const LinkedList *list, const void *valuePtr)
{
	LinkedListNode *current = list->firstNode;
	while (current)
	{
		if (!memcmp(current->item, valuePtr, list->itemSize)) return 1;
		current = current->nextNode;
	}
	return 0;
}

LinkedList *llist_create(Comparision *comparer, const ListItemType type, const size_t itemSize)
{
	LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
	if (list)
	{
		list->firstNode = NULL;
		if (comparer)
		{
			list->comparer = comparer;
		}
		else
		{
			list->comparer = noComparision;
		}
		list->type = type;
		list->itemSize = itemSize;
		list->count = 0;
	}
	return list;
}

void llist_rearrange(LinkedList *list, Comparision *newComparer)
{
	void **buffer;
	size_t count = list->count;
	buffer = calloc(count, sizeof(void*));
	if (buffer)
	{
		for (size_t i = 0; i < count; i++)
		{
			void *item = list->firstNode->item;
			llist_removeItem(list, item);
			buffer[i] = item;
		}
		list->comparer = newComparer;
		for (size_t i = 0; i < count; i++)
		{
			llist_addItem(list, buffer[i]);
		}
	}
	free(buffer);
}

void llist_clear(LinkedList* list)
{
	while (list->firstNode)
	{
		LinkedListNode *next = list->firstNode->nextNode;
		if (list->type == ByValue) free(list->firstNode->item);
		free(list->firstNode);
		list->firstNode = next;
	}
}

void llist_dispose(LinkedList *list)
{
	llist_clear(list);
	free(list);
}