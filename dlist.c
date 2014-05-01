/*
 * dlist.c
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

/****************************************************************************
 * dlist library interface implementation
 ****************************************************************************/


struct dlist_list *dlist_init(struct dlist_list *list,
			      void *(*node_alloc)(size_t),
			      void (*node_dalloc)(void *))
{
}/* dlist_init */


struct dlist_list *dlist_new_list(void *(*node_alloc)(size_t),
				  void (*node_dalloc)(void *))
{
}/* dlist_new_list */


struct dlist_node *dlist_new_node(struct dlist_list *list,
				  void *data, void (*dalloc)(void *))
{
}/* dlist_new_node */


inline void dlist_print_node(const struct dlist_node *node,
			     void (*print)(void *))
{
}/* dlist_print_node */


void dlist_delete_node(struct dlist_list *list,
		       struct dlist_node *node)
{
}/* dlist_delete_node */


void *dlist_delete_list(struct dlist_list *list)
{
}/* dlist_delete_list */


struct dlist_node *dlist_push_node(struct dlist_list *list,
				   struct dlist_node *node)
{
}/* dlist_push_node */


struct dlist_node *dlist_append_node(struct dlist_list *list,
				     struct dlist_node *node)
{
}/* dlist_append_node */


struct dlist_node *dlist_pop_node(struct dlist_list *list)
{
}/* dlist_pop_node */


struct dlist_node *dlist_find_node(struct dlist_list *list, void *key,
				   int (*cmp)(void *a, void *b))
{
}/* dlist_find_node */


size_t dlist_find_index_of(struct dlist_list *list, void *key,
			   int (*cmp)(void *a, void *b))
{
}/* dlist_find_index_of */


struct dlist_node *dlist_remove_node(struct dlist_list *list, void *key,
				     int (*cmp)(void *a, void *b))
{
}/* dlist_remove_node */


struct dlist_node *dlist_remove_node_at(struct dlist_list *list,
				       const size_t index)
{
}/* dlist_remove_node_at */


void dlist_foreach_node(struct dlist_list *list,
			void *(*action)(void *carry, void *data, void *param),
			void *param)
{
}/* dlist_foreach_node */


struct dlist_list *dlist_delete_all_nodes(struct dlist_list *list)
{
}/* dlist_delete_all_nodes */


struct dlist_list *dlist_reverse_list(struct dlist_list *list)
{
}/* dlist_reverse_list */


inline size_t dlist_get_size(struct dlist_list *list)
{
}/* dlist_get_size */


struct dlist_list *dlist_push_list(struct dlist_list *list,
				   struct dlist_list *s_list)
{
}/* dlist_push_list */


struct dlist_list *dlist_append_list(struct dlist_list *list,
				     struct dlist_list *s_list)
{
}/* dlist_append_list */


struct dlist_list *dlist_split_list(struct dlist_list *list, void *key,
				    int (*cmp)(void *a, void *b))
{
}/* dlist_split_list */


struct dlist_list *dlist_split_list_at(struct dlist_list *list,
				       const size_t index)
{
}/* dlist_split_list_at */
