/*
 * common.h
 * This file is part of dutils library collection
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

#include "common.h"


/****************************************************************************
 * helper functions used by the tests
 ****************************************************************************/

void *int_copy(int n)
{
	int *ptr = malloc( sizeof( int ) );
	if (!ptr) {
		wmsg("%s malloc failed\n", __func__);
		return NULL;
	}

	*ptr = n;
	return ptr;
}

void int_dalloc(void *data)
{
	free(data);
	data = NULL;
}

void *action_test(void *carry, void *data, void *param)
{
	//allow silent testing
	if (param) {
		wmsg("%d\n", *(int*)data);
	}
	return NULL;
}

void *action_reverse_print_test(void *carry, void *data, void *param)
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

int print_test_global = -1;
void print_test(void *data)
{
	//clean the global first
	print_test_global = -1;
	//save the test result in the global
	print_test_global = !((*(int*)data) == 10 );
}

int cmp_int(void *a, void *b)
{
	if (!a || !b)
		return -1;

	return !( *(int*)a  == *(int*)b );
}
