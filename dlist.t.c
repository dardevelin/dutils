/*
 * dlist.t.c
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
#include <stdio.h>

#include <assert.h>

#define LOUD
#include "common.h"

int main(int argc, char **argv)
{
	wmsg("testing dlist lib interface\n");

	{
		wmsg("dlist_init");

		struct dlist_list list;
		assert( dlist_init(&list, NULL, NULL) );
		assert( 0 == list.count );
		assert( NULL == list.head );
		assert( NULL == list.tail );
		assert( DLIST_DEF_ALLOC == list.node_alloc );
		assert( DLIST_DEF_DALLOC == list.node_dalloc );

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_new_list");

		struct dlist_list *list = NULL;
		assert( (list = dlist_new_list(NULL, NULL)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( DLIST_DEF_ALLOC == list->node_alloc );
		assert( DLIST_DEF_DALLOC == list->node_dalloc );
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_new_node");

		struct dlist_list *list;
		struct dlist_node *node;
		assert( (list = dlist_new_list(NULL, NULL)) );
		assert( (node = dlist_new_node(list, NULL, NULL)) );
		free(node);
		assert( (node = dlist_new_node(list, int_copy(10), int_dalloc)) );
		assert( 10 == *(int*)node->data );
		assert( !node->next );
		assert( !node->prev );
		assert( int_dalloc == node->data_dalloc );
		assert( (node->data_dalloc(node->data), 1) );
		free(node);
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_print_node");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_list(NULL, NULL);
		assert( (node = dlist_new_node(list, int_copy(10), int_dalloc)) );
		//set print_test_global helper global var to defaul value
		print_test_global = -1;
		dlist_print_node(node, print_test);
		assert( 0 == print_test_global );
		free(node->data);
		free(node);
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_delete_node");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_list(NULL, NULL);
		assert( (node = dlist_new_list(list, int_copy(10), int_dalloc)) );
		dlist_delete_node(list, node);
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_delete_list");

		struct dlist_list *list;
		list = dlist_new_list(NULL, NULL);
		assert( list );
		dlist_delete_list(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_push_node");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(10), int_dalloc);

		//test failures
		assert( NULL == dlist_push_node(NULL, NULL) );
		assert( NULL == dlist_push_node(list, NULL) );
		assert( NULL == dlist_push_node(NULL, node) );
		//test working push
		assert( dlist_push_node(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( list->tail );
		assert( NULL == list->head->prev );
		assert( NULL == list->head->next );
		assert( list->head == list->tail );
		assert( 10 == *(int*)list->head->data );

		node = NULL;
		dlist_delete_node(list, list->head);
		dlist_delete_list(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_append_node");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_node(list, int_copy(10), int_dalloc);

		//test failures
		assert( NULL == dlist_append_node(NULL, NULL) );
		assert( NULL == dlist_append_node(list, NULL) );
		assert( NULL == dlist_append_node(NULL, node) );
		//test working append @ head
		assert( dlist_append_node(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( list->tail );
		assert( NULL == list->head->prev );
		assert( NULL == list->head->next );
		assert( list->head == list->tail );
		assert( 10 == *(int*)list->head->data );
		//create another node to test at end append
		node = dlist_new_node(list, int_copy(11), int_dalloc);
		//test working append at end
		assert( dlist_append_node(list, node) );
		assert( 2 == list->count );
		assert( 11 == *(int*)list->head->next->data );
		assert( list->tail != list->head );
		assert( 11 == *(int*)list->tail->data );

		dlist_delete_node(list, list->head->next);
		dlist_delete_node(list, list->head);
		dlist_delete_list(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_pop_node");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(11), int_dalloc);

		//test failures
		assert( NULL == dlist_pop_node(NULL) );
		assert( NULL == dlist_pop_node(list) );
		//push node for further testing
		dlist_push_node(list, node);
		//test pop
		node = NULL;
		assert( (node == dlist_pop_node(list)) );
		assert( 0 == list->count );
		assert( !list->head );
		assert( !list->tail );

		dlist_delete_node(list, node);
		dlist_delete_list(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_find_node");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(9), int_dalloc);
		key = int_copy(9);

		//test failures
		assert( NULL == dlist_find_node(NULL, NULL, NULL) );
		assert( NULL == dlist_find_node(list, NULL, NULL) );
		assert( NULL == dlist_find_node(NULL, key, NULL) );
		assert( NULL == dlist_find_node(NULL, NULL, cmp_int) );
		assert( NULL == dlist_find_node(list, key, NULL) );
		assert( NULL == dlist_find_node(list, NULL, cmp_int) );
		assert( NULL == dlist_find_node(NULL, key, cmp_int) );
		assert( NULL == dlist_find_node(list, key, cmp_int) );
		//add node to list so we can test it
		dlist_push_node(list, node);
		//test found @ head
		node = NULL;
		assert( (node = dlist_find_node(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );

		//add some nodes to test other scenarios
		node = dlist_new_node(list, int_copy(8), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(7), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(6), int_dalloc);
		dlist_push_node(list, node);
		//after pushes 9 is tail, test found in tail
		node = NULL;
		assert( (node = dlist_find_node(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = dlist_find_node(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == dlist_find_node(list, key, cmp_int) );
		//yes a loop is nice, but lets do this
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		dlist_delete_list(list, node);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_find_index_of");

		struct dlist_list *list;
		struct dlist_list *node;
		void *key;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(10), int_dalloc);
		key = int_copy(10);

		//test failures
		assert( 0 == dlist_find_index_of(NULL, NULL, NULL) );
		assert( 0 == dlist_find_index_of(list, NULL, NULL) );
		assert( 0 == dlist_find_index_of(NULL, key, NULL) );
		assert( 0 == dlist_find_index_of(NULL, NULL, cmp_int) );
		assert( 0 == dlist_find_index_of(list, key, NULL) );
		assert( 0 == dlist_find_index_of(NULL, key, cmp_int) );
		assert( 0 == dlist_find_index_of(list, key, cmp_int) );
		//add node for testing
		dlist_push_node(list, node);
		//test found in head
		assert( 1 == dlist_find_index_of(list, key, cmp_int) );
		//add some more nodes to test other scenarios
		node = dlist_new_node(list, int_copy(9), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(8), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(7), int_dalloc);
		dlist_push_node(list, node);
		//after pushing all nodes, 10 should be index 4
		assert( 4 == dlist_find_index_of(list, key, cmp_int) );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( 2 == dlist_find_index_of(list, key, cmp_int) );
		//edit key to test not found
		*(int*)key = 100;
		assert( 0 == dlist_find_index_of(list, key, cmp_int) );
		//clean
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);
		dlist_delete_list(list);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_remove_node");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == dlist_remove_node(NULL, NULL, NULL) );
		assert( NULL == dlist_remove_node(list, NULL, NULL) );
		assert( NULL == dlist_remove_node(NULL, key, NULL) );
		assert( NULL == dlist_remove_node(NULL, NULL, cmp_int) );
		assert( NULL == dlist_remove_node(list, key, NULL) );
		assert( NULL == dlist_remove_node(NULL, key, cmp_int) );
		assert( NULL == dlist_remove_node(list, key, cmp_int) );
		dlist_push_node(list, node);
		//test found @ head
		node = NULL;
		assert( (node = dlist_remove_node(list, key, cmp_int)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		dlist_push_node(list, node);
		//add some more nodes to test other scenarios
		node = dlist_new_node(list, int_copy(9), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(8), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(7), int_dalloc);
		dlist_push_node(list, node);
		//after all pushes 10 is tail
		node = NULL;
		assert( (node = dlist_remove_node(list, key, cmp_int)) );
		assert( NULL == list->head->next->next->next );
		assert( list->tail == list->head->next->next );
		assert( 10 == *(int*) node->data );
		//append node back for now
		dlist_append_node(list, node);
		//edit key to test common :middle:
		*(int*)key = 8;
		assert( (node = dlist_remove_node(list, key, cmp_int)) );
		assert( 8  == *(int*)node->data );
		assert( 7  == *(int*)list->head->data );
		assert( 9  == *(int*)list->head->next->data );
		assert( 10 == *(int*)list->head->next->next->data );
		assert( list->head->next->prev == list->head );

		dlist_delete_node(node);

		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL = dlist_remove_node(list, key, cmp_int) );
		node = dlist_pop_node(list);
		dlist_delete_node(node);
		node = dlist_pop_node(list);
		dlist_delete_node(node);
		node = dlist_pop_node(list);
		dlist_delete_node(node);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_remove_node_at");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_new_list(NULL, NULL);
		node = dlist_new_node(list, int_copy(10), int_dalloc);
		//test failures
		//no list and lower bounds
		assert( NULL == dlist_remove_node_at(NULL, 0) );
		//no list higher bounds
		assert( NULL == dlist_remove_node_at(NULL, 100) );
		//no list correct bounds
		assert( NULL == dlist_remove_node_at(NULL, 1) );
		//empty list lower bounds
		assert( NULL == dlist_remove_node_at(list, 0) );
		//empty list higher bounds
		assert( NULL == dlist_remove_node_at(list, 100) );
		//empty list correct bounds
		assert( NULL == dlist_remove_node_at(list, 1) );
		dlist_push_node(list, node);
		//non empty list lower bounds
		assert( NULL == dlist_remove_node_at(list, 0) );
		//non empty list higher bounds
		assert( NULL == dlist_remove_node_at(list, 100) );
		//test found @ index 1
		node = NULL;
		assert( (node = list_remove_node_at(list, 1)) );
		assert( 0 == dlist->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 10 == *(int*)node->data );
		//push back for further testing
		dlist_push_node(list, node);
		//add some more nodes to test other scenarios
		node = dlist_new_node(list, int_copy(9), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(8), int_dalloc);
		dlist_push_node(list, node);
		node = dlist_new_node(list, int_copy(7), int_dalloc);
		dlist_push_node(list, node);
		//after all pushes 10 is tail and is the 4 index
		//test remove at tail/last index
		node = NULL;
		assert( (node = dlist_remove_node_at(list, 4)) );
		assert( 3 == list->count );
		assert( NULL == list->head->next->next->next->next );
		assert( NULL == list->tail->next );
		assert( 9 == *(int*)list->tail->data );
		assert( list->tail == list->head->next->next->next );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		dlist_append_node(list, node);
		//test common :middle:
		node = NULL;
		assert( (node = dlist_remove_node_at(list, 2)) );
		assert( 3 == list->count );
		assert( 8 == *(int*)node->data );
		//see if it was really removed
		assert( node != list->head->next );
		assert( list->head == list->head->next->prev );
		dlist_delete_node(list, node);
		//there are 3 nodes
		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		node = dlist_pop_node(list);
		dlist_delete_node(list, node);

		dlist_delete_list(list);
		
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_foreach_node");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_delete_all_nodes");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_reverse_list");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_get_size");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_psuh_list");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_append_list");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_split_list");
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_split_list_at");
		wmsg("[OK]\n");
	}

	return 0;
}
