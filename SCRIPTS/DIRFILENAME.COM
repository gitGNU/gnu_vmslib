$ __save_verif = 'f$verify(0)
$!
$! GNU vmslib - Helps people port GNU software to VMS.
$! Copyright (C) 1994-2008 the Free Software Foundation, Inc.
$!
$! Author: Richard Levitte <levitte@lp.se>
$! Maintainer: Chris Bryant <cabryant@mednet.ucla.edu>
$!
$! This program is free software: you can redistribute it and/or modify
$! it under the terms of the GNU General Public License as published by
$! the Free Software Foundation, either version 3 of the License, or
$! (at your option) any later version.
$!
$! This program is distributed in the hope that it will be useful,
$! but WITHOUT ANY WARRANTY; without even the implied warranty of
$! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
$! GNU General Public License for more details.
$!
$! You should have received a copy of the GNU General Public License
$! along with this program.  If not, see <http://www.gnu.org/licenses/>.
$!
$! DIRFILENAME.COM -- converts foo:[x.y] to foo:[x]y.dir
$!		      converts foo:[x] to foo:[000000]x.dir
$!
$! Description:
$!
$! P1 = directory spec
$! P2 = global variable to put result in
$!
$ __debug = 0
$
$ __node = f$parse(p1,,,"NODE","SYNTAX_ONLY")
$ __dev = f$parse(p1,,,"DEVICE","SYNTAX_ONLY")
$ if p1 - __dev .eqs. p1 then __dev = ""
$ __dir = p1 - __node - __dev
$ __dir = __dir - "[" - "]" - "<" - ">"
$ if __debug
$  then
$   sh sym __node
$   sh sym __dev
$   sh sym __dir
$  endif
$ __i = 0
$ __e = ""
$loop_dir:
$ __olde = __e
$ __e = f$element(__i,".",__dir)
$ if __e .nes. "."
$  then
$   __i = __i + 1
$   goto loop_dir
$  endif
$
$ if __olde .nes. ""
$  then
$   if __dir - ("."+__olde) .nes. __dir
$    then
$     if __dev .nes. "" .or. __dir - ("."+__olde) .nes. ""
$      then __dir = __dir - ("."+__olde)
$      else __dir = __dir - __olde
$      endif
$    else __dir = __dir - __olde
$    endif
$   __dir = "[" + __dir + "]"
$   if __debug then sh sym __dir
$   if __dir .eqs. "[]" then __dir = "[000000]"
$   if __dir .eqs. "[.]" then __dir = ""
$   'p2' == __node+__dev+__dir+__olde+".DIR"
$   if __debug then sh sym 'p2'/global
$  endif
$exit:
$ exit 1 + 0*f$verify(__save_verif)
