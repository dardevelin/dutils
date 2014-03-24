/*
 * dlist.h
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

#ifndef DUTILS_DLIST_H_
#define DUTILS_DLIST_H_


/****************************************************************************
 * standard libraries
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>


#define DLIST_DEF_ALLOC malloc
#define DLIST_DEF_DALLOC free


/****************************************************************************
 * base data structures
 ****************************************************************************/


struct dlist_node
{
	void *data;
	struct dlist_node *next;
	struct dlist_node *prev;
	void (*data_dalloc)(void *);
};

struct dlist_list
{
	size_t count;
	struct dlist_node *head;
	struct dlist_node *tail;
	void *(*node_alloc)(size_t);
	void (*node_dalloc)(void *);
};

/****************************************************************************
 * conviniente data types for library consumers/users
 ****************************************************************************/


typedef struct dlist_node dlist_node_t;
typedef struct dlist_list dlist_list_t;


/****************************************************************************
 * library interface and _base_ documentation
 ****************************************************************************/


#endif
