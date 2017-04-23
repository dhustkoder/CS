#ifndef CS_DATA_STRUCTURES_LINKED_LIST_H_
#define CS_DATA_STRUCTURES_LINKED_LIST_H_
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "iterator.h"


typedef struct Node {
	struct Node* next;
	unsigned char value[];
} Node;


typedef struct LinkedList {	
	Node* head;
	Node* tail;
	int membsize;
	int count;
} LinkedList;


static inline LinkedList* create_linked_list(const int membsize)
{
	LinkedList* const l = malloc(sizeof(LinkedList));
	if (l == NULL)
		return NULL;

	l->head = NULL;
	l->tail = NULL;
	l->membsize = membsize;
	l->count = 0;
	return l;
}


static inline void destroy_linked_list(LinkedList* const l)
{
	Node* p = l->head;
	const int count = l->count;

	for (int i = 0; i < count; ++i) {
		Node* const tmp = p;
		p = p->next;
		free(tmp);
	}

	free(l);
}


static inline Node* linked_list_get_node(const int index, const LinkedList* const l)
{
	if (index < 0 || index >= l->count)
		return NULL;

	Node* p = l->head;
	for (int i = 0; i < index; ++i)
		p = p->next;

	return p;
}


static inline void linked_list_remove_node(const int index, LinkedList* const l)
{
	assert(index >= 0 && index < l->count);

	Node* rm;

	if (index == 0) {
		rm = l->head;
		l->head = l->head->next;
		if (rm == l->tail)
			l->tail = l->head;
	} else if (index == l->count - 1) {
		rm = l->tail;
		l->tail = linked_list_get_node(index - 1, l);
		l->tail->next = NULL;
	} else {
		Node* const p = linked_list_get_node(index - 1, l);
		rm = p->next;
		p->next = rm->next;
	}

	l->count -= 1;
	free(rm);
}


static inline void linked_list_push_data_into_node(const void* const data, const int size, Node** const node)
{
	assert(data != NULL && node != NULL);

	if ((*node) == NULL) {
		(*node) = malloc(sizeof(Node) + size);
		(*node)->next = NULL;
	}

	memcpy((*node)->value, data, size);
}


static inline void linked_list_push_back(const void* const data, LinkedList* const l)
{
	if (l->head == NULL) {
		linked_list_push_data_into_node(data, l->membsize, &l->head);
		l->tail = l->head;
	} else {
		linked_list_push_data_into_node(data, l->membsize, &l->tail->next);
		l->tail = l->tail->next;
	}

	++l->count;
}


static inline void linked_list_pop_back(LinkedList* const l)
{
	linked_list_remove_node(l->count - 1, l);
}


static inline void* linked_list_get(const int index, const LinkedList* const l)
{
	return linked_list_get_node(index, l)->value;
}


static inline ConstIterator linked_list_cbegin(const LinkedList* const l)
{
	if (l->head != NULL) {
		const ConstIterator it = { .ds.ll = l, l->head->value, 0 };
		return it;
	}
	const ConstIterator it = { .ds.ll = l, NULL, 0 };
	return it;
}


static inline ConstIterator linked_list_cend(const LinkedList* const l)
{
	const ConstIterator it = { .ds.ll = l, NULL, l->count };
	return it;
}


static inline ConstIterator linked_list_cadvance(const ConstIterator it, const int n)
{
	const LinkedList* const l = it.ds.ll;
	const int newindex = it.index + n;
	const Node* const node = linked_list_get_node(newindex, l);
	const void* const ptr = node != NULL ? node->value : NULL;
	const ConstIterator r = { it.ds, ptr, newindex };
	return r;
}


static inline Iterator linked_list_begin(const LinkedList* const l)
{
	const ConstIterator cit = linked_list_cbegin(l);
	const Iterator it = { cit.ds, (void*) cit.ptr, cit.index };
	return it;
}


static inline Iterator linked_list_end(const LinkedList* const l)
{
	const ConstIterator cit = linked_list_cend(l);
	const Iterator it = { cit.ds, (void*) cit.ptr, cit.index };
	return it;
}


static inline Iterator linked_list_advance(const Iterator it, const int n)
{

	const ConstIterator cit = { it.ds, it.ptr, it.index };
	const ConstIterator rit = linked_list_cadvance(cit, n);
	const Iterator r = { rit.ds, (void*) rit.ptr, rit.index };
	return r;
}


#endif

