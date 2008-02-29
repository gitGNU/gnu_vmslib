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
$! COMPARE_DIRS.COM -- Compares directory specifications
$!
$! Description:
$!
$! P1 = first directory
$! P2 = second directory
$! P3 = global variable to put result in
$
$ proc = f$environment("PROCEDURE")
$ proc_dir = f$parse(proc,,,"NODE")+f$parse(proc,,,"DEVICE")+f$parse(proc,,,"DIRECTORY")
$ __debug = f$trnlnm("DEBUG_COMPARE_DIRS")
$ if __debug
$  then
$   write sys$output "%COMPARE_DIRS-I-DEBUG, P1 = """,p1,""""
$   write sys$output "%COMPARE_DIRS-I-DEBUG, P2 = """,p2,""""
$  endif
$ @'proc_dir'decomplnm 'p1' __c_d_dir1 DIRS,RETURN_DEVICE
$ @'proc_dir'decomplnm 'p2' __c_d_dir2 DIRS,RETURN_DEVICE
$ if __debug
$  then
$   write sys$output "%COMPARE_DIRS-I-DEBUG, __c_d_dir1 = """,__c_d_dir1,""""
$   write sys$output "%COMPARE_DIRS-I-DEBUG, __c_d_dir2 = """,__c_d_dir2,""""
$  endif
$ 'p3' == f$edit(__c_d_dir1,"UPCASE") .eqs. f$edit(__c_d_dir2,"UPCASE")
$
$exit:
$ exit 1 + 0*f$verify(__save_verif)
