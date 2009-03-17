
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

#ifdef __DECC
#define isatty decc$isatty
#if __DECC_VER < 50200000
#define read decc$read
#endif
#endif

#include <unixio.h>
#include <setjmp.h>
/* The following include file defines the name "read" */
#include "vl_stdio.h"

/* simple replacement for standard read.
   Snarfed from cccp.c of GNU cc 2.6.3 */

static int assume_mailbox = 0;
static int add_nl = 0;

int VMS_read_assume_mailbox (i)
     int i;
{
  int j = assume_mailbox;

  assume_mailbox = i;
  add_nl = 0;
  return j;
}

int
read (fd, buf, size)
     int fd;
     char *buf;
     int size;
{
#ifdef read
#undef read	/* Get back the REAL read routine */
#endif
#ifdef __DECC
#define read decc$read
#endif
  register int i;
  register int total = 0;

  /* Read until the buffer is exhausted */
  while (size > 0) {
    /* Limit each read to 32KB */
    if (add_nl)
      {
	*buf = '\n';
	i = 1;
	add_nl = 0;
      }
    else
      {
	i = (size > (32*1024)) ? (32*1024) : size;
	i = read (fd, buf, i);
	if (i <= 0) {
	  if (i == 0)
	    return (total);
	  return (i);
	}
	if (assume_mailbox)
	  add_nl = 1;
      }
    /* Account for this read */
    total += i;
    buf += i;
    size -= i;

    if (!assume_mailbox && isatty (fd))
      return total;
  }

  return (total);
}
