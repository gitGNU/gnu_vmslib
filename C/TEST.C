
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

#include <stdio.h>

main (int argc, char **argv)
{
  int i;
  for (i = 0; i < argc; i++) {
    printf ("stdout: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
  }
  hackargv (&argc, &argv);
  printf ("stdout: main line %3d :- argc == %d\n", __LINE__, argc);
  for (i = 0; i < argc; i++) {
    printf ("stdout: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
    fprintf (stderr, "stderr: main line %3d :- argv[%2d] = %s\n", __LINE__, i, argv[i]);
  }
}
