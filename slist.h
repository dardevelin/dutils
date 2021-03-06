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
struct slist_list *slist_list_new(void *(*node_alloc)(size_t),
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
struct slist_node *slist_node_new(struct slist_list *list,
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
void slist_node_print(const struct slist_node *node,
			     void (*print)(void *));


/* deletes 'node'
 * attempts to delete 'data' when 'data_dalloc' is set.
 * passing NULL in 'list' returns with no operation executed
 * passing NULL in 'node' returns with no operation executed
 *
 * passing invalid ['list' or 'node' ]
 * ------- results in undefined behavior
 */
void slist_node_delete(struct slist_list *list,
		       struct slist_node *node);


/* deletes 'list'
 * passing NULL in 'list' returns with no operation executed
 *
 * NOTE: deleting the 'list0 won't free the nodes contained in it.
 * ------- see slist_list_delete_all_nodes for that.
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
void slist_list_delete(struct slist_list *list);


/* returns the 'list' head new head
 * passing NULL in 'list' returns NULL
 * passing NULL in 'node' returns NULL
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct slist_node *slist_node_push(struct slist_list *list,
				   struct slist_node *node);


/* adds 'node' to the end of 'list', returning it.
 * passing NULL in 'list' returns NULL
 * passing NULL in 'node' returns NULL
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct slist_node *slist_node_append(struct slist_list *list,
				     struct slist_node *node);


/* removes 'list' head and returns it
 * passing NULL in 'list' returns NULL
 * passing an empty 'list' returns NULL
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct slist_node *slist_node_pop(struct slist_list *list);


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
struct slist_node *slist_node_find(struct slist_list *list, void *key,
				   int (*cmp)(void *a, void *b));


/* returns the 'index' of 'key' in 'list'
 * returns 0 if 'list' is NULL
 * returns 0 if 'list' is empty
 * returns 0 if 'key' is NULL
 * returns 0 if 'key' is no found
 * returns 0 if 'cmp' is NULL
 *
 * ABOUT indexing: starts at 1, 0 is reserved (see above)
 *
 * ABOUT 'cmp': function needs to return 0 when 'a' and 'b' match
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
size_t slist_find_index_of(struct slist_list *list, void *key,
			   int (*cmp)(void *a, void *b));


/* returns the 'node' matching the 'key' removing it
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 * returns NULL if 'key' is NULL
 * returns NULL if 'key' is no found
 * returns NULL if 'cmp' is NULL
 *
 * ABOUT 'cmp': function needs to return 0 when 'a' and 'b' match
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
struct slist_node *slist_node_remove(struct slist_list *list, void *key,
				     int (*cmp)(void *a, void *b));


/* returns the 'node' stored in 'index' position removing it
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 * returns NULL if 'index' is out of bounds
 *
 * ABOUT 'index' : starts counting at 1
 *
 * passing invalid ['list' or 'index']
 * ------- results in undefined behavior
 */
struct slist_node *slist_node_remove_at(struct slist_list *list,
					const size_t index);


/* executes 'action' in each 'node' contained in 'list'
 * returns without any action performed if 'list' is NULL
 * returns without any action performed if 'list' is empty
 * returns without any action performed if 'action' is NULL
 * passing NULL in 'param' is allowed
 *
 * ABOUT 'action': function takes 3 parameters
 * -------- carry: an argument that is used to 'catch' the result
 * --------------- of 'action' and carry it into next execution
 * --------------- _foreach call of slist_node_foreach carry is NULL
 * --------------- in first execution of 'action'
 * -------- data:  the data contained in the node
 * -------- param: optional. param is passed to all action calls
 *
 * passing invalid ['list' or 'action' or 'param']
 * ------- results in undefined behavior
 * //FIXME: consider setting errno
 * //FIXME: consider removing carry and return value, since a packed param
 * -------- should suffice.
 */
void slist_node_foreach(struct slist_list *list,
			void *(*action)(void *carry, void *data, void *param),
			void *param);


/* returns 'list' empty deleting all 'nodes' contained in 'list'
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 * ABOUT 'list' _status_ : still needs to be freed. see slist_list_delete
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct slist_list *slist_list_delete_all_nodes(struct slist_list *list);


/* returns 'list' reversed
 * example:******************************************************************
 * -------- if your list configuration looks like the following
 * -------- list-> 0, 1, 2, 3, 4, 5, end
 * --------
 * -------- 'list' after execution
 * -------- list-> 5, 4, 3, 2, 1, 0, end
 *
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */


struct slist_list *slist_list_reverse(struct slist_list *list);

/* returns the number of 'nodes' contained in 'list'
 * ABOUT slist_get_size : this function is a stub/dummy function
 * ---------------------- and is inlined, because of that, no safety
 * ---------------------- checks are made
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
size_t slist_get_size(struct slist_list *list);


/* returns 'list' after merging 's_list' into 'list' head.
 * returns NULL if 'list' is NULL
 * returns NULL if 's_list' is NULL
 * returns NULL if 's_list' is empty
 *
 * ABOUT merging: 's_list' becomes empty after merge but still needs to
 * ------- be manually freed. see slist_list_delete for that
 * ------- if 'list' node_dalloc can't free
 * ------- 's_list' allocated memory by it's node_alloc, the lists shouldn't
 * ------- be merged before conversion since this may lead to memory leaks.
 * ------- tip: use slist_node_foreach and just build a 'action' that makes
 * ------- conversion if required.
 * example: *****************************************************************
 * ------- before merge
 * -------  list  -> 4, 5, 6, end
 * ------- s_list -> 1, 2, 3, end
 * ------- ******************************************************************
 * ------- after merge
 * ------- list   ->  1, 2, 3, 4, 5, 6, end
 * ------- s_list -> end
 * **************************************************************************
 *
 * passing invalid ['list' or 's_list']
 * ------- results in undefined behavior
 */
struct slist_list *slist_list_push(struct slist_list *list,
				   struct slist_list *s_list);


/* returns 'list' after merging 's_list' into 'list' tail/end
 * returns NULL if 'list' is NULL
 * returns NULL if 's_list' is NULL
 * returns NULL if 's_list' is empty
 *
 * ABOUT merging: 's_list' becomes empty after merge but still needs to be
 * ------- be manually freed. see slist_list_delete for that
 * ------- if 'list' node_dalloc can't free
 * ------- 's_list' allocated memory by it's node_alloc, the lists shouldn't
 * ------- be merged before conversion since this may lead to memory leaks.
 * ------- tip: use slist_node_foreach and just build a 'action' that makes
 * ------- conversion if required.
 * example: *****************************************************************
 * ------- before merge
 * ------- list   -> 4, 5, 6, end
 * ------- s_list -> 1, 2, 3, end
 * ------- ******************************************************************
 * ------- after merge
 * ------- list   -> 4, 5, 6, 1, 2, 3, end
 * ------- s_list -> end
 * **************************************************************************
 *
 * passing invalid ['list' or 's_list']
 * ------- results in undefined behavior
 */
struct slist_list *slist_list_append(struct slist_list *list,
				     struct slist_list *s_list);


/* returns a newly allocated 'list' with head set to 'key'
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 * returns NULL if 'key' is NULL
 * returns NULL if 'cmp' is NULL
 * returns NULL if 'key' is not found
 * returns NULL if allocation of 'new list' fails
 *
 * ---info makes 'list' empty if 'key' matches @ head
 * ---info the newly allocated list has to be freed. see slist_list_delete
 *
 * ABOUT cmp: function needs to return 0 when 'a' and 'b' match
 * example: *****************************************************************
 * ------- before split
 * ------- list          -> 4, 5, 6, end
 * ------- ******************************************************************
 * ------- after split [with key set to 5]
 * ------- list          -> 4, end
 * ------- returned_list -> 5, 6, end
 * **************************************************************************
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct slist_list *slist_list_split(struct slist_list *list, void *key,
				    int (*cmp)(void *a, void *b));


/* returns a newly allocated 'list' with head set to 'node' stored in index
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 * returns NULL if 'index' is out of bounds
 * returns NULL if allocation of 'new list' fails
 *
 * ---info makes 'list' empty if 'index' is 1/head
 * ---info the newly allocated list has to be freed. see slist_list_delete
 *
 * ABOUT cmp: function needs to return 0 when 'a' and 'b' match
 * example: *****************************************************************
 * ------- before split
 * ------- list          -> 4, 5, 6, end
 * ------- ******************************************************************
 * ------- after split [with index set to 2]
 * ------- list          -> 4, end
 * ------- returned_list -> 5, 6, end
 * **************************************************************************
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct slist_list *slist_list_split_at(struct slist_list *list,
				       const size_t index);


#endif
