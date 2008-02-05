/*
 * Copyright © 1994 the Free Software Foundation, Inc.
 *
 * Author: Richard Levitte <levitte@e.kth.se>
 *
 * This file is a part of GNU VMSLIB, the GNU library for porting GNU
 * software to VMS.
 *
 * GNU VMSLIB is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GNU VMSLIB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU VMSLIB; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef __DECC
#define write decc$write
#endif

#include <unixio.h>
/* The following include file defines the name "write" */
#include "vl_stdio.h"

/* simple replacement for standard write.
   Snarfed from cccp.c of GNU cc 2.6.3 */

int
write (fd, buf, size)
     int fd;
     char *buf;
     int size;
{
#ifdef write
#undef write	/* Get back the REAL write routine */
#endif
#ifdef __DECC
#define write decc$write
#endif
  int i;
  int j;

  /* Limit individual writes to 32Kb */
  i = size;
  while (i > 0) {
    j = (i > (32*1024)) ? (32*1024) : i;
    if (write (fd, buf, j) < 0) return (-1);
    /* Account for the data written */
    buf += j;
    i -= j;
  }
  return (size);
}
