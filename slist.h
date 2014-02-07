/*
 * slist.h
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

#ifndef DUTILS_SLIST_H_
#define DUTILS_SLIST_H_

#include <stdlib.h>

#define SLIST_DEF_ALLOC malloc
#define SLIST_DEF_DALLOC free

/****************************************************************************
 * base data structures
 ****************************************************************************/

struct slist_node
{
	void *data;
	void (*data_dalloc)(void *);
	struct slist_node *next;
};

struct slist_list
{
	size_t count;
	void *(*node_alloc)(size_t);
	void (*node_dalloc)(void *);
	struct slist_node *head;
};

/****************************************************************************
 * convinience data types for library consumers/users 
 ****************************************************************************/

typedef struct slist_node slist_node_t;
typedef struct slist_list slist_list_t;


/****************************************************************************
 * library interface and _base_ documentation
 ****************************************************************************/

/* returns 'list' initialized with 'node_alloc' and 'node_dalloc'
 * NULL is returned if 'list' is NULL
 * passing NULL to 'node_alloc' sets it to SLIST_DEF_ALLOC.
 * passing NULL to node_dalloc sets is to SLIST_DEF_DALLOC.
 *
 * passing invalid ['list' or 'node_alloc' or 'node_dalloc' ]
 * ------- results in undefined behavior
 */
struct slist_list *slist_init(struct slist_list *list,
			      void *(*node_alloc)(size_t),
			      void (*node_dalloc)(void *));

#endif
