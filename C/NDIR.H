
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

/*
	<ndir.h> -- definitions for 4.2BSD-compatible directory access

	last edit:	28-dec-1994	Richard Levitte
*/

#if 0
#ifndef FAB$C_BID
#include <fab.h>
#endif
#endif
#ifndef NAM$C_BID
#include <nam.h>
#endif
#if 0
#ifndef RMS$_SUC
#include <rmsdef.h>
#endif
#include <dir.h>
#else
#define	DIR$S_NAME	80
#endif

#define DIRBLKSIZ	512		/* size of directory block */
#ifdef VMS
#define MAXNAMLEN	(DIR$S_NAME + 7) /* 80 plus room for version #.  */
#define MAXFULLSPEC	NAM$C_MAXRSS /* Maximum full spec */
#else
#define MAXNAMLEN	15		/* maximum filename length */
#endif /* VMS */
	/* NOTE:  MAXNAMLEN must be one less than a multiple of 4 */

struct direct				/* data from readdir() */
	{
	long		d_ino;		/* inode number of entry */
	unsigned short	d_reclen;	/* length of this record */
	unsigned short	d_namlen;	/* length of string in d_name */
	char		d_name[MAXNAMLEN+1];	/* name of file */
	};

typedef struct
	{
	int	dd_fd;			/* file descriptor */
	int	dd_loc;			/* offset in block */
	int	dd_size;		/* amount of valid data */
	char	dd_buf[DIRBLKSIZ];	/* directory block */
	}	DIR;			/* stream data from opendir() */

extern DIR		*opendir();
extern struct direct	*readdir();
#ifndef VMS
extern long		telldir();
extern void		seekdir();
#endif
extern void		closedir();

#define rewinddir( dirp )	seekdir( dirp, 0L )
