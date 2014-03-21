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

#ifndef DUTILS_COMMON_H_
#define DUTILS_COMMON_H_

//libraries used by helper functions
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/****************************************************************************
 * configure messages
 ****************************************************************************/
#ifdef LOUD
#define wmsg(...) fprintf(stderr,__VA_ARGS__);
#else
#define wmsg(...)
#endif

void *int_copy(int n);

void int_dalloc(void *data);

void *action_test(void *carry, void *data, void *param);

void *action_reverse_print_test(void *carry, void *data, void *param);

//helper global variable for print_test
extern int print_test_global;
void print_test(void *data);

int cmp_int(void *a, void *b);

#endif


