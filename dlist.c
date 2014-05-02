/*
 * dlist.c
 * This file is part of dlist and dutils library collection
 * Copyright (C) 2014  Darcy Bras da Silva
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

#include "dlist.h"

/****************************************************************************
 * dlist library interface implementation
 ****************************************************************************/


struct dlist_list *dlist_init(struct dlist_list *list,
			      void *(*node_alloc)(size_t),
			      void (*node_dalloc)(void *))
{
	list->count = 0;
	list->node_alloc = (node_alloc ? node_alloc : DLIST_DEF_ALLOC);
	list->node_dalloc = (node_dalloc ? node_dalloc : DLIST_DEF_DALLOC);

	list->head = NULL;
	list->tail = NULL;
	return list;
}/* dlist_init */


struct dlist_list *dlist_list_new(void *(*node_alloc)(size_t),
				  void (*node_dalloc)(void *))
{
	node_alloc = (node_alloc ? node_alloc : DLIST_DEF_ALLOC);
	node_dalloc = (node_dalloc ? node_dalloc : DLIST_DEF_DALLOC);

	struct dlist_list *list = NULL;

	if ( NULL == (list = node_alloc( sizeof( struct dlist_list))) ) {
		//FIXME: add support for custom error logging and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	list->node_alloc = node_alloc;
	list->node_dalloc = node_dalloc;
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}/* dlist_list_new */


struct dlist_node *dlist_node_new(struct dlist_list *list,
				  void *data, void (*dalloc)(void *))
{
	struct dlist_node *node = NULL;

	if ( NULL == (node = list->node_alloc(sizeof( struct dlist_node))) ) {
		//FIXME: add support for custom error loggin and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	node->data = data;
	node->data_dalloc = dalloc;
	node->next = NULL;
	node->prev = NULL;

	return node;
}/* dlist_node_new */


inline void dlist_node_print(const struct dlist_node *node,
			     void (*print)(void *))
{
	if ( !node || !print )
		return;
	print(node->data);
}/* dlist_node_print */


void dlist_node_delete(struct dlist_list *list,
		       struct dlist_node *node)
{
	if ( !list || !node )
		return;

	//FIXME: add debug warning for existing 'node->data
	//------ without _node_dalloc
	if ( node->data && node->data_dalloc )
		node->data_dalloc( node->data );

	list->node_dalloc(node);
	node = NULL;
}/* dlist_node_delete */


void dlist_list_delete(struct dlist_list *list)
{
	if (!list)
		return;

	void (*node_dalloc)(void *) = list->node_dalloc;
	node_dalloc(list);
}/* dlist_list_delete */


struct dlist_node *dlist_node_push(struct dlist_list *list,
				   struct dlist_node *node)
{
	if ( !list || !node )
		return NULL;

	node->next = list->head;

	//is this our first node ?
	if (!list->head)
		list->tail = node;
	else
		list->head->prev = node;

	node->prev = NULL;
	++list->count;

	list->head = node;

	return node;
}/* dlist_node_push */


struct dlist_node *dlist_node_append(struct dlist_list *list,
				     struct dlist_node *node)
{
	if ( !list || !node )
		return NULL;

	//check add @ head
	if ( !list->head ) {
		list->head = node;
		node->next = NULL;
		node->prev = NULL;
		list->tail = node;
		++list->count;
		return node;
	}

	list->tail->next = node;
	node->next = NULL;
	node->prev = list->tail;
	list->tail = node;
	++list->count;

	return node;
}/* dlist_node_append */


struct dlist_node *dlist_node_pop(struct dlist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	struct dlist_node *node = list->head;

	if ( list->head == list->tail )
		list->tail = NULL;

	list->head = list->head->next;
	if ( list->head )
		list->head->prev = NULL;

	--list->count;
	return node;
}/* dlist_node_pop */


struct dlist_node *dlist_node_find(struct dlist_list *list, void *key,
				   int (*cmp)(void *a, void *b))
{

	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct dlist_node *iter;
	//check if found @ head
	if ( 0 == cmp(list->head->data, key) )
		return list->head;

	if ( list->head != list->tail && 0 == cmp(list->tail->data, key) )
		return list->tail;

	for(iter = list->head; NULL != iter->next; iter = iter->next)
	{
		if ( 0 == cmp(iter->next->data, key) )
			break;
	}

	return iter->next;

}/* dlist_node_find */


size_t dlist_find_index_of(struct dlist_list *list, void *key,
			   int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return 0;

	struct dlist_node *iter;
	size_t idx = 1;

	for(iter = list->head; NULL != iter; iter = iter->next, ++idx)
	{
		if ( 0 == cmp(iter->data, key) )
			break;
	}

	if ( !iter )
		return 0;

	return idx;
}/* dlist_find_index_of */


struct dlist_node *dlist_node_remove(struct dlist_list *list, void *key,
				     int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct dlist_node *iter;
	struct dlist_node *pnode;
	//check found @ head
	if ( 0 == cmp(list->head->data, key) ) {
		pnode = list->head;
		if ( list->head == list->tail )
			list->tail = NULL;

		list->head = list->head->next;
		if ( list->head )
			list->head->prev = NULL;
		--list->count;

		return pnode;
	}

	//check found @ tail
	if ( list->head != list->tail && 0 == cmp(list->tail->data, key) ) {
		pnode = list->tail;
		list->tail->prev->next = NULL;
		list->tail = list->tail->prev;
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
	pnode->next->prev = pnode->prev;
	pnode->prev->next = pnode->next;
	--list->count;
	return pnode;
}/* dlist_node_remove */


struct dlist_node *dlist_node_remove_at(struct dlist_list *list,
				       const size_t index)
{
	if ( !list || !list->head || 0 == index || index > list->count )
		return NULL;

	//check remove @ head
	if ( 1 == index )
		return dlist_node_pop(list);

	struct dlist_node *node = NULL;
	//check remove @ tail
	if ( list->count == index ) {
		node = list->tail;
		list->tail->prev->next = NULL;
		list->tail = list->tail->prev;
		--list->count;
		return node;
	}

	node = list->head;
	for(size_t idx=1; idx < index; ++idx)
		node = node->next;

	node->prev->next = node->next;
	node->next->prev = node->prev;

	--list->count;
	return node;
}/* dlist_node_remove_at */


void dlist_node_foreach(struct dlist_list *list,
			void *(*action)(void *carry, void *data, void *param),
			void *param)
{
	if ( !list || !list->head || !action )
		return;

	struct dlist_node *iter = NULL;
	void *carry = NULL;

	for(iter = list->head; NULL != iter; iter = iter->next)
		carry = action(carry, iter->data, param);

	return;
}/* dlist_node_foreach */


struct dlist_list *dlist_list_delete_all_nodes(struct dlist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	while( NULL != list->head )
		dlist_node_delete(list, dlist_node_pop(list));

	return list;
}/* dlist_list_delete_all_nodes */


struct dlist_list *dlist_list_reverse(struct dlist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	struct dlist_node *node = NULL;
	struct dlist_node *head = list->head;
	while ( head )
	{
		node = head->prev;
		head->prev = head->next;
		head->next = node;
		head = head->prev;
	}

	//set new head and return
	node = list->tail;
	list->tail = list->head;
	list->head = node;

	return list;
}/* dlist_list_reverse */


inline size_t dlist_get_size(struct dlist_list *list)
{
	return list->count;
}/* dlist_get_size */


struct dlist_list *dlist_list_push(struct dlist_list *list,
				   struct dlist_list *s_list)
{
	if ( !list || !s_list || !s_list->head )
		return NULL;

	if ( !list->head ) {
		list->head = s_list->head;
		list->count = s_list->count;
		//empty s_list
		s_list->head = NULL;
		s_list->count = 0;
		return list;
	}

	s_list->tail->next = list->head;
	list->head->prev = s_list->tail;

	list->head = s_list->head;
	list->head->prev = NULL;
	list->count += s_list->count;

	s_list->head = NULL;
	s_list->tail = NULL;
	s_list->count = 0;
	return list;
}/* dlist_list_push */


struct dlist_list *dlist_list_append(struct dlist_list *list,
				     struct dlist_list *s_list)
{
	if ( !list || !s_list || !s_list->head )
		return NULL;

	if ( !list->head ) {
		list->head = s_list->head;
		list->count = s_list->count;
		//empty s_list
		s_list->head = NULL;
		s_list->count = 0;
		return list;
	}

	list->tail->next = s_list->head;
	s_list->head->prev = list->tail;
	list->count += s_list->count;

	list->tail = s_list->tail;

	s_list->head = NULL;
	s_list->tail = NULL;
	s_list->count = 0;

	return list;
}/* dlist_list_append */


struct dlist_list *dlist_list_split(struct dlist_list *list, void *key,
				    int (*cmp)(void *a, void *b))
{
	if ( !list || !list->head || !key || !cmp )
		return NULL;

	struct dlist_list *n_list = NULL;
	struct dlist_node *iter = NULL;

	//check key @ head
	if ( 0 == cmp(list->head->data, key) ) {
		n_list = dlist_list_new(list->node_alloc, list->node_dalloc);
		if ( !n_list ) {
			//FIXME: add support for custom error loggin and msg
			fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
				__LINE__,__func__);

			return NULL;
		}

		n_list->head = list->head;
		n_list->tail = list->tail;
		n_list->count = list->count;
		//make list empty
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
		return n_list;
	}

	//help us update list sizes without transversing twice
	size_t idx = 0;
	//search for key
	for(iter = list->head; NULL != iter->next; iter = iter->next, ++idx)
	{
		if ( 0 == cmp(iter->next->data, key) )
			break;
	}

	//check if node was found
	if ( !iter->next )
		return NULL;

	n_list = dlist_list_new(list->node_alloc, list->node_dalloc);

	if ( !n_list ) {
		//FIXME: add support for custom error loggin and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	//keep in mind we are one node behind so we can remove/trim
	n_list->head = iter->next;
	n_list->head->prev = NULL;
	n_list->tail = list->tail;
	n_list->count = list->count - idx - 1;
	list->tail = iter;
	list->count = idx + 1;
	iter->next = NULL;

	return n_list;
}/* dlist_list_split */


struct dlist_list *dlist_list_split_at(struct dlist_list *list,
				       const size_t index)
{
	if ( !list || !list->head || 0 == index || index > list->count )
		return NULL;

	struct dlist_list *n_list = NULL;
	n_list = dlist_list_new(list->node_alloc, list->node_dalloc);

	if ( !n_list ) {
		//FIXME: add support for custom error loggin and msg
		fprintf(stderr,"%s[%d]:%s alloc failed\n", __FILE__,
			__LINE__,__func__);

		return NULL;
	}

	//check @ head
	if ( 1 == index ) {
		n_list->head = list->head;
		n_list->tail = list->tail;
		n_list->count = list->count;
		//make list empty
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
		return n_list;
	}

	struct dlist_node *head = NULL;
	struct dlist_node *prev = NULL;
	size_t idx;

	prev = head = list->head;

	for(idx= index -1; idx; --idx)
	{
		prev = head;
		head = head->next;
	}

	prev->next->prev = NULL;
	n_list->tail = list->tail;
	prev->next = NULL;
	n_list->head = head;

	//remove at tail ?
	if ( list->count == index )
		n_list->count = list->count - index + 1;
	else
		n_list->count = index;

	list->count = index - 1;

	return n_list;
}/* dlist_list_split_at */
