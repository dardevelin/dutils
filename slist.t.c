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
#include "common.h"

int main(int argc, char **argv)
{

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
		wmsg("slist_list_new");
		struct slist_list *list;
		assert( (list = slist_list_new(NULL, NULL)) );
		assert( 0 == list->count );
		assert( SLIST_DEF_ALLOC == list->node_alloc );
		assert( SLIST_DEF_DALLOC == list->node_dalloc );
		assert( NULL == list->head );
		//don't leak
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_new");
		struct slist_list *list;
		struct slist_node *node;
		assert( (list = slist_list_new(NULL, NULL)) );
		//assume slist_list_new works
		assert( (node = slist_node_new(list, NULL, NULL)) );
		//free for next test
		free(node);
		assert( (node = slist_node_new(list, int_copy(10), int_dalloc)) );
		assert( 10 == *(int*)node->data );
		assert( int_dalloc == node->data_dalloc );
		assert( !node->next );
		assert( (node->data_dalloc(node->data), 1) );
		free(node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_print");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new works
		assert( (node = slist_node_new(list, int_copy(10), int_dalloc)) );
		//set print_test_global to default value
		print_test_global = -1;
		slist_node_print(node, print_test);
		assert( 0 == print_test_global );
		slist_node_print(node, print_test);
		//clean up
		free(node->data);
		free(node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_delete");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		assert( (node = slist_node_new(list, int_copy(10), int_dalloc)) );
		//assume slist_node_new works
		slist_node_delete(list, node);
		free(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_delete");
		struct slist_list *list;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		assert( list );
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_push");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_node_push(NULL, NULL) );
		assert( NULL == slist_node_push(NULL, node) );
		assert( NULL == slist_node_push(list, NULL) );
		//test working push
		assert(  slist_node_push(list, node) );
		//check if push did everything
		assert( 1 == list->count );
		assert( list->head );
		assert( 10 == *(int*) list->head->data );
		//everything seems fine, now lets clean up
		node = NULL;
		slist_node_delete(list, list->head);
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_append");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_node_append(NULL, NULL) );
		assert( NULL == slist_node_append(NULL, node) );
		assert( NULL == slist_node_append(list, NULL) );
		//test working append at head
		assert( slist_node_append(list, node) );
		assert( 1 == list->count );
		assert( list->head );
		assert( 10 == *(int*)list->head->data );
		//create another node to test at end append
		node = slist_node_new(list, int_copy(11), int_dalloc);
		//test working append at end
		assert( slist_node_append(list, node) );
		assert( 2 == list->count );
		assert( 11 == *(int*)list->head->next->data );
		//clean up
		//assume  slist_node_delete + slist_list_delete works
		slist_node_delete(list, list->head->next);
		slist_node_delete(list, list->head);
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_pop");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_node_pop(NULL) );
		assert( NULL == slist_node_pop(NULL) );
		//push node for further testings, assume slist_node_push works
		slist_node_push(list, node);
		node = NULL;
		assert( (node = slist_node_pop(list)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		//clean up
		slist_node_delete(list, node);
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_find");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//asume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == slist_node_find(NULL, NULL, NULL) );
		assert( NULL == slist_node_find(list, NULL, NULL) );
		assert( NULL == slist_node_find(NULL, key, NULL) );
		assert( NULL == slist_node_find(NULL, NULL, cmp_int) );
		assert( NULL == slist_node_find(list, key, NULL) );
		assert( NULL == slist_node_find(list, NULL, cmp_int) );
		assert( NULL == slist_node_find(NULL, key, cmp_int) );
		//empty list
		assert( NULL == slist_node_find(list, key, cmp_int) );
		//add node to list so we can test it
		slist_node_push(list, node);
		//test found in head
		node = NULL;
		assert( (node = slist_node_find(list, key, cmp_int)) );
		assert( 10 == *(int*)node->data );

		//add some more nodes to test other scenarios
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list, node);
		//after all pushes, 10 is tail
		//test found in end/tail
		assert( (node = slist_node_find(list, key, cmp_int)) );
		assert( 10 == *(int*)node->data );
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = slist_node_find(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == slist_node_find(list, key, cmp_int) );
		//clean up
		//there are 4 nodes.
		//(yes i know about loops i just prefer test like this)
		node = slist_node_pop(list);
		slist_node_delete(list,node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);
		//delete our list
		slist_list_delete(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_find_index_of");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
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
		slist_node_push(list, node);
		//test found in head
		assert( 1 == slist_find_index_of(list, key, cmp_int) );
		//add some more nodes to test other scenarios
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list, node);
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
		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);
		//delete our list
		slist_list_delete(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_remove");
		struct slist_list *list;
		struct slist_node *node;
		void *key;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		key = int_copy(10);
		//test failures
		assert( NULL == slist_node_remove(NULL, NULL, NULL) );
		assert( NULL == slist_node_remove(list, NULL, NULL) );
		assert( NULL == slist_node_remove(NULL, key, NULL) );
		assert( NULL == slist_node_remove(NULL, NULL, cmp_int) );
		assert( NULL == slist_node_remove(list, key, NULL) );
		assert( NULL == slist_node_remove(NULL, key, cmp_int) );
		//empty list
		assert( NULL == slist_node_remove(list, key, cmp_int) );
		//add node to list so we can test it
		slist_node_push(list, node);
		//test found in head
		node = NULL;
		assert( (node = slist_node_remove(list, key, cmp_int)) );
		assert( NULL == list->head );
		assert( 0 == list->count );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		slist_node_push(list, node);
		//add some more nodes to test other scenarios
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list, node);
		//after all pushes
		//test found and remove in end/tail
		assert( (node = slist_node_remove(list, key, cmp_int)) );
		assert( NULL == list->head->next->next->next );
		assert( 3 == list->count );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		slist_node_append(list, node);
		//edit key to test common :middle: found
		*(int*)key = 8;
		assert( (node = slist_node_remove(list, key, cmp_int)) );
		assert( 8 == *(int*)node->data );
		//delete the node
		slist_node_delete(list, node);
		//edit key to test not found
		*(int*)key = 100;
		//test not found
		assert( NULL == slist_node_remove(list, key, cmp_int) );
		//clean up
		//there are 3 nodes
		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);
		//delete our list
		slist_list_delete(list);
		//time to clean our key
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_remove_at");
		struct slist_list *list;
		struct slist_node *node;
		//assume slist_list_new works
		list = slist_list_new(NULL, NULL);
		//assume slist_node_new + int_copy works
		node = slist_node_new(list, int_copy(10), int_dalloc);
		//test failures
		assert( NULL == slist_node_remove_at(NULL, 1) );
		//test empty
		assert( NULL == slist_node_remove_at(list, 1) );
		//add the node for further testing
		//assume slist_node_push works
		slist_node_push(list, node);
		//test lower bounds
		assert( NULL == slist_node_remove_at(list, 0) );
		//test higher bounds
		assert( NULL == slist_node_remove_at(list, 100) );
		//test found at head
		node = NULL;
		assert( (node = slist_node_remove_at(list, 1)) );
		assert( 0 == list->count );
		assert( NULL == list->head );
		assert( 10 == *(int*)node->data );
		//push node back for further testing
		slist_node_push(list, node);
		//add some more nodes to test other scenarios
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list, node);
		//after all pushes ::there are 4 nodes::
		//test found and remove in end/tail
		assert( (node = slist_node_remove_at(list, 4)) );
		assert( 3 == list->count );
		assert( 10 == *(int*)node->data );
		//append our node back for now
		slist_node_append(list, node);
		//test common :middle: found
		node = NULL;
		assert( (node = slist_node_remove_at(list, 2)) );
		assert( 3 == list->count );
		assert( 8 == *(int*)node->data );
		//see if it was really removed
		assert( node != list->head->next );
		//delete our node
		slist_node_delete(list, node);
		//clean up
		//there are 3 nodes
		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list, node);
		//delete our list
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_node_foreach");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		//create some nodes to test
		node = slist_node_new(list, int_copy(10), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list,node);
		
		slist_node_foreach(list, action_test, NULL);
		//clean up
		node = slist_node_pop(list);
		slist_node_delete(list, node);

		node = slist_node_pop(list);
		slist_node_delete(list,node);

		node = slist_node_pop(list);
		slist_node_delete(list,node);
		
		node = slist_node_pop(list);
		slist_node_delete(list,node);
		
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_delete_all_nodes");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		//test failures
		assert( NULL == slist_list_delete_all_nodes(NULL) );
		//test empty
		assert( NULL == slist_list_delete_all_nodes(list) );
		//create some nodes for further testing
		node = slist_node_new(list, int_copy(10), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(9), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(8), int_dalloc);
		slist_node_push(list,node);
		node = slist_node_new(list, int_copy(7), int_dalloc);
		slist_node_push(list,node);
		assert( NULL != slist_list_delete_all_nodes(list) );
		assert( NULL == list->head );
		assert( 0 == list->count );
		//clean up
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_reverse");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		//test failures
		assert( NULL == slist_list_reverse(NULL) );
		//test empty
		assert( NULL == slist_list_reverse(list) );

		for(int i=1000; 0 < i; --i) 
		{
			node = slist_node_new(list, int_copy(i), int_dalloc);
			slist_node_push(list,node);
		}

		//execute
		assert( NULL != slist_list_reverse(list) );
		//do the actual test , set to silent
		slist_node_foreach(list, action_reverse_print_test, NULL);
		//clean up
		slist_list_delete_all_nodes(list);
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_get_size");
		struct slist_list *list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		node = slist_node_new(list, int_copy(10), int_dalloc);
		slist_node_push(list, node);
		assert( 1 == list->count && 1 == slist_get_size(list) );
		//clean up
		slist_node_delete(list, slist_node_pop(list));
		slist_list_delete(list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_push");
		struct slist_list *list;
		struct slist_list *s_list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		s_list = slist_list_new(NULL, NULL);
		//test failures
		assert( NULL == slist_list_push(NULL, NULL) );
		assert( NULL == slist_list_push(list, NULL) );
		assert( NULL == slist_list_push(NULL, s_list) );
		//test empty
		assert( NULL == slist_list_push(list, s_list) );
		//build the lists to test 'proper behavior'
		//list
		node = slist_node_new(list, int_copy(6), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(5), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(4), int_dalloc);
		slist_node_push(list, node);
		//s_list
		node = slist_node_new(s_list, int_copy(3), int_dalloc);
		slist_node_push(s_list, node);
		node = slist_node_new(s_list, int_copy(2), int_dalloc);
		slist_node_push(s_list, node);
		node = slist_node_new(s_list, int_copy(1), int_dalloc);
		slist_node_push(s_list, node);
		//test
		assert( NULL != slist_list_push(list, s_list) );
		//confirm that s_list is now empty
		assert( NULL == s_list->head );
		assert( 0 == s_list->count );
		//confirm that the merge was properly done
		//check for new list head (should be 1)
		assert( 1 == *(int*)list->head->data );
		//check if 3 node points to 'list' old head
		assert( 4 == *(int*)list->head->next->next->next->data );
		//clean up
		slist_list_delete_all_nodes(list);
		slist_list_delete(list);
		slist_list_delete(s_list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_append");
		struct slist_list *list;
		struct slist_list *s_list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		s_list = slist_list_new(NULL, NULL);
		//test failures
		assert( NULL == slist_list_append(NULL, NULL) );
		assert( NULL == slist_list_append(list, NULL) );
		assert( NULL == slist_list_append(NULL, s_list) );
		//test empty
		assert( NULL == slist_list_append(list, s_list) );
		//build the list to test 'proper behavior'
		//list
		node = slist_node_new(list, int_copy(6), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(5), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(4), int_dalloc);
		slist_node_push(list, node);
		//s_list
		node = slist_node_new(s_list, int_copy(3), int_dalloc);
		slist_node_push(s_list, node);
		node = slist_node_new(s_list, int_copy(2), int_dalloc);
		slist_node_push(s_list, node);
		node = slist_node_new(s_list, int_copy(1), int_dalloc);
		slist_node_push(s_list, node);
		//test
		assert( NULL != slist_list_append(list, s_list) );
		//confirm that s_list is now empty
		assert( NULL == s_list->head );
		assert( 0 == s_list->count );
		//confirm that the merge was properly done
		//check for list head (should be the same)
		assert( 4 == *(int*)list->head->data );
		//check if 3 node points to 's_list' old head
		assert( 1 == *(int*)list->head->next->next->next->data );
		//clean up
		slist_list_delete_all_nodes(list);
		slist_list_delete(list);
		slist_list_delete(s_list);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_split");
		struct slist_list *list;
		struct slist_list *n_list;
		struct slist_node *node;
		void *key;
		list = slist_list_new(NULL, NULL);
		key = int_copy(6);
		//test failures
		assert( NULL == slist_list_split(NULL, NULL, NULL) );
		assert( NULL == slist_list_split(list, NULL, NULL) );
		assert( NULL == slist_list_split(NULL, key, NULL) );
		assert( NULL == slist_list_split(NULL, NULL, cmp_int) );
		assert( NULL == slist_list_split(list, key, NULL) );
		assert( NULL == slist_list_split(NULL, key, cmp_int) );
		//test empty
		assert( NULL == slist_list_split(list, key, cmp_int) );
		//create node for testing
		node = slist_node_new(list, int_copy(6), int_dalloc);
		slist_node_push(list, node);
		//test found @ head and split
		assert( (n_list = slist_list_split(list, key, cmp_int)) );
		//check list is indeed empty
		assert( NULL == list->head );
		assert( 0 == list->count );
		//check n_list has all data properly set
		assert( NULL != n_list->head );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );
		//quick clean up
		slist_list_delete_all_nodes(n_list);
		slist_list_delete(n_list);
		n_list = NULL;
		node = NULL;
		//build some nodes for further testing
		node = slist_node_new(list, int_copy(6), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(5), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(4), int_dalloc);
		slist_node_push(list, node);
		//test end/tail split
		assert( (n_list = slist_list_split(list, key, cmp_int)) );
		//check list was properly split
		assert( NULL == list->head->next->next );
		assert( 2 == list->count );
		//check n_list has all data properly set
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );
		//save node in 'list' for further testing
		node = slist_node_pop(n_list);
		slist_node_append(list, node);
		//quick clean up
		slist_list_delete(n_list);
		n_list = NULL;
		//edit key to test common :middle: split
		*(int*)key = 5;
		//the test
		assert( (n_list = slist_list_split(list, key, cmp_int)) );
		//check list has everything properly set
		assert( 1 == list->count );
		assert( 4 == *(int*)list->head->data );
		//check n_list has everything properly set
		assert( 2 == n_list->count );
		assert( 5 == *(int*)n_list->head->data );
		assert( 6 == *(int*)n_list->head->next->data );
		assert( NULL == n_list->head->next->next );
		//clean up
		slist_list_delete_all_nodes(list);
		slist_list_delete_all_nodes(n_list);
		slist_list_delete(list);
		slist_list_delete(n_list);
		int_dalloc(key);
		wmsg("[OK]\n");
	}

	{
		wmsg("slist_list_split_at");
		struct slist_list *list;
		struct slist_list *n_list;
		struct slist_node *node;
		list = slist_list_new(NULL, NULL);
		//test failures
		assert( NULL == slist_list_split_at(NULL, 1) );
		//test empty
		assert( NULL == slist_list_split_at(list, 1) );
		//create some nodes for further testing
		node = slist_node_new(list, int_copy(6), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(5), int_dalloc);
		slist_node_push(list, node);
		node = slist_node_new(list, int_copy(4), int_dalloc);
		slist_node_push(list, node);
		//test lower bounds
		assert( NULL == slist_list_split_at(list, 0) );
		//test higher bounds
		assert( NULL == slist_list_split_at(list, 100) );
		//test index at head
		assert( (n_list = slist_list_split_at(list, 1)) );
		//check list is indeed empty
		assert( NULL == list->head );
		assert( 0 == list->count );
		//check n_list has all data properly set
		assert(NULL != n_list->head );
		assert( 3 == n_list->count );
		//move all data back to list and test next scenarios
		list->head = n_list->head;
		list->count = n_list->count;
		//quick clean up
		slist_list_delete(n_list);
		n_list = NULL;
		//test index at end/tail
		assert( (n_list = slist_list_split_at(list, 3)) );
		//check list has everything properly set
		assert( NULL != list->head );
		assert( 2 == list->count );
		assert( NULL == list->head->next->next );
		assert( 5 == *(int*)list->head->next->data );
		//check n_list has everything properly set
		assert( NULL != n_list->head );
		assert( 1 == n_list->count );
		assert( 6 == *(int*)n_list->head->data );
		//move data back to list for futher testing
		node = slist_node_pop(n_list);
		slist_node_append(list, node);
		//quick clean up
		slist_list_delete(n_list);
		//test common :middle: index
		assert( (n_list = slist_list_split_at(list, 2)) );
		//check list has everything properly set
		assert( NULL != list->head );
		assert( NULL == list->head->next );
		assert( 1 == list->count );
		assert( 4 == *(int*)list->head->data );
		//check n_list has everything properly set
		assert( NULL != n_list->head );
		assert( 2 == n_list->count );
		assert( 5 == *(int*)n_list->head->data );
		assert( NULL == n_list->head->next->next );
		//clean up
		slist_list_delete_all_nodes(list);
		slist_list_delete_all_nodes(n_list);
		slist_list_delete(list);
		slist_list_delete(n_list);
		wmsg("[OK]\n");
	}


	return 0;
}
