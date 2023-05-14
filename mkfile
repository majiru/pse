</$objtype/mkfile

BIN=$home/bin/$objtype
TARG=pse
HFILES=\
	gen3.h\

OFILES=\
	gen3.$O\
	gen3dat.$O\

</sys/src/cmd/mkone

gen3dat.c: gen3.h
	dfc -l $prereq > $target
