/*
 * slist.c
 * This file is part of slist and dutils library collection
 *  Copyright (C) 2014  Darcy Bras da Silva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "slist.h"

/****************************************************************************
 * library interface implementation
 ****************************************************************************/

struct slist_list *slist_init(struct slist_list *list,
			      void *(*node_alloc)(size_t),
			      void (*node_dalloc)(void *))
{
	list->count = 0;
	list->node_alloc = (node_alloc ? node_alloc : SLIST_DEF_ALLOC);
	list->node_dalloc = (node_dalloc ? node_dalloc : SLIST_DEF_DALLOC);

	list->head = NULL;
	return list;
}/* slist_init */

struct slist_list *slist_list_new(void *(*node_alloc)(size_t),
				  void (*node_dalloc)(void *))
{
	node_alloc = (node_alloc ? node_alloc : SLIST_DEF_ALLOC);
	node_dalloc = (node_dalloc ? node_dalloc : SLIST_DEF_DALLOC);

	struct slist_list *list = NULL;

	if ( NULL == (list = node_alloc( sizeof( struct slist_list))) ) {
		//FIXME: add support for custom error logging and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	list->node_alloc = node_alloc;
	list->node_dalloc = node_dalloc;
	list->count = 0;
	list->head = NULL;

	return list;
}/* slist_list_new */

struct slist_node *slist_node_new(struct slist_list *list,
				  void *data, void (*dalloc)(void *))
{

	struct slist_node *node = NULL;

	if ( NULL == (node = list->node_alloc( sizeof( struct slist_node))) ) {
		//FIXME: add support for custom error loggin and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	node->data = data;
	node->data_dalloc = dalloc;
	node->next = NULL;

	return node;
}/* slist_node_new */

inline void slist_node_print(const struct slist_node *node,
			     void(*print)(void *))
{
	if ( !node || !print )
		return;
	print(node->data);
}/* slist_node_print */

void slist_node_delete(struct slist_list *list,
		       struct slist_node *node)
{
	if (!list || !node )
		return;

	//FIXME: add debug warning for existing 'node->data
	//------ without _node_dalloc
	if ( node->data && node->data_dalloc )
		node->data_dalloc( node->data );

	list->node_dalloc(node);
	node = NULL;
}/* slist_node_delete */

void slist_list_delete(struct slist_list *list)
{
	if (!list)
		return;

	void (*node_dalloc)(void *) = list->node_dalloc;
	node_dalloc(list);
}/* slist_list_delete */

struct slist_node *slist_node_push(struct slist_list *list,
				   struct slist_node *node)
{
	if ( !list || !node )
		return NULL;

	node->next = list->head;
	list->head = node;
	++list->count;

	return node;
}/* slist_node_push */

struct slist_node *slist_node_append(struct slist_list *list,
				     struct slist_node *node)
{
	if ( !list || !node )
		return NULL;

	struct slist_node *pnode = NULL;
	//check add @ head
	if ( !list->head ) {
		list->head = node;
		++list->count;
		return node;
	}

	pnode = list->head;
	while( NULL != pnode->next )
		pnode = pnode->next;

	pnode->next = node;
	//ensure a node is a working tail
	node->next = NULL;
	++list->count;
	return pnode;
}/* slist_node_append */

struct slist_node *slist_node_pop(struct slist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	struct slist_node *node = list->head;

	list->head = list->head->next;
	--list->count;
	return node;
}/* slist_node_pop */

struct slist_node *slist_node_find(struct slist_list *list, void *key,
				   int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct slist_node *iter;
	//check if found @ head
	if ( 0 == cmp(list->head->data, key) )
		return list->head;

	for(iter = list->head; NULL != iter->next; iter = iter->next)
	{
		if ( 0 == cmp(iter->next->data, key) )
			break;
	}

	return iter->next;

}/* slist_node_find */

size_t slist_find_index_of(struct slist_list *list, void *key,
			   int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return 0;

	struct slist_node *iter;
	size_t idx = 1;

	for(iter = list->head; NULL != iter; iter = iter->next, ++idx)
	{
		if ( 0 == cmp(iter->data, key) )
			break;
	}

	if( !iter )
		return 0;

	return idx;
}/* slist_find_index_of */

struct slist_node *slist_node_remove(struct slist_list *list, void *key,
				     int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct slist_node *iter;
	struct slist_node *pnode;

	//check found @ head
	if ( 0 == cmp(list->head->data, key) ) {
		pnode = list->head;
		list->head = list->head->next;
		--list->count;
		return pnode;
	}

	for(iter = list->head; NULL != iter->next; iter = iter->next)
	{
		if ( 0 == cmp(iter->next->data, key) )
			break;
	}

	//see if found a node
	if ( !iter->next )
		return NULL;

	pnode = iter->next;
	iter->next = pnode->next;
	--list->count;
	return pnode;

}/* slist_node_remove */

struct slist_node *slist_node_remove_at(struct slist_list *list,
					const size_t index)
{
	if ( !list || !list->head || index == 0 || index > list->count )
		return NULL;

	struct slist_node *head = list->head;
	struct slist_node *prev = NULL;

	size_t idx = 1;

	if ( 1 == index )
		return slist_node_pop(list);

	do{
		prev = head;
		head = head->next;
	}while( index > ++idx );

	prev->next = head->next;
	--list->count;

	return head;

}/* slist_node_remove_at */

void slist_node_foreach(struct slist_list *list,
			void *(*action)(void *carry, void *data, void *param),
			void *param)
{
	if ( !list || !list->head || !action )
		return;

	struct slist_node *iter = NULL;
	void *carry = NULL;

	for(iter = list->head; NULL != iter; iter = iter->next)
	{
		carry = action(carry, iter->data, param);
	}

}/* slist_node_foreach */

struct slist_list *slist_nodes_delete_all(struct slist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	while( NULL != list->head )
	{
		slist_node_delete(list, slist_node_pop(list));
	}

	return list;

}/* slist_nodes_delete_all */

struct slist_list *slist_list_reverse(struct slist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	struct slist_node *head = list->head;
	struct slist_node *prev = NULL;
	struct slist_node *next = NULL;

	while ( head )
	{
		//set the head as last node
		list->head = head;
		//save the heads next in next
		next = head->next;
		//change the next to previous
		head->next = prev;
		//set the current as previous
		prev = head;
		//set current as next
		head = next;
	}

	//set new head and return
	next = list->head;
	list->head = next;

	return list;
}/* slist_list_reverse */

inline size_t slist_get_size(struct slist_list *list)
{
	return list->count;
}/* slist_get_size */

struct slist_list *slist_list_push(struct slist_list *list,
				   struct slist_list *s_list)
{
	if ( !list || !s_list || !s_list->head )
		return NULL;

	struct slist_node *iter = s_list->head;
	struct slist_node *head = list->head;

	if ( !list->head ) {
		list->head = s_list->head;
		list->count = s_list->count;
		//empty s_list
		s_list->head = NULL;
		s_list->count = 0;
		return list;
	}

	//skip to end of s_list
	while( NULL != iter->next )
		iter = iter->next;

	list->head = s_list->head;
	iter->next = head;
	list->count += s_list->count;
	//empty s_list
	s_list->head = NULL;
	s_list->count = 0;

	return list;

}/* slist_list_push */

struct slist_list *slist_list_append(struct slist_list *list,
				     struct slist_list *s_list)
{
	if ( !list || !s_list || !s_list->head )
		return NULL;

	struct slist_node *iter = list->head;

	if ( !list->head ) {
		list->head = s_list->head;
		list->count = s_list->count;
		//empty s_list
		s_list->head = NULL;
		s_list->count = 0;
		return list;
	}

	//slip to end of list
	while( NULL != iter->next )
		iter = iter->next;

	iter->next = s_list->head;
	list->count += s_list->count;
	//empty s_list
	s_list->head = NULL;
	s_list->count = 0;

	return list;

}/* slist_list_append */

struct slist_list *slist_list_split(struct slist_list *list, void *key,
				    int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct slist_list *n_list = NULL;
	struct slist_node *iter = NULL;

	//check key @ head
	if ( 0 == cmp(list->head->data, key) ) {
		n_list = slist_list_new(list->node_alloc, list->node_dalloc);
		if ( NULL == n_list )
			return NULL;

		n_list->head = list->head;
		n_list->count = list->count;
		//make list empty
		list->head = NULL;
		list->count = 0;
		return n_list;
	}

	//help us update list sizes without transversing
	size_t idx = 0;

	//search for key
	for(iter = list->head; NULL != iter->next; iter = iter->next, ++idx)
	{
		if ( 0 == cmp(iter->next->data, key) )
			break;
	}

	//check if node was found
	if ( NULL == iter->next )
		return NULL;

	n_list = slist_list_new(list->node_alloc, list->node_dalloc);

	if ( NULL == n_list )
		return NULL;

	//keep in mind we are one node behind so we can remove/trim
	n_list->head = iter->next;
	n_list->count = list->count - idx - 1;
	list->count = idx + 1;
	iter->next = NULL;

	return n_list;

}/* slist_list_split */

struct slist_list *slist_list_split_at(struct slist_list *list,
				       const size_t index)
{

	if ( !list || !list->head || 0 == index || index > list->count )
		return NULL;

	struct slist_list *n_list = NULL;
	struct slist_node *head = NULL;
	struct slist_node *prev = NULL;

	size_t idx = 1;

	//create our slist
	n_list = slist_list_new(list->node_alloc, list->node_dalloc);

	if ( !n_list )
		return NULL;

	//remove at head
	if ( 1 == index ) {
		n_list->head = list->head;
		n_list->count = list->count;
		list->head = NULL;
		list->count = 0;
		return n_list;
	}

	prev = head = list->head;

	for(idx = index - 1; idx; --idx)
	{
		prev = head;
		head = head->next;
	}


	prev->next = NULL;
	n_list->head = head;

	//remove at tail ?
	if (list->count == index ) {
		n_list->count = list->count - index + 1;
	} else {
		n_list->count = index;
	}

	list->count = index - 1;

	return n_list;

}/* slist_list_split_at */
