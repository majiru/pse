#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>
#include "gen3.h"

int poketab[24][4] = {
	{0, 12, 36, 24},
	{0, 12, 24, 36},
	{0, 36, 12, 24},
	{0, 36, 24, 12},
	{0, 24, 12, 36},
	{0, 24, 36, 12},
	{12, 0, 36, 24},
	{12, 0, 24, 36},
	{12, 36, 0, 24},
	{12, 36, 24, 0},
	{12, 24, 0, 36},
	{12, 24, 36, 0},
	{36, 0, 12, 24},
	{36, 0, 24, 12},
	{36, 12, 0, 24},
	{36, 12, 24, 0},
	{36, 24, 0, 12},
	{36, 24, 12, 0},
	{24, 0, 12, 36},
	{24, 0, 36, 12},
	{24, 12, 0, 36},
	{24, 12, 36, 0},
	{24, 36, 0, 12},
	{24, 36, 12, 0},
};

enum{
	STrainer,
	SInvent,
	SState,
	SMisc,
	SRiv,
	SPCA,
	SPCB,
	SPCC,
	SPCD,
	SPCE,
	SPCF,
	SPCG,
	SPCH,
	SPCI,
};

typedef struct Save Save;
struct Save{
	Section bank1[14];
	Section bank2[14];
	Section *active;
	Trainer tr;
	Invent inv;
	PC pc;
};

Save save;

uchar pcbuf[3968*8 + 2000];

long getsection(Section*,uchar*);
long gettrainer(Trainer*,uchar*);
long getinvent(Invent*,uchar*);
long getpokedat(Pokedat*,uchar*);
long getpc(PC*,uchar*);

#define PUT4(p, u) (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)

static void
cryptpokedat(uchar *b, u32int id, u32int personality)
{
	uchar key[4];
	int i;

	PUT4(key, id^personality);
	for(i = 0; i < 12*4; i++)
		b[i] = b[i] ^ key[i%4];
}

static Pokedat
decryptpokemon(Pokemon *src)
{
	uchar buf[12*4];
	Pokedat ret;
	int *t;

	t = poketab[src->personality % nelem(poketab)];
	memcpy(buf + t[0], src->data + 0, 12);
	memcpy(buf + t[1], src->data + 12, 12);
	memcpy(buf + t[2], src->data + 24, 12);
	memcpy(buf + t[3], src->data + 36, 12);
	cryptpokedat(buf, src->otid, src->personality);
	assert(getpokedat(&ret, buf) == sizeof buf);
	return ret;
}

static char*
pkstr(uchar *p, uchar *e)
{
	char out[16];
	char *d;

	for(d = out; p < e && *p != 0xFF; p++){
		if(*p >= 0xA1 && *p <= 0xAA)
			*d++ = '0' + (*p - 0xA1);
		else if(*p >= 0xBB && *p <= 0xD4)
			*d++ = 'A' + (*p - 0xBB);
		else if(*p >= 0xD5 && *p <= 0xEE)
			*d++ = 'a' + (*p - 0xD5);
	}
	*d = 0;
	return strdup(out);
}

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
	Pokedat pd;
	char *p, *e;

	f = r->fid->file->aux;
	p = buf;
	e = buf + sizeof buf;

	switch(f->type){
	case Qtrainer:
		seprint(p, e, "%s\t%d\t%d\n", pkstr(f->tr->name, f->tr->name + 7), f->tr->id, f->tr->secretid);
		break;
	case Qpokemon:
		pd = decryptpokemon(f->p);
		seprint(p, e, "%s\t%d\n", pkstr(f->p->name, f->p->name + 10), pd.g.species);
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
	int i, j;
	uchar buf[8192];
	Xfile *xf;
	File *box;
	char *user;
	char *mtpt, *srvname;

	srvname = nil;
	mtpt = "/mnt/gen3";
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

	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save.bank1 + i, buf);
	}

	seek(fd, 0xE000, 0);
	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save.bank2 + i, buf);
	}
	save.active = save.bank1[0].index > save.bank2[0].index ? save.bank1 : save.bank2;

	user = getenv("user");
	fs.tree = alloctree(user, "sys", DMDIR|0555, nil);
	box = createfile(fs.tree->root, "box", user, DMDIR|0555, nil);

	for(i = 0; i < 14; i++){
		switch(save.active[i].id){
		case STrainer:
			gettrainer(&save.tr, save.active[i].data);
			xf = mallocz(sizeof *xf, 1);
			xf->type = Qtrainer;
			xf->tr = &save.tr;
			createfile(fs.tree->root, "trainer", user, 0444, xf);
			break;
		case SInvent:
			getinvent(&save.inv, save.active[i].data + 0x34);
			break;
		case SState: case SMisc: case SRiv:
			break;
		default:
			j = save.active[i].id - SPCA;
			if(j >= 0 && j < 8)
				memcpy(pcbuf + j * 3968, save.active[i].data, 3968);
			else if(j == 8)
				memcpy(pcbuf + 8 * 3968, save.active[i].data, 2000);
			else
				sysfatal("invalid section");
		}
	}

	getpc(&save.pc, pcbuf);
	for(j = 0; j < 420; j++){
		if(save.pc.box[j].personality == 0 && save.pc.box[j].otid == 0)
			continue;
		xf = mallocz(sizeof *xf, 1);
		xf->type = Qpokemon;
		xf->p = &save.pc.box[j];
		createfile(box, smprint("%d", j), user, 0444, xf);
	}
	postmountsrv(&fs, srvname, mtpt, MREPL);
	exits(nil);
}
