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
#define close decc$close
#define fstat decc$fstat
#define open decc$open
#endif

#include <unixio.h>
#include <file.h>
#include <sys/stat.h>
/* The following include file defines the name "read" */
#include "vl_stdio.h"

/* simple replacement for standard read.
   Snarfed from cccp.c of GNU cc 2.6.3 */

int
stat (path, buf)
#ifdef stat
#undef stat	/* Get back the REAL read routine */
#endif
     char *path;
     struct stat *buf;
{
  register int fd = open (path, O_RDONLY, 0);
  register int st = fstat (fd, buf);
  close (fd);
  return st;
}
