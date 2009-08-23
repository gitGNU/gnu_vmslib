
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

#include <stdlib.h>
#include <descrip.h>
#include <rmsdef.h>
#include <nam.h>
#include <errno.h>
#include <string.h>
#include <lib$routines.h>

/*
 * char **glob (char *try)
 *
 * Synopsis
 *   Return a NULL-terminate array of existing files matching pattern TRY
 *   or NULL if no matches.  Treats '?' the same as '%' for unix
 *   compatibility.
 *
 * Errors
 *   ENOMEM  - memory (re)allocation failed,
 *   EVMSERR - lib$find_file did not return RMS$_NORMAL or RMS$_NMF
 *
 *   In either case, glob() will return as many matches as possible;
 *   thus, checking only the return value is _not_ sufficient.
 *
 * Author:
 *   Roland B Roberts (roberts@nsrl.rochester.edu)
 *   March 1994
 *
 * Modification History
 *   1 Sep 94 - RBR
 *      - Removed references to xmalloc(), xrealloc(), strdup()
 *      - Fixed memory leak, sid.dsc$a_pointer was not free'd for error exits
 *      - Removed `if (status == RMS$_NMF)' test, always free context block
 */

/* VMS variable-length string struct.
   Works with DSC$K_CLASS_VS descriptors */
struct Vstring
{
  short length;
  char body[NAM$C_MAXRSS+1];
};

char **glob (try)
  char *try;
{
  long status;                  /* VMS status coes */
  long context;                 /* RMS search context */
  char **list;                  /* List of matches */
  long sz;                      /* Allocated size of list */
  int nfiles;                   /* Number of matches */
  static struct Vstring file;   /* Current/last match */
  static $DESCRIPTOR(sdd,"SYS$DISK:[]*.*"); /* Default file name */
  static struct dsc$descriptor sid = /* Input descriptor */
    { 0, DSC$K_DTYPE_T, DSC$K_CLASS_S, NULL };
  static struct dsc$descriptor sod = /* Output descriptor */
    { sizeof(file), DSC$K_DTYPE_VT, DSC$K_CLASS_VS, (void *) &file };
  
  /* Initialize input descriptor */
  sid.dsc$w_length  = strlen(try);
  sid.dsc$a_pointer = malloc ((1+strlen(try)) * sizeof(char));
  if (!sid.dsc$a_pointer)
    {
      errno = ENOMEM;
      return NULL;
    }
  strcpy (sid.dsc$a_pointer, try);
  
  /* Convert unix-style wildcards to VMS style
     Note this does NOT convert unix-syntax names to VMS style */
  {
    char *qm;
    while (qm = strchr(sid.dsc$a_pointer,'?'))
      *qm = '%';
  }
  
  /* Got to be able to allocate space.
     malloc() and friends don't set errno.  Go figure. */
  list = (char **) malloc ((sz=10) * sizeof(char *));
  if (!list)
    {
      errno = ENOMEM;
      free (sid.dsc$a_pointer);
      return NULL;
    }

  context = 0;
  nfiles = 0;
  while ((status=lib$find_file(&sid,&sod,&context,&sdd,0,0,0)) == RMS$_NORMAL)
    {
      file.body[file.length] = 0;
      /* Need to make room for more files */
      if (sz <= nfiles)
        {
          char **tmp = (char **) realloc (list, (sz = nfiles+10) * sizeof (char *));
          /* Failed realloc.  Release the last match and return what we've got */
          if (!tmp)
            {
              free (list[nfiles]);
              list[nfiles] = NULL;
              errno = ENOMEM;
              free (sid.dsc$a_pointer);
              return list;
            }
          list = tmp;
        }
      /* Make room for the resultant file name */
      list[nfiles] = (char *) malloc ((1+file.length) * sizeof(char));
      if (!list[nfiles])
        {
          list[nfiles] = NULL;
          errno = ENOMEM;
          free (sid.dsc$a_pointer);
          return list;
        }
      strcpy (list[nfiles++], file.body);
    }
  
  /* Something went wrong.
     Note that it is possible for glob() to return a file list and
     still have encountered an error */
  if (status != RMS$_NMF)
    {
      errno = EVMSERR;
      vaxc$errno = status;
    }
  
  /* Always free the RMS contet block */
  lib$find_file_end (&context);
  context = 0;

  /* Free everything if there are no files */
  if (nfiles == 0)
    {
      free (list);
      free (sid.dsc$a_pointer);
      return (NULL);
    }
  
  /* Downsize the block and return it.
     Actually, this might increase the block by one. */
  {
    char **tmp = (char **) realloc (list, (nfiles+1) * sizeof (char *));
    /* Failed realloc.  Release the last match and return what we've got */
    if (!tmp)
      {
        free (list[nfiles]);
        errno = ENOMEM;
      }
    else
      list = tmp;
  }
  list[nfiles] = NULL;
  free (sid.dsc$a_pointer);
  return (list);
}
