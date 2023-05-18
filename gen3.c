#include <u.h>
#include <libc.h>
#include "gen3dat.h"
#include "gen3.h"

char *gen3gnametab[] = {
	[GRS] "Ruby/Sapphire",
	[GFRLG] "Fire Red/Leaf Green",
	[GEM] "Emerald",
};

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

void
decryptpokemon(Pokedat *dst, Pokemon *src)
{
	uchar buf[12*4];
	int *t;

	t = poketab[src->personality % nelem(poketab)];
	memcpy(buf + t[0], src->data + 0, 12);
	memcpy(buf + t[1], src->data + 12, 12);
	memcpy(buf + t[2], src->data + 24, 12);
	memcpy(buf + t[3], src->data + 36, 12);
	cryptpokedat(buf, src->otid|(src->otsecretid<<16), src->personality);
	assert(getpokedat(dst, buf) == sizeof buf);
}

int
gen3shiny(Pokemon *p)
{
	u16int v;

	v = p->otid ^ p->otsecretid ^ (u16int)(p->personality>>16) ^ (u16int)(p->personality);
	return v < 8;
}

void
gen3pkstr(uchar *d, uchar *p, int n)
{
	uchar *e;

	for(e = p+n; p<e && *p != 0xFF; p++){
		if(*p >= 0xA1 && *p <= 0xAA)
			*d++ = '0' + (*p - 0xA1);
		else if(*p >= 0xBB && *p <= 0xD4)
			*d++ = 'A' + (*p - 0xBB);
		else if(*p >= 0xD5 && *p <= 0xEE)
			*d++ = 'a' + (*p - 0xD5);
	}
	*d = 0;
}

int
gen3strfmt(Fmt *f)
{
	uchar *p;
	long n;
	uchar buf[32];

	p = va_arg(f->args, uchar*);
	n = va_arg(f->args, long);
	gen3pkstr(buf, p, n);
	return fmtprint(f, "%s", (char*)buf);
}

void
getgen3iv(Gen3iv *dst, u32int src)
{
	dst->hp = (src & 0x1f);
	src >>= 5;
	dst->atk = (src & 0x1f);
	src >>= 5;
	dst->def = (src & 0x1f);
	src >>= 5;
	dst->spe = (src & 0x1f);
	src >>= 5;
	dst->spatk = (src & 0x1f);
	src >>= 5;
	dst->spdef = (src & 0x1f);
	src >>= 5;

	dst->egg = (src & 0x1);
	src >>= 1;
	dst->ability = (src & 0x1);
}

void
getgen3(int fd, Gen3 *save)
{
	int i, j;
	long off;
	uchar buf[8192];

	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save->bank1 + i, buf);
	}
	seek(fd, 0xE000, 0);
	for(i = 0; i < 14; i++){
		if(readn(fd, buf, 4096) != 4096)
			sysfatal("unexpected eof");
		getsection(save->bank2 + i, buf);
	}
	save->active = save->bank1[0].index > save->bank2[0].index ? save->bank1 : save->bank2;

	for(i = 0; i < 14; i++){
		switch(save->active[i].id){
		case STrainer:
			gettrainer(&save->tr, save->active[i].data);
			switch(save->tr.gamecode){
			case 0:
				save->type = GRS;
				break;
			case 1:
				save->type = GFRLG;
				break;
			default:
				save->type = GEM;
				break;
			}
			break;
		case SInvent:
			switch(save->type){
			default:
			case GEM:
			case GRS:
				off = 0x234;
				break;
			case GFRLG:
				off = 0x34;
				break;
			}
			getinvent(&save->inv, save->active[i].data + off);
			break;
		case SState: case SMisc: case SRiv:
			break;
		default:
			j = save->active[i].id - SPCA;
			if(j >= 0 && j < 8)
				memcpy(save->pcbuf + j * 3968, save->active[i].data, 3968);
			else if(j == 8)
				memcpy(save->pcbuf + 8 * 3968, save->active[i].data, 2000);
			else
				sysfatal("invalid section");
		}
	}

	getpc(&save->pc, save->pcbuf);
}

extern int gen3speciestab[];

int
getgen3dex(u16int species)
{

	return gen3speciestab[species]-1;
}
