
/*
 * GNU vmslib - Helps people port GNU software to VMS.
 * Copyright (C) 1994-2008 the Free Software Foundation, Inc.
 *
 * Author: Richard Levitte <levitte@lp.se>
 * Maintainer: Chris Bryant <chrisbryant@ucla.edu>
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
