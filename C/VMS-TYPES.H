
/*
 * GNU vmslib - Helps people port GNU software to VMS.
 * Copyright (C) 1994-2008 the Free Software Foundation, Inc.
 *
 * Author: Richard Levitte <levitte@lp.se>
 * Maintainer: Chris Bryant <cabryant@mednet.ucla.edu>
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

#ifndef __types_loaded__
#define __types_loaded__ 1

#include <stddef.h>

/*
 * Miscellaneous VMS types that are not normally defined
 * in any consistent fashion.
 */

/* VMS I/O status block */
struct IOSB
{
  short status, count;
  long devinfo;
};

/* VMS Item List 3 structure */
struct itm$list3
{
  short buflen;
  short itemcode;
  void *buffer;
  size_t *retlen;
};

/* VMS Lock status block with value block */
struct LOCK
{
  short status, reserved;
  long lockid;
  long value[4];
};

/* VMS Exit Handler Control block */
struct EXHCB
{
  struct exhcb *exh$a_link;
  int (*exh$a_routine)();
  long exh$l_argcount;
  long *exh$a_status;
  long exh$l_status;
};

#endif /* __types_loaded__ 1 */
