
/*
 * GNU vmslib - Helps people port GNU software to VMS.
 * Copyright (C) 1994-2008 the Free Software Foundation, Inc.
 *
 * Author: Richard Levitte <levitte@lp.se>
 * Maintainer: Chris Bryant <chris.bryant@kofc13555.org>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

void fatal ();
void *xmalloc ();
void *xrealloc ();
void x_free();
/* This is a trick, because the linker wants uppercase symbols, and in
   that case, xfree is confused with Xfree, which is bad.  */
#define xfree x_free

/*
 * Some string utilities.
 */
char *downcase ();
char *strdup ();
char *strndup ();

int fixpath ();
char *argvconcat ();
