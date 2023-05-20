#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>
#include "gen3dat.h"
#include "colodat.h"
#include "pse.h"

Gen3 gen3;

enum { Qtrainer, Qpokemon };
typedef struct Xfile Xfile;
struct Xfile {
	int type;
	union {
		Pokemon *p;
		Invent *i;
		Trainer *tr;
	};
};

static void
fsread(Req *r)
{
	Xfile *f;
	char buf[8192];
	uchar str[64];
	Pokedat pd;
	char *p, *e;

	f = r->fid->file->aux;
	p = buf;
	e = buf + sizeof buf;

	switch(f->type){
	case Qtrainer:
		gen3pkstr(str, f->tr->name, sizeof f->tr->name);
		seprint(p, e, "%s\t%d\t%d\n", (char*)str, f->tr->id, f->tr->secretid);
		break;
	case Qpokemon:
		decryptpokemon(&pd, f->p);
		gen3pkstr(str, f->p->name, sizeof f->p->name);
		seprint(p, e, "%s\t%d\n", (char*)str, pd.g.species);
		break;
	}
	readstr(r, buf);
	respond(r, nil);
}

Srv fs = {
.read = fsread,
};

static void
usage(void)
{
	fprint(2, "usage: %s [-s srv] [-m mtpt] file.sav\n", argv0);
	exits("usage");
}

void
main(int argc, char **argv)
{
	int fd;
	int j;
	Xfile *xf;
	File *box;
	char *user;
	char *mtpt, *srvname;

	srvname = nil;
	mtpt = "/mnt/pse";
	ARGBEGIN{
	case 's':
		srvname = EARGF(usage());
		break;
	case 'm':
		mtpt = EARGF(usage());
		break;
	default:
		usage();
	}ARGEND;
	if(argc < 1)
		usage();

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	getgen3(fd, &gen3);

	user = getenv("user");
	fs.tree = alloctree(user, "sys", DMDIR|0555, nil);
	box = createfile(fs.tree->root, "box", user, DMDIR|0555, nil);

	xf = mallocz(sizeof *xf, 1);
	xf->type = Qtrainer;
	xf->tr = &gen3.tr;
	createfile(fs.tree->root, "trainer", user, 0444, xf);

	for(j = 0; j < 420; j++){
		if(gen3.pc.box[j].personality == 0 && gen3.pc.box[j].otid == 0)
			continue;
		xf = mallocz(sizeof *xf, 1);
		xf->type = Qpokemon;
		xf->p = &gen3.pc.box[j];
		createfile(box, smprint("%d", j), user, 0444, xf);
	}
	postmountsrv(&fs, srvname, mtpt, MREPL);
	exits(nil);
}
