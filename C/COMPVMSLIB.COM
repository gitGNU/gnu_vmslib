$ set noverify
$!
$! VMS command file to recompile all VMSLIB .C files which need recompilation.
$! These are all .C files that have no .OBJ files or that are newer then the
$! corresponding .OBJ files.  This file is self contained and does not require
$! you to do anything before running it.
$!
$! Arguments
$!   P1 any value forces recompilation of all .C files
$!
$ if p1 .eqs. "" 
$ then force = 0
$ else force = 1
$ endif
$ srcdir_dev := @srcdir_dev@
$ srcdir_dir := @srcdir_dir@
$! This is just to make absolutelly sure we don't get any local definition
$! of __result
$ __result = ""
$ delete/symbol/local __result
$! Let's shorten the paths as much as possible...
$ @'srcdir_dev'['srcdir_dir']canonicaldir 'srcdir_dev'['srcdir_dir'] __result
$ vmssrc = __result
$ @'vmssrc'canonicaldir 'srcdir_dev'['srcdir_dir'.-.src] __result
$ define __srcdir '__result'
$
$ @precomp
$ olddef = f$environment("DEFAULT")
$! We KNOW this script is on the [.VMS] subdirectory, so let's move to it
$ proc = f$environment("PROCEDURE")
$ proc_dir = f$parse(proc,,,"NODE")+f$parse(proc,,,"DEVICE")+f$parse(proc,,,"DIRECTORY")
$ set def 'proc_dir'
$ set def [-.src]
$ write sys$output "---------------"
$!
$! If there is any need for a listing file (foo.LIS), do the call like this:
$!
$!	$ call recomp foo.c "''ccom' /list/show=include"
$!
$ call recomp esctrans.c
$ call recomp glob.c
$ call recomp hackargv.c
$ call recomp misc.c
$
$ deassign __srcdir
$ set default 'olddef'
$ exit
$!
$! VMS subroutine file to recompile a .C file which needs recompilation.
$! This is a .C files that has no .OBJ file or that is newer
$! than the corresponding .OBJ file.  This file is self contained
$! and does not require you to do anything before running it.
$!
$ recomp: subroutine
$ on error then continue
$ file = f$search("__srcdir:"+p1,1)
$ cmd  = p2
$ if cmd .eqs. "" 
$ then
$!   if ccom .eqs. "" then call precomp
$   cmd = ccom
$ endif
$ name = f$parse(file,,,"NAME")
$ obj  = name + ".OBJ"
$ if (f$search(obj) .nes. "") .and. (.not. force)
$ then
$   if f$cvtime(f$file(file,"RDT")) .les. f$cvtime(f$file(obj,"RDT")) then -
	goto sub_exit
$ endif
$ obj = f$parse(file,,,"DEVICE") + f$parse(file,,,"DIRECTORY") + obj
$ write sys$output "Compiling ''name'..."
$ on error then goto error_exit
$ 'cmd' 'file' /object='obj'
$ if f$search(obj) .nes. "" then purge /nolog 'obj'
$error_exit:
$ write sys$output "---------------"
$sub_exit:
$ exit
$!
$ endsubroutine
