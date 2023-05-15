#include <u.h>
#include <libc.h>
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
};

typedef struct Save Save;
struct Save{
	Section bank1[14];
	Section bank2[14];
	Section *active;
	Trainer tr;
	Invent inv;
};

Save save;

long getsection(Section*,uchar*);
long gettrainer(Trainer*,uchar*);
long getinvent(Invent*,uchar*);
long getpokedat(Pokedat*,uchar*);

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
	memcpy(buf + t[0], src->data, 12);
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

void
main(int argc, char **argv)
{
	int fd;
	int i, j;
	uchar buf[8192];
	Pokedat pd;

	ARGBEGIN{
	default:
		fprint(2, "usage");
		exits("usage");
	}ARGEND;
	if(argc < 1)
		sysfatal("usage");

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save.bank1 + i, buf);
		fprint(2, "%d\n", save.bank1[i].id);
	}

	seek(fd, 0xE000, 0);
	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save.bank2 + i, buf);
		fprint(2, "%d\n", save.bank2[i].id);
	}
	save.active = save.bank1[0].index > save.bank2[0].index ? save.bank1 : save.bank2;
	for(i = 0; i < 14; i++){
		switch(save.active[i].id){
		case STrainer:
			gettrainer(&save.tr, save.active[i].data);
			fprint(2, "%s\n", pkstr(save.tr.name, save.tr.name + 7));
			fprint(2, "%d %d %d\n", save.tr.hours, save.tr.min, save.tr.sec);
			fprint(2, "%d %d\n", save.tr.id, save.tr.secretid);
			fprint(2, "%d\n", save.tr.gamecode);
			break;
		case SInvent:
			getinvent(&save.inv, save.active[i].data + 0x34);
			fprint(2, "tamsz: %d\n", save.inv.teamsz);
			for(j = 0; j < save.inv.teamsz; j++){
				fprint(2, "%s\n", pkstr(save.inv.team[j].name, save.inv.team[j].name + 10));
				pd = decryptpokemon(save.inv.team+j);
				fprint(2, "%d\n", pd.g.species);
			}
			break;
		}
	}
}
