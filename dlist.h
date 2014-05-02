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

/* returns 'list' initialized with 'node_alloc' and 'node_dalloc'
 * NULL is returned if 'list' is NULL
 * passing NULL to 'node_alloc' sets it to DLIST_DEF_ALLOC
 * passing NULL to 'node_dalloc' sets it to DLIST_DEF_DALLOC.
 *
 * passing invalid ['list' or 'node_alloc' or 'node_dalloc']
 * ------- results in undefined behavior
 */
struct dlist_list *dlist_init(struct dlist_list *list,
			      void *(*node_alloc)(size_t),
			      void (*node_dalloc)(void *));


/* returns a new allocated 'list' initialized with
 * ------- 'node_alloc' and 'node_dalloc'.
 * ------- the returned 'list' has to be freed
 * passing NULL to 'node_alloc' sets it to DLIST_DEF_ALLOC.
 * passing NULL to 'node_dalloc' sets it to DLIST_DEF_DALLOC.
 * NULL is returned if 'node_alloc' fails to allocate memory.
 *
 * ABOUT [ node_alloc, node_dalloc ]:
 * ------- these functions can either be library defaults or
 * ------- user defined. user defined functions need to provide
 * ------- the same interface that DLIST_DEF_ALLOC and DLIST_DEF_DALLOC
 * ------- which are often 'malloc' and 'free' from <stdlib.h>
 *
 * passing invalid ['list' or 'node_alloc' or 'node_dalloc']
 * ------- results in undefined behavior
 */
struct dlist_list *dlist_list_new(void *(*node_alloc)(size_t),
				  void (*node_dalloc)(void *));


/* returns a new allocated 'node' initialized with 'data' and 'dalloc'
 * ------- returned 'node' needs to be freed
 * passing NULL in 'data' sets 'node' 'data' to NULL
 * passing NULL in 'dalloc' sets 'node' 'dalloc' to NULL
 * ------- if 'dalloc' is set, _delete_ functions attempt to delete 'data
 * ------- check each _delete_ function documentation for better information
 *
 * passing invalid ['list' or 'data' or 'dalloc']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_new(struct dlist_list *list,
				  void *data, void (*dalloc)(void *));


/* executes 'print' on 'node' passing 'data' member to it.
 * passing NULL in 'node' returns with no operation executed
 * passing NULL in 'print' returns with no operation executed
 *
 * passing invalid ['node' or 'print']
 * ------- results in undefined behavior
 *
 * FIXME NOTE: consider using macro instead. unsing inline for now
 */
void dlist_node_print(const struct dlist_node *node,
		      void (*print)(void *));


/* deletes 'node'
 * attempts to delete 'data' when 'data_dalloc' is set.
 * passing NULL in 'list' returns with no operation executed
 * passing NULL in 'node' returns with no operation executed
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
void dlist_node_delete(struct dlist_list *list,
		       struct dlist_node *node);


/* deletes 'list'
 * passing NULL in 'list' returns with no operation executed
 *
 * NOTE: deleting the 'list' won't free the 'nodes' contained in it.
 * ------- see dlist_nodes_delete_all for that.
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
void dlist_list_delete(struct dlist_list *list);


/* returns the 'list' head, new head.
 * passing NULL in 'list' returns NULL.
 * passing NULL in 'node' returns NULL.
 *
 * NOTE: pushing in a empty list sets tail.
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_push(struct dlist_list *list,
				   struct dlist_node *node);


/* adds 'node' to the end of 'list', updates tail and returns it.
 * passing NULL in 'list' returns NULL.
 * passing NULL in 'node' returns NULL.
 *
 * passing invalid ['list' or 'node']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_append(struct dlist_list *list,
				     struct dlist_node *node);


/* removes 'list' head and returns it.
 * updates tail if last node.
 * passing NULL in 'list' returns NULL.
 * passing an empty 'list' returns NULL.
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_pop(struct dlist_list *list);


/* returns the 'node' matching 'key'
 * returns NULL if 'key' is not found
 * passing NULL in 'list' returns NULL.
 * passing an empty 'list' returns NULL.
 * passing NULL 'key' returns NULL.
 * passing NULL 'cmp' returns NULL.
 *
 * ABOUT ['cmp']: function needs to return 0 when 'a' and 'b' match
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_find(struct dlist_list *list, void *key,
				   int (*cmp)(void *a, void *b));


/* returns the 'index' of 'key' in 'list'
 * returns 0 if 'list' is NULL.
 * returns 0 if 'list' is empty.
 * returns 0 if 'key' is NULL.
 * returns 0 if 'key' is not found.
 * returns 0 if 'cmp' is NULL
 *
 * ABOUT [index]: starts at 1, 0 is reserved (see above)
 *
 * ABOUT ['cmp']: function needs to return 0 when 'a' and 'b' match
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
size_t dlist_find_index_of(struct dlist_list *list, void *key,
			   int (*cmp)(void *a, void *b));


/* returns the 'node' matching the 'key' removing it.
 * returns NULL if 'list' is NULL.
 * returns NULL if 'list' is empty.
 * returns NULL if 'key' is NUL.
 * returns NULL if 'key' is not found.
 * returns NULL if 'cmp' is NULL
 *
 * ABOUT ['cmp']: function needs to return 0 when 'a' and 'b' match
 *
 * NOTE: updates tail when tail is removed.
 *
 * passing invalid ['list' or 'key' or 'cmp']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_remove(struct dlist_list *list, void *key,
				     int (*cmp)(void *a, void *b));


/* returns the 'node' stored in 'index' position removing it.
 * returns NULL if 'list' is NULL.
 * returns NULL if 'list' is empty.
 * returns NULL if 'index' is out of bounds.
 *
 * ABOUT ['index']: starts counting at 1
 *
 * passing invalid ['list' or 'index']
 * ------- results in undefined behavior
 */
struct dlist_node *dlist_node_remove_at(struct dlist_list *list,
					const size_t index);


/* executes 'action' in each 'node' contained in 'list'
 * returns without any action performed if 'list' is NULL
 * returns without any action performed if 'list' is empty
 * returns without any action performed if 'action' is NULL
 * passing NULL in 'param' is allowed
 *
 * ABOUT ['action']: function takes 3 parameters
 * ------- carry: an argument that is used to 'catch' the result
 * -------------- of 'action' and carry it into next execution
 * -------------- _foreach call of dlist_node_foreach carry is NULL
 * -------------- in the first execution of 'action'
 * ------- data: the data contained in the node
 * ------- param: optional. param is passed to all action calls
 *
 * passing invalid ['list' or 'action' or 'param']
 * ------- results in undefined behavior
 * FIXME: consider setting errno
 * FIXME: consider removing carry and return value, since a packed param
 * ------- should suffice.
 */
void dlist_node_foreach(struct dlist_list *list,
			void *(action)(void *carry, void *data, void *param),
			void *param);


/* returns an empty 'list' after deleting all 'nodes' contained in it.
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 *
 * ABOUT ['list'] _status_ : still needs to be freed if it was allocated with
 * ------- dlist_list_new. see dlist_list_delete documentation for more info.
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct dlist_list *dlist_nodes_delete_all(struct dlist_list *list);


/* returns 'list' reversed
 * example: ******************************************************************
 * -------- if your list configuration was (head perspective)
 * -------- list-> 0, 1, 2, 3, 4, 5, end
 * --------
 * -------- 'list' after being reversed
 * -------- list-> 5, 4, 3, 2, 1, 0, end
 * -------- ******************************************************************
 *
 * returns NULL if 'list' is NULL
 * returns NULL if 'list' is empty
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
struct dlist_list *dlist_list_reverse(struct dlist_list *list);


/* returns the number of 'nodes' contained in 'list'
 * ABOUT dlist_get_size: this function is a stub/dummy function
 * --------------------- and is inlined, because of that, no safety
 * --------------------- checks are made
 *
 * passing invalid ['list']
 * ------- results in undefined behavior
 */
size_t dlist_get_size(struct dlist_list *list);


/* returns 'list' after merging 's_list' into 'list' head
 * returns NULL if 'list' is NULL.
 * returns NULL if 's_list' is NULL.
 * returns NULL if 's_list' is empty.
 *
 * ABOUT [merging]: 's_list' becomes empty after merge, but may need to
 * ------- be manually freed. see dlist_list_delete for that.
 * ------- if 'list' -> 'node_dalloc' can't free
 * ------- 's_list' -> 'node_alloc' allocated memory, the lists shouldn't
 * ------- be merged before conversion, since this may lead to memory leaks.
 * ------- tip: use dlist_node_foreach and just build an 'action' function
 * ------- that makes the conversion if required.
 *
 * example: ******************************************************************
 * -------- before merge
 * -------- list ->   4, 5, 6, end
 * -------- s_list -> 1, 2, 3, end
 * -------- ******************************************************************
 * -------- after merge
 * -------- list ->   1, 2, 3, 4, 5, 6, end
 * -------- s_list -> end
 * -------- ******************************************************************
 *
 * passing invalid ['list' or 's_list']
 * -------- results in undefined behavior
 */
struct dlist_list *dlist_list_push(struct dlist_list *list,
				   struct dlist_list *s_list);


/* returns 'list' after merging 's_list' into 'list' tail/end
 * returns NULL if 'list' is NULL.
 * returns NULL if 's_list' is NULL.
 * returns NULL if 's_list' is empty.
 *
 * ABOUT [merging]: 's_list' becomes empty after merge but still needs to be
 * ------- be manually freed. see dlist_list_delete for that.
 * ------- if 'list' -> 'node_dalloc' can't free
 * ------- 's_list' -> 'node_alloc' allocated memory, the lists shouldn't
 * ------- be merged before conversion, since this may lead to memory leaks.
 * ------- tip: use dlist_forach_node and just build an 'action' function
 * ------- that makes the conversion if required.
 *
 * example: ******************************************************************
 * -------- before merge
 * -------- list   ->  4, 5, 6, end
 * -------- s_list ->  1, 2, 3, end
 * -------- ******************************************************************
 * -------- after merge
 * -------- list   -> 4, 5, 6, 1, 2, 3, end
 * -------- s_list -> end
 * -------- ******************************************************************
 *
 * passing invalid ['list' or 's_list']
 * ------- results in undefined behavior
 */
struct dlist_list *dlist_list_append(struct dlist_list *list,
				     struct dlist_list *s_list);


/* returns a newly allocated 'list' with head set to 'key'
 * -------- sets tail of new list and re-sets tail of old list
 * returns NULL if 'list' is NULL.
 * returns NULL if 'list' is empty.
 * returns NULL if 'key' is NULL.
 * returns NULL if 'cmp' is NULL.
 * returns NULL if 'key' is not found.
 * returns NULL if allocation of 'new list' fails
 *
 * ---info makes 'list' empty if 'key' matches @ head
 * ---info  the newly allocated list has to be freed. see dlist_list_delete
 *
 * ABOUT ['cmp']: function needs to return 0 when 'a' and 'b' match
 * example: ******************************************************************
 * -------- before split
 * -------- list          -> 4, 5, 6, end
 * -------- ******************************************************************
 * -------- after split [with key set to 5]
 * -------- list          -> 4, end
 * -------- returned_list -> 5, 6, end
 * -------- ******************************************************************
 *
 * passing invalid ['list']
 * -------- results in undefined behavior
 */
struct dlist_list *dlist_list_split(struct dlist_list *list, void *key,
				    int (*cmp)(void *a, void *b));


/* returns a newly allocated 'list' with head set to 'node' stored in index
 * -------- sets tail of new list and re-sets tail of old list
 * returns NULL if 'list' is NULL.
 * returns NULL if 'list' is empty.
 * returns NULL if 'index' is out of bounds.
 * returns NULL if allocation of 'new list' fails
 *
 * ---info makes 'list' empty if 'index' is 1/head
 * ---info the newly allocated list has to be freed. see dlist_list_delete
 *
 * ABOUT ['cmp']: function needs to return 0 when 'a' and 'b' match
 *
 * example: ******************************************************************
 * -------- before split
 * -------- list          -> 4, 5, 6, end
 * -------- ******************************************************************
 * -------- after split [with index set to 2]
 * -------- list          -> 4, end
 * -------- returned_list -> 5, 6, end
 * -------- ******************************************************************
 *
 * passing invalid ['list']
 * -------- results in undefined behavior
 */
struct dlist_list *dlist_list_split_at(struct dlist_list *list,
				       const size_t index);

#endif
