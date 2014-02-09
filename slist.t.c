/*
 * slist.t.c
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
#include <stdio.h>

#include <assert.h>

#define LOUD
/****************************************************************************
 * configure messages
 ****************************************************************************/
#ifdef LOUD
#define wmsg(...) fprintf(stderr,__VA_ARGS__);
#else
#define wmsg(...)
#endif

/****************************************************************************
 * helper functions used by the tests
 ****************************************************************************/

static void *int_copy(int n)
{
	int *ptr = malloc( sizeof( int ) );
	if (!ptr) {
		wmsg("%s malloc failed\n", __func__);
		return NULL;
	}

	*ptr = n;
	return ptr;
}

static void int_dalloc(void *data)
{
	free(data);
	data = NULL;
}

static void *action_test(void *carry, void *data, void *param)
{
	//allow silent testing
	if (param) {
		wmsg("%d\n", *(int*)data);
	}
	return NULL;
}

static void *action_reverse_print_test(void *carry, void *data, void *param)
{
	static int max;
	//it's our first time, set the data
	if (!carry) {
		max = 1000;
	}

	//allow test
	if (param) {
		wmsg("\t:(max=%4.d ? data=%4.d) -> %c\n",
		     max, *(int*)data, (max == *(int*)data ? 'T' : 'F') );
	}

	assert( max == *(int*)data );

	--max;
	return data;
}

static int print_test_global = -1;
static void print_test(void *data)
{
	//clean the global first
	print_test_global = -1;
	//save the test result in the global
	print_test_global = !((*(int*)data) == 10 );
}

static int cmp_int(void *a, void *b)
{
	if (!a || !b)
		return -1;

	return !( *(int*)a  == *(int*)b );
}

int main(int argc, char **argv)
{

/****************************************************************************
 * test helper functions first
 ****************************************************************************/

	{
		void *data = NULL;
		void *qata = NULL;
		wmsg("testing defaul functions\n");

		wmsg("int_copy");
		assert( !data && !qata );
		data = int_copy(10);
		qata = int_copy(10);
		assert( data && 10 == *(int*)data );
		assert( qata && 10 == *(int*)qata );


		//clean up
		free(data);
		free(qata);

		wmsg("[OK]\n");
	}

	{
		void *data = NULL;

		data = int_copy(10);

		wmsg("print_test");
		assert( -1 == print_test_global );
		assert( (print_test(data), !print_test_global) );

		free(data);
		wmsg("[OK]\n");
	}

	{
		void *data = NULL;
		void *qata = NULL;

		data = int_copy(10);
		qata = int_copy(10);

		wmsg("cmp_int");
		assert( !cmp_int(data, qata) );

		//clean up
		free(data);
		free(qata);

		wmsg("[OK]\n");
	}

	{
		void *data = NULL;

		wmsg("int_dalloc");

		data = int_copy(10);
		int_dalloc(data);

		wmsg("[OK]\n");
	}

	{
		void *data = NULL;
		data = int_copy(10);

		wmsg("action_test");
		assert( !action_test(NULL, data, NULL) );

		free(data);

		wmsg("[OK]\n");
	}

	{
		void *data = NULL;
		data = int_copy(1000);

		wmsg("action_reverse_print_test");
		assert( 1000 == *(int*)action_reverse_print_test(NULL, data, NULL) );

		free(data);

		wmsg("[OK]\n");

	}

	{
		wmsg("testing library interface\n");
		struct slist_list list;
		wmsg("slist_init");
		assert( slist_init(&list, NULL, NULL) );
		assert( 0 == list.count );
		assert( SLIST_DEF_ALLOC == list.node_alloc );
		assert( SLIST_DEF_DALLOC == list.node_dalloc );
		assert( NULL == list.head );
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_new_list");
		struct slist_list *list;
		assert( (list = slist_new_list(NULL, NULL)) );
		assert( 0 == list->count );
		assert( SLIST_DEF_ALLOC == list->node_alloc );
		assert( SLIST_DEF_DALLOC == list->node_dalloc );
		assert( NULL == list->head );
		//don't leak
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_new_node");
		struct slist_list *list;
		struct slist_node *node;
		assert( (list = slist_new_list(NULL, NULL)) );
		//assume slist_new_list works
		assert( (node = slist_new_node(list, NULL, NULL)) );
		//free for next test
		free(node);
		assert( (node = slist_new_node(list, int_copy(10), int_dalloc)) );
		assert( 10 == *(int*)node->data );
		assert( int_dalloc == node->data_dalloc );
		assert( !node->next );
		assert( (node->data_dalloc(node->data), 1) );
		free(node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_print_node");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node works
		assert( (node = slist_new_node(list, int_copy(10), int_dalloc)) );
		//set print_test_global to default value
		print_test_global = -1;
		slist_print_node(node, print_test);
		assert( 0 == print_test_global );
		slist_print_node(node, print_test);
		//clean up
		free(node->data);
		free(node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_delete_node");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		assert( (node = slist_new_node(list, int_copy(10), int_dalloc)) );
		//assume slist_new_node works
		slist_delete_node(list, node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_delete_list");
		struct slist_list *list;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		assert( list );
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_push_node");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_push_node(NULL, NULL) );
		assert( NULL == slist_push_node(NULL, node) );
		assert( NULL == slist_push_node(list, NULL) );
		//test working push
		assert(  slist_push_node(list, node) );
		//check if push did everything
		assert( 1 == list->count );
		assert( list->head );
		assert( 10 == *(int*) list->head->data );
		//everything seems fine, now lets clean up
		node = NULL;
		slist_delete_node(list, list->head);
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_append_node");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_append_node(NULL, NULL) );
		assert( NULL == slist_append_node(NULL, node) );
		assert( NULL == slist_append_node(list, NULL) );
		//test working append at head
		assert( slist_append_node(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( 10 == *(int*)list->head->data );
		//create another node to test at end append
		node = slist_new_node(list, int_copy(11), int_dalloc);
		//test working append at end
		assert( slist_append_node(list, node) );
		assert( 2 == list->count );
		assert( 11 == *(int*)list->head->next->data );
		//clean up
		//assume  slist_delete_node + slist_delete_list works
		slist_delete_node(list, list->head->next);
		slist_delete_node(list, list->head);
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_pop_node");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_pop_node(NULL) );
		assert( NULL == slist_pop_node(NULL) );
		//push node for further testings, assume slist_push_node works
		slist_push_node(list, node);
		node = NULL;
		assert( (node = slist_pop_node(list)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		//clean up
		slist_delete_node(list, node);
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_find_node");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//asume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == slist_find_node(NULL, NULL, NULL) );
		assert( NULL == slist_find_node(list, NULL, NULL) );
		assert( NULL == slist_find_node(NULL, key, NULL) );
		assert( NULL == slist_find_node(NULL, NULL, cmp_int) );
		assert( NULL == slist_find_node(list, key, NULL) );
		assert( NULL == slist_find_node(list, NULL, cmp_int) );
		assert( NULL == slist_find_node(NULL, key, cmp_int) );
		//empty list
		assert( NULL == slist_find_node(list, key, cmp_int) );
		//add node to list so we can test it
		slist_push_node(list, node);
		//test found in head
		node = NULL;
		assert( (node = slist_find_node(list, key, cmp_int)) );
		assert( 10 == *(int*)node->data );

		//add some more nodes to test other scenarios
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list, node);
		//after all pushes, 10 is tail
		//test found in end/tail
		assert( (node = slist_find_node(list, key, cmp_int)) );
		assert( 10 == *(int*)node->data );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = slist_find_node(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == slist_find_node(list, key, cmp_int) );
		//clean up
		//there are 4 nodes.
		//(yes i know about loops i just prefer test like this)
		node = slist_pop_node(list);
		slist_delete_node(list,node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);
		//delete our list
		slist_delete_list(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_find_index_of");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( 0 == slist_find_index_of(NULL, NULL, NULL) );
		assert( 0 == slist_find_index_of(list, NULL, NULL) );
		assert( 0 == slist_find_index_of(NULL, key, NULL) );
		assert( 0 == slist_find_index_of(NULL, NULL, cmp_int) );
		assert( 0 == slist_find_index_of(list, key, NULL) );
		assert( 0 == slist_find_index_of(NULL, key, cmp_int) );
		//empty list
		assert( 0 == slist_find_index_of(list, key, cmp_int) );
		//add node to list so we can test it
		slist_push_node(list, node);
		//test found in head
		assert( 1 == slist_find_index_of(list, key, cmp_int) );
		//add some more nodes to test other scenarios
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list, node);
		//after all pushes, 10 is tail so should be index 4
		//test found in tail
		assert( 4 == slist_find_index_of(list, key, cmp_int) );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( 2 == slist_find_index_of(list, key, cmp_int) );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( 0 == slist_find_index_of(list, key, cmp_int) );
		//clean uo
		//there are 4 nodes
		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);
		//delete our list
		slist_delete_list(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_remove_node");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == slist_remove_node(NULL, NULL, NULL) );
		assert( NULL == slist_remove_node(list, NULL, NULL) );
		assert( NULL == slist_remove_node(NULL, key, NULL) );
		assert( NULL == slist_remove_node(NULL, NULL, cmp_int) );
		assert( NULL == slist_remove_node(list, key, NULL) );
		assert( NULL == slist_remove_node(NULL, key, cmp_int) );
		//empty list
		assert( NULL == slist_remove_node(list, key, cmp_int) );
		//add node to list so we can test it
		slist_push_node(list, node);
		//test found in head
		node = NULL;
		assert( (node = slist_remove_node(list, key, cmp_int)) );
		assert( NULL == list->head );
		assert( 0 == list->count );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		slist_push_node(list, node);
		//add some more nodes to test other scenarios
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list, node);
		//after all pushes
		//test found and remove in end/tail
		assert( (node = slist_remove_node(list, key, cmp_int)) );
		assert( NULL == list->head->next->next->next );
		assert( 3 == list->count );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		slist_append_node(list, node);
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = slist_remove_node(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//delete the node
		slist_delete_node(list, node);
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == slist_remove_node(list, key, cmp_int) );
		//clean up
		//there are 3 nodes
		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);
		//delete our list
		slist_delete_list(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_remove_node_at");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_new_list works
		list = slist_new_list(NULL, NULL);
		//assume slist_new_node + int_copy works
		node = slist_new_node(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_remove_node_at(NULL, 1) );
		//test empty
		assert( NULL == slist_remove_node_at(list, 1) );
		//add the node for further testing
		//assume slist_push_node works
		slist_push_node(list, node);
		//test lower bounds
		assert( NULL == slist_remove_node_at(list, 0) );
		//test higher bounds
		assert( NULL == slist_remove_node_at(list, 100) );
		//test found at head
		node = NULL;
		assert( (node = slist_remove_node_at(list, 1)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		slist_push_node(list, node);
		//add some more nodes to test other scenarios
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list, node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list, node);
		//after all pushes ::there are 4 nodes::
		//test found and remove in end/tail
		assert( (node = slist_remove_node_at(list, 4)) );
		assert( 3 == list->count );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		slist_append_node(list, node);
		//test common :middle: found
		node = NULL;
		assert( (node = slist_remove_node_at(list, 2)) );
		assert( 3 == list->count );
		assert( 8 == *(int*)node->data );
		//see if it was really removed
		assert( node != list->head->next );
		//delete our node
		slist_delete_node(list, node);
		//clean up
		//there are 3 nodes
		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list, node);
		//delete our list
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_foreach_node");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_new_list(NULL, NULL);
		//create some nodes to test
		node = slist_new_node(list, int_copy(10), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list,node);
		
		slist_foreach_node(list, action_test, NULL);
		//clean up
		node = slist_pop_node(list);
		slist_delete_node(list, node);

		node = slist_pop_node(list);
		slist_delete_node(list,node);

		node = slist_pop_node(list);
		slist_delete_node(list,node);
		
		node = slist_pop_node(list);
		slist_delete_node(list,node);
		
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_delete_all_nodes");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_new_list(NULL, NULL);
		//test failures
		assert( NULL == slist_delete_all_nodes(NULL) );
		//test empty
		assert( NULL == slist_delete_all_nodes(list) );
		//create some nodes for further testing
		node = slist_new_node(list, int_copy(10), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(9), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(8), int_dalloc);
		slist_push_node(list,node);
		node = slist_new_node(list, int_copy(7), int_dalloc);
		slist_push_node(list,node);
		assert( NULL != slist_delete_all_nodes(list) );
		assert( NULL == list->head );
		assert( 0 == list->count );
		//clean up
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_reverse_list");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_new_list(NULL, NULL);
		//test failures
		assert( NULL == slist_reverse_list(NULL) );
		//test empty
		assert( NULL == slist_reverse_list(list) );

		for(int i=1000; 0 < i; --i) 
		{
			node = slist_new_node(list, int_copy(i), int_dalloc);
			slist_push_node(list,node);
		}

		//execute
		assert( NULL != slist_reverse_list(list) );
		//do the actual test , set to silent
		slist_foreach_node(list, action_reverse_print_test, NULL);
		//clean up
		slist_delete_all_nodes(list);
		slist_delete_list(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_get_size");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_new_list(NULL, NULL);
		node = slist_new_node(list, int_copy(10), int_dalloc);
		slist_push_node(list, node);
		assert( 1 == list->count && 1 == slist_get_size(list) );
		//clean up
		slist_delete_node(list, slist_pop_node(list));
		slist_delete_list(list);
		wmsg("[OK]\n");
	}


	return 0;
}
