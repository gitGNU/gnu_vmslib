
dnl GNU vmslib - Helps people port GNU software to VMS.
dnl Copyright (C) 1994-2008 the Free Software Foundation, Inc.
dnl
dnl Author: Richard Levitte <levitte@lp.se>
dnl Maintainer: Chris Bryant <chris.bryant@kofc13555.org>
dnl
dnl This program is free software: you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation, either version 3 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program.  If not, see <http://www.gnu.org/licenses/>.
dnl
dnl -----
dnl
dnl This file just contains a hack to accumulate a bunch of logical
dnl names together with their DECOMPOSED value!
dnl
dnl -----
dnl
dnl VMS_ACCUMULATE_LOGICAL
dnl This macro takes three arguments:
dnl	$1 - logical name.
dnl	$2 - direcory and name of the script which does the decomposition.
dnl	$3 - DIRS if the logical is expected to represent a directory.
define([VMS_ACCUMULATE_LOGICAL],dnl
[$ if f$type(__logicals_n) .eqs. "" then __logicals_n = 0
$ __logicals_i = 0
$ __tmp := [$1]
$ if f$extract(f$length(__tmp)-1,1,__tmp) .eqs. ":" then -
	__tmp = f$extract(0,f$length(__tmp)-1,__tmp)
$AC_DEFINE_VMS_LABLE(loop_check_logicals,logicals)AC_USE_VMS_LABLE(loop_check_logicals):
$ if __logicals_i .lt. __logicals_n
$  then
$   if __logical_name'__logicals_i' .nes. "''__tmp'"
$    then
$     __logicals_i = __logicals_i + 1
$     goto AC_USE_VMS_LABLE(loop_check_logicals)
$    endif
$  endif
$ if __logicals_i .eq. __logicals_n
$  then
$   __logical_name'__logicals_n' = __tmp
$   @[$2] '__tmp' __result [$3] __mode
$   if __result .nes. ""
$    then
$     __logical_mode'__logicals_n' = __mode
$     __logical_value'__logicals_n' = __result
$     __logicals_n = __logicals_n + 1
$    endif
$  endif
$])dnl
dnl
dnl -----
dnl
dnl VMS_DUMP_LOGICALS
dnl This macro takes one argument:
dnl	$1 - the logical which points at a file to write to.
define([VMS_DUMP_LOGICALS],dnl
[$ __logicals_i = 0
$loop_logicals:
$ if __logicals_i .lt. __logicals_n
$  then
$   write $1 " D\",__logical_name'__logicals_i',"\",-
	__logical_mode'__logicals_i',"\ -"
$   __j = 0
$   __olde = 0
$  loop_values:
$   __e = f$element(__j,",",__logical_value'__logicals_i')
$   if f$type(__olde) .eqs. "STRING"
$    then
$     if __e .nes. ","
$      then write $1 "	",__olde,",-"
$      else write $1 "	",__olde
$      endif
$    endif
$   __olde = __e
$   __j = __j + 1
$   if __e .nes. "," then goto loop_values
$   __logicals_i = __logicals_i + 1
$   goto loop_logicals
$  endif
$])
dnl
dnl -----
dnl
dnl VMS_INSERT_LOGICALS
dnl Hack a file to include all the dumped logicals
dnl $1 = template file to use as source.
dnl $2 = goal file.
dnl $3 = file with logical name definitions (by default, CONFIG.LOGICALS).
dnl $4 = word to replace (by default, @all_logicals@).
dnl
define([VMS_INSERT_LOGICALS],[dnl
$ open/write foo config.tpu_tmp
$ write foo "input_file := GET_INFO (COMMAND_LINE, ""file_name"");"
$ write foo "main_buffer := CREATE_BUFFER (""main"", input_file);"
$ write foo "p_spcs := (SPAN("" "") | """");"
$ write foo "!"
$ write foo "POSITION (BEGINNING_OF (main_buffer));"
$ write foo "my_pattern := LINE_BEGIN + (""[#]""@r + "" Generated"" | ""/**/[#]""@r) + unanchor + """" + LINE_END;"
$ write foo "LOOP"
$ write foo "  my_range := SEARCH_QUIETLY (my_pattern, FORWARD);"
$ write foo "  EXITIF my_range = 0;"
$ write foo "  POSITION (r); COPY_TEXT(""!"");"
$ write foo "  POSITION (END_OF (my_range));"
$ write foo "ENDLOOP;"
$ write foo "!"
$ write foo "POSITION (BEGINNING_OF (main_buffer));"
ifelse([$4], ,[dnl
$ write foo "my_pattern := LINE_BEGIN + (""@all_logicals@""@r) + unanchor + """" + LINE_END;"],[dnl
$ write foo "my_pattern := LINE_BEGIN + (""[$1]""@r) + unanchor + """" + LINE_END;"])
$ write foo "LOOP"
$ write foo "  my_range := SEARCH_QUIETLY (my_pattern, FORWARD);"
$ write foo "  EXITIF my_range = 0;"
ifelse([$3], ,[dnl
$ write foo "  ERASE (my_range); POSITION (my_range); READ_FILE (""CONFIG.LOGICALS"");"],[dnl
$ write foo "  ERASE (my_range); POSITION (my_range); READ_FILE (""[$3]"");"])
$ write foo "  POSITION (END_OF (my_range));"
$ write foo "ENDLOOP;"
$ write foo "!"
$ write foo "WRITE_FILE (main_buffer, get_info (command_line, ""output_file""));"
$ write foo "QUIT;"
$ close foo
$ save_mesg = f$environment("MESSAGE")
$ set message/nofacility/noseverity/noident/notext
$ write sys$output "creating [$2]"
$ edit/tpu/nosection/command=config.tpu_tmp/nodisplay/out=config.new_tmp [$1]
$ set message 'save_mesg'
$ if f$search("[$2]") .nes. "" then -
	diff config.new_tmp [$2]/out=nla0:
$ if $status .eq. %X06c8009 ! code for no change
$  then
$   ! The file exists and we would not be changing it.
$   delete config.new_tmp.*/nolog
$   write sys$output "[$2] is unchanged."
$  else
$   rename config.new_tmp [$2]
$   purge/nolog [$2]
$  endif
$ delete config.tpu_tmp.*,config.new_tmp.*
$])dnl
