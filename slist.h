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

/****************************************************************************
 * standard libraries
 ****************************************************************************/
#include <stdio.h>
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


/* returns a new allocated 'list' initialized with
 * ------- 'node_alloc and 'node_dalloc'.
 * ------- 'list' has to be freed
 * passing NULL to 'node_alloc' sets it to SLIST_DEF_ALLOC
 * passing NULL to node_dalloc sets it to SLIST_DEF_DALLOC.
 * NULL is returned if node_alloc fails
 *
 * ABOUT [ node_alloc , node_dalloc ]:
 * ------- this can either be library defaults or user defined
 * ------- this functions must provided simillar interface to
 * ------- SLIST_DEF_ALLOC and SLIST_DEF_DALLOC which are often
 * ------- defined as 'malloc' and 'free' from <stdlib.h>
 *
 * passing invalid ['list' or 'node_alloc' or 'node_dalloc' ]
 * ------- results in undefined behavior
 */
struct slist_list *slist_new_list(void *(*node_alloc)(size_t),
				  void (*node_dalloc)(void *));


/* returns a new allocated 'node' initialized with 'data' and 'dalloc' 
 * ------- returned 'node' needs to be freed
 * passing NULL in 'data' sets 'node' 'data' to NULL
 * passing NULL in 'dalloc' sets 'node' 'dalloc' to NULL
 * ------- if 'dalloc' is set, _delete_ functions attemp to delete 'data'
 *
 * passing invalid ['list' or 'data' or 'dalloc' ]
 * ------- results in undefined behavior
 */
struct slist_node *slist_new_node(struct slist_list *list,
				  void *data, void (*dalloc)(void *));


/* executes 'print' on 'node' 'data' member
 * passing NULL in 'node' returns with no operation executed
 * passing NULL in 'print' returns with no operation executed
 *
 * passing invalid ['node' or 'print' ]
 * ------- results in undefined behavior
 *
 * FIXME_NOTE: consider using a macro instead. using inline for now
 */
void slist_print_node(const struct slist_node *node,
			     void (*print)(void *));


/* deletes 'node'
 * ------- returned 'node' needs to be freed
 * attempts to delete 'data' when 'data_dalloc' is set.
 * passing NULL in 'list' returns with no operation executed
 * passing NULL in 'node' returns with no operation executed
 *
 * passing invalid ['list' or 'node' ]
 * ------- results in undefined behavior
 */
void slist_delete_node(struct slist_list *list,
		       struct slist_node *node);


/* deletes 'list'
 * passing NULL in 'list' returns with no operation executed
 *
 * NOTE: deleting the 'list0 won't free the nodes contained in it.
 * ------- see slist_delete_all_nodes for that.
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
void slist_delete_list(struct slist_list *list);


/* returns the 'list' head new head
 * passing NULL in 'list' returns NULL
 * passing NULL in 'node' returns NULL
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct slist_node *slist_push_node(struct slist_list *list,
				   struct slist_node *node);


/* adds 'node' to the end of 'list', returning it.
 * passing NULL in 'list' returns NULL
 * passing NULL in 'node' returns NULL
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct slist_node *slist_append_node(struct slist_list *list,
				     struct slist_node *node);


/* removes 'list' head and returns it
 * passing NULL in 'list' returns NULL
 * passing an empty 'list' returns NULL
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct slist_node *slist_pop_node(struct slist_list *list);


/* returns the 'node matching 'key'
 * returns NULL if 'key' is not found
 * passing NULL in 'list' returns NULL
 * passing an empty 'list' returns NULL
 * passing NULL 'key' returns NULL
 * passing NULL 'cmp' function returns NULL
 *
 * ABOUT 'cmp': function needs to return 0 when 'a' and 'b' match
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
struct slist_node *slist_find_node(struct slist_list *list, void *key,
				   int (*cmp)(void *a, void *b));

#endif
