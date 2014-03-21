/*
 * common.t.c
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

#define LOUD
#include "common.h"

int main(int argc, char **argv)
{

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

	return 0;
}
