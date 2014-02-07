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
 * helper functions used by the library
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

	return 0;
}
