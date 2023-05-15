</$objtype/mkfile

BIN=$home/bin/$objtype/pse
TARG=\
	fs\
	view\

HFILES=\
	gen3.h\
	gen3dat.h\

OFILES=\
	gen3.$O\
	gen3dat.$O\
	dex.$O\

</sys/src/cmd/mkmany

gen3dat.c: gen3dat.h
	dfc -l $prereq > $target

pokesprite:
	git/clone https://github.com/msikma/pokesprite.git

installsprite:	pokesprite
	mkdir -p sprite/regular
	for(i in pokesprite/pokemon-gen7x/regular/*.png)
		png -tc $i | resize -n '-x200%' '-y200%' > sprite/regular/^`{basename $i}
	mkdir -p /sys/games/lib/pokesprite
	clone sprite/* /sys/games/lib/pokesprite

install:V:
	mkdir -p $BIN
	for(i in $TARG)
		mk $MKFLAGS $i.install
