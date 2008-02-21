
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

static char hackdebug_version[] = "VMS hackdebug version 1.1";

#ifdef __DECC
#define close decc$close
#define lseek decc$lseek
#define open decc$open
#if __DECC_VER < 50200000
/* For some reason, this one IS declared in stdio.h with DEC C >5.0 */
#define read decc$read
#endif
#define write decc$write
#endif

/* Now this is probably one of the dirtiest hacks you've ever seen */

/* For now, this is a VERY ugly hack, with everything hardcoded...
   What more do we need? */

#include <unixio.h>
#include <file.h>

#ifdef __DECC
#if __VMS_VER < 70000000
/* Of course, with VMS <7.0, read() is STILL not in the library! */
#define read decc$read
#endif
#endif

#define FLAGS (O_RDWR|O_EXCL)
#define MODE (0755)

#define BLOCK_SIZE (512)

#ifdef VAX
#define offset (0x20)
#define mask (~0x01)
#define value (0x01)
#else
#define offset (0x50)
#define mask (~0x01)
#define value (0x01)
#endif

static char buffer[BLOCK_SIZE];

int main (argc, argv)
     int argc;
     char *argv[];
{
#define IMAGE argv[1]
  int fd = open (IMAGE, FLAGS, MODE);

  if (fd == -1)
    {
      perror ("Could not open TEMACS.EXE");
      return 4; /* This is fatal */
    }

  read (fd, buffer, BLOCK_SIZE);

  buffer[offset] &= mask;

  lseek (fd, 0, SEEK_SET);

  write (fd, buffer, BLOCK_SIZE);

  close (fd);

  return 1;
}
