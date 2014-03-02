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

/****************************************************************************
 * configure messages
 ****************************************************************************/
#ifdef LOUD
#define wmsg(...) fprintf(stderr,__VA_ARGS__);
#else
#define wmsg(...)
#endif


#endif


