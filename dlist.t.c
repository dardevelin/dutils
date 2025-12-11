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

// Helper functions for tests
void *double_int(void *data) {
    int *val = (int *)data;
    int *new_val = int_copy(*val * 2);
    return new_val;
}

bool is_even(void *data) {
    int *val = (int *)data;
    return (*val % 2) == 0;
}

void *add_int(void *acc, void *data) {
    int *a = (int *)acc;
    int *d = (int *)data;
    *a += *d;
    return acc;
}

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
		wmsg("dlist_list_new");

		struct dlist_list *list = NULL;
		assert( (list = dlist_list_new(NULL, NULL)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( DLIST_DEF_ALLOC == list->node_alloc );
		assert( DLIST_DEF_DALLOC == list->node_dalloc );
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_new");

		struct dlist_list *list;
		struct dlist_node *node;
		assert( (list = dlist_list_new(NULL, NULL)) );
		assert( (node = dlist_node_new(list, NULL, NULL)) );
		free(node);
		assert( (node = dlist_node_new(list, int_copy(10), int_dalloc)) );
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
		wmsg("dlist_node_print");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		assert( (node = dlist_node_new(list, int_copy(10), int_dalloc)) );
		//set print_test_global helper global var to defaul value
		print_test_global = -1;
		dlist_node_print(node, print_test);
		assert( 0 == print_test_global );
		free(node->data);
		free(node);
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_delete");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		assert( (node = dlist_node_new(list, int_copy(10), int_dalloc)) );
		dlist_node_delete(list, node);
		free(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_delete");

		struct dlist_list *list;
		list = dlist_list_new(NULL, NULL);
		assert( list );
		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_push");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);

		//test failures
		assert( NULL == dlist_node_push(NULL, NULL) );
		assert( NULL == dlist_node_push(list, NULL) );
		assert( NULL == dlist_node_push(NULL, node) );
		//test working push
		assert( dlist_node_push(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( list->tail );
		assert( NULL == list->head->prev );
		assert( NULL == list->head->next );
		assert( list->head == list->tail );
		assert( 10 == *(int*)list->head->data );

		node = NULL;
		dlist_node_delete(list, list->head);
		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_append");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);

		//test failures
		assert( NULL == dlist_node_append(NULL, NULL) );
		assert( NULL == dlist_node_append(list, NULL) );
		assert( NULL == dlist_node_append(NULL, node) );
		//test working append @ head
		assert( dlist_node_append(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( list->tail );
		assert( NULL == list->head->prev );
		assert( NULL == list->head->next );
		assert( list->head == list->tail );
		assert( 10 == *(int*)list->head->data );
		//create another node to test at end append
		node = dlist_node_new(list, int_copy(11), int_dalloc);
		//test working append at end
		assert( dlist_node_append(list, node) );
		assert( 2 == list->count );
		assert( 11 == *(int*)list->head->next->data );
		assert( list->tail != list->head );
		assert( 11 == *(int*)list->tail->data );

		dlist_node_delete(list, list->head->next);
		dlist_node_delete(list, list->head);
		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_pop");

		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(11), int_dalloc);

		//test failures
		assert( NULL == dlist_node_pop(NULL) );
		assert( NULL == dlist_node_pop(list) );
		//push node for further testing
		dlist_node_push(list, node);
		//test pop
		node = NULL;
		assert( (node = dlist_node_pop(list)) );
		assert( 0 == list->count );
		assert( !list->head );
		assert( !list->tail );

		dlist_node_delete(list, node);
		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_find");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		key = int_copy(9);

		//test failures
		assert( NULL == dlist_node_find(NULL, NULL, NULL) );
		assert( NULL == dlist_node_find(list, NULL, NULL) );
		assert( NULL == dlist_node_find(NULL, key, NULL) );
		assert( NULL == dlist_node_find(NULL, NULL, cmp_int) );
		assert( NULL == dlist_node_find(list, key, NULL) );
		assert( NULL == dlist_node_find(list, NULL, cmp_int) );
		assert( NULL == dlist_node_find(NULL, key, cmp_int) );
		assert( NULL == dlist_node_find(list, key, cmp_int) );
		//add node to list so we can test it
		dlist_node_push(list, node);
		//test found @ head
		node = NULL;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );

		//add some nodes to test other scenarios
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		//after pushes 9 is tail, test found in tail
		node = NULL;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == dlist_node_find(list, key, cmp_int) );
		//yes a loop is nice, but lets do this
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		dlist_list_delete(list);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_fast_find");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		key = int_copy(9);

		//test failures
		assert( NULL == dlist_node_find(NULL, NULL, NULL) );
		assert( NULL == dlist_node_find(list, NULL, NULL) );
		assert( NULL == dlist_node_find(NULL, key, NULL) );
		assert( NULL == dlist_node_find(NULL, NULL, cmp_int) );
		assert( NULL == dlist_node_find(list, key, NULL) );
		assert( NULL == dlist_node_find(list, NULL, cmp_int) );
		assert( NULL == dlist_node_find(NULL, key, cmp_int) );
		assert( NULL == dlist_node_find(list, key, cmp_int) );
		//add node to list so we can test it
		dlist_node_push(list, node);
		//test found @ head
		node = NULL;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );

		//add some nodes to test other scenarios
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		//after pushes 9 is tail, test found in tail
		node = NULL;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 9 == *(int*)node->data );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = dlist_node_find(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == dlist_node_find(list, key, cmp_int) );
		//yes a loop is nice, but lets do this
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		dlist_list_delete(list);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_find_index_of");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);
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
		dlist_node_push(list, node);
		//test found in head
		assert( 1 == dlist_find_index_of(list, key, cmp_int) );
		//add some more nodes to test other scenarios
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);
		//after pushing all nodes, 10 should be index 4
		assert( 4 == dlist_find_index_of(list, key, cmp_int) );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( 2 == dlist_find_index_of(list, key, cmp_int) );
		//edit key to test not found
		*(int*)key = 100;
		assert( 0 == dlist_find_index_of(list, key, cmp_int) );
		//clean
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		dlist_list_delete(list);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_remove");

		struct dlist_list *list;
		struct dlist_node *node;
		void *key;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == dlist_node_remove(NULL, NULL, NULL) );
		assert( NULL == dlist_node_remove(list, NULL, NULL) );
		assert( NULL == dlist_node_remove(NULL, key, NULL) );
		assert( NULL == dlist_node_remove(NULL, NULL, cmp_int) );
		assert( NULL == dlist_node_remove(list, key, NULL) );
		assert( NULL == dlist_node_remove(NULL, key, cmp_int) );
		assert( NULL == dlist_node_remove(list, key, cmp_int) );
		dlist_node_push(list, node);
		//test found @ head
		node = NULL;
		assert( (node = dlist_node_remove(list, key, cmp_int)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		dlist_node_push(list, node);
		//add some more nodes to test other scenarios
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);
		//after all pushes 10 is tail
		node = NULL;
		assert( (node = dlist_node_remove(list, key, cmp_int)) );
		assert( NULL == list->head->next->next->next );
		assert( list->tail == list->head->next->next );
		assert( 10 == *(int*) node->data );
		//append node back for now
		dlist_node_append(list, node);
		//edit key to test common :middle:
		*(int*)key = 8;
		assert( (node = dlist_node_remove(list, key, cmp_int)) );
		assert( 8  == *(int*)node->data );
		assert( 7  == *(int*)list->head->data );
		assert( 9  == *(int*)list->head->next->data );
		assert( 10 == *(int*)list->head->next->next->data );
		assert( list->head->next->prev == list->head );

		dlist_node_delete(list, node);

		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == dlist_node_remove(list, key, cmp_int) );
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);
		int_dalloc(key);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_remove_at");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);
		//test failures
		//no list and lower bounds
		assert( NULL == dlist_node_remove_at(NULL, 0) );
		//no list higher bounds
		assert( NULL == dlist_node_remove_at(NULL, 100) );
		//no list correct bounds
		assert( NULL == dlist_node_remove_at(NULL, 1) );
		//empty list lower bounds
		assert( NULL == dlist_node_remove_at(list, 0) );
		//empty list higher bounds
		assert( NULL == dlist_node_remove_at(list, 100) );
		//empty list correct bounds
		assert( NULL == dlist_node_remove_at(list, 1) );
		dlist_node_push(list, node);
		//non empty list lower bounds
		assert( NULL == dlist_node_remove_at(list, 0) );
		//non empty list higher bounds
		assert( NULL == dlist_node_remove_at(list, 100) );
		//test found @ index 1
		node = NULL;
		assert( (node = dlist_node_remove_at(list, 1)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 10 == *(int*)node->data );
		//push back for further testing
		dlist_node_push(list, node);
		//add some more nodes to test other scenarios
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);
		//after all pushes 10 is tail and is the 4 index
		//test remove at tail/last index
		node = NULL;
		assert( (node = dlist_node_remove_at(list, 4)) );
		assert( 3 == list->count );
		assert( NULL == list->head->next->next->next );
		assert( NULL == list->tail->next );
		assert( 9 == *(int*)list->tail->data );
		assert( list->tail == list->head->next->next );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		dlist_node_append(list, node);
		//test common :middle:
		node = NULL;
		assert( (node = dlist_node_remove_at(list, 2)) );
		assert( 3 == list->count );
		assert( 8 == *(int*)node->data );
		//see if it was really removed
		assert( node != list->head->next );
		assert( list->head == list->head->next->prev );
		dlist_node_delete(list, node);
		//there are 3 nodes
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_node_foreach");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		//create some nodes to test
		node = dlist_node_new(list, int_copy(10), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);

		dlist_node_foreach(list, action_test, NULL);
		//clean up
		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		node = dlist_node_pop(list);
		dlist_node_delete(list, node);

		dlist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_delete_all_nodes");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		//test failures
		assert( NULL == dlist_list_delete_all_nodes(NULL) );
		//test empty
		assert( NULL == dlist_list_delete_all_nodes(list) );
		//create some nodes for further testing
		node = dlist_node_new(list, int_copy(10), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(9), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(8), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(7), int_dalloc);
		dlist_node_push(list, node);

		assert( NULL != dlist_list_delete_all_nodes(list) );
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 0 == list->count );

		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_reverse");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		//test failures
		assert( NULL == dlist_list_reverse(NULL) );
		//test empty
		assert( NULL == dlist_list_reverse(list) );

		for(int i=1000; 0 < i; --i)
		{
			node = dlist_node_new(list, int_copy(i), int_dalloc);
			dlist_node_push(list, node);
		}

		//execute
		assert( NULL != dlist_list_reverse(list) );
		//do the actual test, set to silent
		dlist_node_foreach(list, action_reverse_print_test, NULL);
		//clean up
		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_get_size");
		struct dlist_list *list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(10), int_dalloc);
		dlist_node_push(list, node);
		assert( 1 == list->count && 1 == dlist_get_size(list) );
		dlist_node_delete(list, dlist_node_pop(list));
		dlist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_push");
		struct dlist_list *list;
		struct dlist_list *s_list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		s_list = dlist_list_new(NULL, NULL);
		//test failures
		assert( NULL == dlist_list_push(NULL, NULL) );
		assert( NULL == dlist_list_push(list, NULL) );
		assert( NULL == dlist_list_push(NULL, s_list) );
		//test empty
		assert( NULL == dlist_list_push(list, s_list) );
		//build the lists to test 'proper behavior'
		//list
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(5), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(4), int_dalloc);
		dlist_node_push(list, node);
		//s_list
		node = dlist_node_new(s_list, int_copy(3), int_dalloc);
		dlist_node_push(s_list, node);
		node = dlist_node_new(s_list, int_copy(2), int_dalloc);
		dlist_node_push(s_list, node);
		node = dlist_node_new(s_list, int_copy(1), int_dalloc);
		dlist_node_push(s_list, node);
		//test
		assert( NULL != dlist_list_push(list, s_list) );
		//confirm that s_list is now empty
		assert( NULL == s_list->head );
		assert( 0 == s_list->count );
		//confirm that the merge was properly done
		//check for new list head (should be 1)
		assert( 1 == *(int*) list->head->data );
		//check for new list tail (should be 6)
		assert( 6 == *(int*) list->tail->data );
		//check if merge points are properly linked
		assert( 4 == *(int*) list->head->next->next->next->data );
		assert( 3 == *(int*) list->tail->prev->prev->prev->data );
		//check new list count
		assert( 6 == list->count );

		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);
		dlist_list_delete(s_list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_append");
		struct dlist_list *list;
		struct dlist_list *s_list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		s_list = dlist_list_new(NULL, NULL);
		//test failures
		assert( NULL == dlist_list_append(NULL, NULL) );
		assert( NULL == dlist_list_append(list, NULL) );
		assert( NULL == dlist_list_append(NULL, s_list) );
		//test empty
		assert( NULL == dlist_list_append(list, s_list) );
		//build the list to test 'proper behavior'
		//list
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(5), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(4), int_dalloc);
		dlist_node_push(list, node);
		//s_list
		node = dlist_node_new(s_list, int_copy(3), int_dalloc);
		dlist_node_push(s_list, node);
		node = dlist_node_new(s_list, int_copy(2), int_dalloc);
		dlist_node_push(s_list, node);
		node = dlist_node_new(s_list, int_copy(1), int_dalloc);
		dlist_node_push(s_list, node);
		//test
		assert( NULL != dlist_list_append(list, s_list) );
		//confirm that s_list is now empty
		assert( NULL == s_list->head );
		assert( NULL == s_list->tail );
		assert( 0 == s_list->count );
		//confirm that the merge was properly done
		//check for list head (should be the same)
		assert( 4 == *(int*)list->head->data );
		assert( 5 == *(int*)list->head->next->data );
		assert( 6 == *(int*)list->head->next->next->data );
		assert( 1 == *(int*)list->head->next->next->next->data );
		assert( 2 == *(int*)list->head->next->next->next->next->data );
		assert( 3 == *(int*)list->head->next->next->next->next->next->data );
		//check for tail (should be the old tail of s_list)
		assert( 3 == *(int*)list->tail->data );
		//check if merge points are properly linked
		assert( 1 == *(int*) list->head->next->next->next->data );
		assert( 6 == *(int*) list->tail->prev->prev->prev->data );
		//check new list size
		assert( 6 == list->count );

		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);
		dlist_list_delete(s_list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_split");
		struct dlist_list *list;
		struct dlist_list *n_list;
		struct dlist_node *node;
		void *key;
		list = dlist_list_new(NULL, NULL);
		key = int_copy(6);
		//test failures
		assert( NULL == dlist_list_split(NULL, NULL, NULL) );
		assert( NULL == dlist_list_split(list, NULL, NULL) );
		assert( NULL == dlist_list_split(NULL, key, NULL) );
		assert( NULL == dlist_list_split(NULL, NULL, cmp_int) );
		assert( NULL == dlist_list_split(list, key, NULL) );
		assert( NULL == dlist_list_split(NULL, key, cmp_int) );
		//test empty
		assert( NULL == dlist_list_split(list, key, cmp_int) );
		//create node for testing
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		//test found @ head and split;
		assert( (n_list = dlist_list_split(list, key, cmp_int)) );
		//check the list is indeed empty
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 0 == list->count );
		//check n_list has all data properly set
		assert( NULL != n_list->head );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );

		dlist_list_delete_all_nodes(n_list);
		dlist_list_delete(n_list);
		n_list = NULL;
		node = NULL;

		//build some nodes for further testing
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(5), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(4), int_dalloc);
		dlist_node_push(list, node);
		//test end/tail split
		assert( (n_list = dlist_list_split(list, key, cmp_int)) );
		//check list was properly split
		assert( NULL == list->head->next->next );
		assert( 2 == list->count );
		//check n_list has all data properly set
		assert( NULL != n_list->head );
		assert( NULL != n_list->tail );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );
		//save the node in 'list' for further testing
		node = dlist_node_pop(n_list);
		dlist_node_append(list, node);

		dlist_list_delete(n_list);
		n_list = NULL;
		//edit key to test common :middle: split
		*(int*)key = 5;
		//the test
		assert( (n_list = dlist_list_split(list, key, cmp_int)) );
		//check list has everything properly set
		assert( 1 == list->count );
		assert( 4 == *(int*)list->head->data );
		//check n_list has everything properly set
		assert( NULL != n_list->head );
		assert( 2 == n_list->count );
		assert( 5 == *(int*)n_list->head->data );
		assert( 6 == *(int*)n_list->head->next->data );
		//check if heads and tails are properly terminated
		assert( NULL == list->head->prev );
		assert( NULL == list->tail->next );
		assert( NULL == n_list->head->prev );
		assert( NULL == n_list->tail->next );
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_list_split_at");
		struct dlist_list *list;
		struct dlist_list *n_list;
		struct dlist_node *node;
		list = dlist_list_new(NULL, NULL);
		node = dlist_node_new(list, int_copy(6), int_dalloc);
		//test failures
		//no list and lower bounds
		assert( NULL == dlist_list_split_at(NULL, 0) );
		//no list and higher bounds
		assert( NULL == dlist_list_split_at(NULL, 100) );
		//no list correct bounds
		assert( NULL == dlist_list_split_at(NULL, 1) );
		//empty list lower bounds
		assert( NULL == dlist_list_split_at(list, 0) );
		//empty list higher bounds
		assert( NULL == dlist_list_split_at(list, 100) );
		//empty list correct bounds
		assert( NULL == dlist_list_split_at(list, 1) );
		dlist_node_push(list, node);
		//non empty list lower bounds
		assert( NULL == dlist_list_split_at(list, 0) );
		//non empty list higher bounds
		assert( NULL == dlist_list_split_at(list, 100) );
		//test found @ index 1
		node = NULL;
		assert( (n_list = dlist_list_split_at(list, 1)) );
		//check list is indeed empty
		assert( NULL == list->head );
		assert( NULL == list->tail );
		assert( 0 == list->count );
		//check n_list has all data properly set
		assert( NULL != n_list->head );
		assert( NULL != n_list->tail );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );
		//move all data back to list and test next scenarios
		list->head = n_list->head;
		list->tail = n_list->tail;
		list->count = n_list->count;

		dlist_list_delete(n_list);
		n_list = NULL;

		//create some nodes for further testing
		node = dlist_node_new(list, int_copy(5), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(4), int_dalloc);
		dlist_node_push(list, node);
		//test index at end/tail
		assert( (n_list = dlist_list_split_at(list, 3)) );
		//check list has everything properly set
		assert( NULL != list->head );
		assert( NULL != list->tail );
		assert( NULL == list->head->prev );
		assert( NULL == list->tail->next );
		assert( 2 == list->count );
		assert( 5 == *(int*) list->head->next->data );
		//check that n_list has everything properly set
		assert( NULL != n_list->head );
		assert( NULL != n_list->tail );
		assert( NULL == n_list->head->prev );
		assert( NULL == n_list->tail->next );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );

		dlist_list_delete_all_nodes(list);
		dlist_list_delete_all_nodes(n_list);
		dlist_list_delete(list);
		dlist_list_delete(n_list);
		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_map");

		struct dlist_list *list;
		struct dlist_list *mapped;
		struct dlist_node *node;

		list = dlist_list_new(NULL, NULL);

		// Test failures
		assert(NULL == dlist_map(NULL, NULL, NULL));
		assert(NULL == dlist_map(list, NULL, NULL));

		// Test empty list
		assert(NULL == dlist_map(list, double_int, int_dalloc));

		// Create list with 1,2,3
		node = dlist_node_new(list, int_copy(1), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(2), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(3), int_dalloc);
		dlist_node_push(list, node);

		// Map
		assert((mapped = dlist_map(list, double_int, int_dalloc)));
		assert(3 == mapped->count);

		// Check values: should be 2,4,6 (since push reverses order? Wait, push adds to head, so original order is 3,2,1, mapped 6,4,2
		// Wait, list has head=3, next=2, next=1
		// Map iterates from head: 3->6, 2->4, 1->2, so mapped head=6, next=4, next=2
		assert(6 == *(int*)mapped->head->data);
		assert(4 == *(int*)mapped->head->next->data);
		assert(2 == *(int*)mapped->tail->data);

		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);
		dlist_list_delete_all_nodes(mapped);
		dlist_list_delete(mapped);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_filter");

		struct dlist_list *list;
		struct dlist_list *filtered;
		struct dlist_node *node;

		list = dlist_list_new(NULL, NULL);

		// Test failures
		assert(NULL == dlist_filter(NULL, NULL));
		assert(NULL == dlist_filter(list, NULL));

		// Test empty list
		assert(NULL == dlist_filter(list, is_even));

		// Create list with 1,2,3,4
		node = dlist_node_new(list, int_copy(1), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(2), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(3), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(4), int_dalloc);
		dlist_node_push(list, node);

		// Filter
		assert((filtered = dlist_filter(list, is_even)));
		assert(2 == filtered->count);  // 4 and 2 (since order: head=4, next=2)

		assert(4 == *(int*)filtered->head->data);
		assert(2 == *(int*)filtered->tail->data);

		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);
		dlist_list_delete_all_nodes(filtered);
		dlist_list_delete(filtered);

		wmsg("[OK]\n");
	}

	{
		wmsg("dlist_fold");

		struct dlist_list *list;
		struct dlist_node *node;
		int sum = 0;

		list = dlist_list_new(NULL, NULL);

		// Test failures
		assert(NULL == dlist_fold(NULL, &sum, NULL));
		assert(&sum == dlist_fold(list, &sum, NULL));  // Empty list returns initial

		// Test empty list
		sum = 0;
		assert(&sum == dlist_fold(list, &sum, add_int));
		assert(0 == sum);

		// Create list with 1,2,3
		node = dlist_node_new(list, int_copy(1), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(2), int_dalloc);
		dlist_node_push(list, node);
		node = dlist_node_new(list, int_copy(3), int_dalloc);
		dlist_node_push(list, node);

		// Fold sum
		sum = 0;
		assert(&sum == dlist_fold(list, &sum, add_int));
		assert(6 == sum);  // 3+2+1

		dlist_list_delete_all_nodes(list);
		dlist_list_delete(list);

		wmsg("[OK]\n");
	}

	return 0;
}
