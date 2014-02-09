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

struct slist_list *slist_new_list(void *(*node_alloc)(size_t),
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
}/* slist_new_list */

struct slist_node *slist_new_node(struct slist_list *list,
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
}/* slist_new_node */

inline void slist_print_node(const struct slist_node *node,
			     void(*print)(void *))
{
	if ( !node || !print )
		return;
	print(node->data);
}/* slist_print_node */

void slist_delete_node(struct slist_list *list,
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
}/* slist_delete_node */

void slist_delete_list(struct slist_list *list)
{
	if (!list)
		return;

	void (*node_dalloc)(void *) = list->node_dalloc;
	node_dalloc(list);
}/* slist_delete_list */

struct slist_node *slist_push_node(struct slist_list *list,
				   struct slist_node *node)
{
	if ( !list || !node )
		return NULL;

	node->next = list->head;
	list->head = node;
	++list->count;

	return node;
}/* slist_push_node */

struct slist_node *slist_append_node(struct slist_list *list,
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
}/* slist_append_node */

struct slist_node *slist_pop_node(struct slist_list *list)
{
	if ( !list || !list->head )
		return NULL;

	struct slist_node *node = list->head;

	list->head = list->head->next;
	--list->count;
	return node;
}/* slist_pop_node */

struct slist_node *slist_find_node(struct slist_list *list, void *key,
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

}/* slist_find_node */

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

struct slist_node *slist_remove_node(struct slist_list *list, void *key,
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

}/* slist_remove_node */
