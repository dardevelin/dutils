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
