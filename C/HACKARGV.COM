$ gcc/noopt/debug hackargv
$ gcc/noopt/debug pipe
$ gcc/noopt/debug glob
$ gcc/noopt/debug esctrans
$ gcc/noopt/debug misc
$ gcc/noopt/debug test
$ gcc/noopt/debug echo
$ link test,hackargv,pipe,glob,esctrans,misc,sys$input/opt
gnu_cc:[000000]gcclib/lib
sys$share:vaxcrtl/share
$ link echo,hackargv,pipe,glob,esctrans,misc,sys$input/opt
gnu_cc:[000000]gcclib/lib
sys$share:vaxcrtl/share
$ test := sys$disk:[]test
$ echo := sys$disk:[]echo
$ exit
