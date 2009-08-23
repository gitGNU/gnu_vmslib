$ __save_verif = 'f$verify(0)
$!
$! GNU vmslib - Helps people port GNU software to VMS.
$! Copyright (C) 1994-2008 the Free Software Foundation, Inc.
$!
$! Author: Richard Levitte <levitte@lp.se>
$! Maintainer: Chris Bryant <chrisbryant@ucla.edu>
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
$ set symbol/verb/scope=(noglobal,nolocal)
$ if f$search("''p2'") .nes. ""
$  then
$   diff 'p1' 'p2'/output=nla0:
$   if $severity .eq. 3
$    then
$     set prot=(owner:red,group:red,world:red) 'p2'.*
$     delete/nolog 'p2'.*
$     copy/nolog 'p1' 'p2'
$     delete/nolog 'p1'.*
$    else
$     write sys$output "''p2' is unchanged."
$     delete/nolog 'p1'.*
$    endif
$  else
$   copy/nolog 'p1' 'p2'
$   delete/nolog 'p1'.*
$  endif
$ exit
