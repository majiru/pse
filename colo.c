#include <u.h>
#include <libc.h>
#include <mp.h>
#include <libsec.h>
#include "colodat.h"
#include "gen3dat.h"
#include "pse.h"

#define GET2(p) (u16int)(p)[1] | (u16int)(p)[0]<<8
#define GET4(p) (u32int)(p)[3] | (u32int)(p)[2]<<8 | (u32int)(p)[1]<<16 | (u32int)(p)[0]<<24

static void
dodecrypt(uchar *dst, uchar *src)
{
	uchar d1[SHA1dlen];
	uchar d2[SHA1dlen];
	uchar *s, *e, *dot;
	int i;

	memcpy(d2, src + Slotszcolo - SHA1dlen, SHA1dlen);
	for(i = 0; i < SHA1dlen; i++)
		d2[i] = ~d2[i];

	s = src + 0x18;
	dst += 0x18;
	e = src + Slotszcolo - (2*SHA1dlen);
	while(s < e){
		dot = s + SHA1dlen;
		if(dot > e)
			dot = e;
		sha1(s, dot - s, d1, nil);
		for(i = 0; i < dot - s; i++)
			*dst++ = s[i] ^ d2[i];
		memmove(d2, d1, dot - s);
		s = dot;
	}
}

void
getcolo(int fd, Colo *dst)
{
	uchar buf[0x6000];
	int i;
	uchar *dd;
	u32int max = 0;

	if(readn(fd, dst->gcihdr, sizeof dst->gcihdr) != sizeof dst->gcihdr)
		sysfatal("read: %r");
	if(readn(fd, buf, sizeof buf) != sizeof buf)
		sysfatal("read: %r");
	for(i = 0; i < nelem(dst->slots); i++)
		if(readn(fd, dst->slots[i], sizeof dst->slots[i]) != sizeof dst->slots[i])
			sysfatal("read: %r");
	for(i = 0; i < nelem(dst->index); i++){
		dst->index[i] = GET4(&dst->slots[i][4]);
		if(dst->index[i] > max)
			dst->active = dst->slots[i];
	}
	dodecrypt(dst->decrypted, dst->active);
	gettrainercolo(&dst->tr, dst->decrypted+0x78);
	dst->money = GET4(dst->decrypted+0xAFC);
	dst->coupons = GET4(dst->decrypted+0xB00);
	dd = dst->decrypted + 0x00B90;
	for(i = 0; i < 30*3; i++){
		getpokemoncolo(dst->pc+i, dd + ((i/30 + 1) * 0x14));
		dd += 312;
	}
}

#pragma varargck type "L" uchar*

/* colo strings are UTF16 but the game predates surrogate pairs */
static int
colostrfmt(Fmt *f)
{
	uchar *p;
	Rune r;
	int n;

	p = va_arg(f->args, uchar*);
	for(n = 0;; p +=2){
		r = GET2(p);
		if(r == 0)
			break;
		n += fmtprint(f, "%C", r);
	}
	return n;
}

static void
vinit(void)
{
	fmtinstall('L', colostrfmt);
}

extern int gen3speciestab[];
extern char *dexfiletab[];
extern char *movenametab[];

static int
vdex(void *v)
{
	Pokemoncolo *p;

	p = v;
	if(p->species == 0)
		return -1;
	return gen3speciestab[p->species]-1;
}

static int
vhdr(char *dst, char *e, void *v, int box)
{
	Colo *s;

	s = v;
	dst = seprint(dst, e, "Name: %L  ID: %d  Secret ID: %d\n", s->tr.name, s->tr.id, s->tr.secretid);
	dst = seprint(dst, e, "Game: Colosseum  Money: %ud  Coupons: %ud\n", s->money, s->coupons);
	seprint(dst, e, "Box %d: %L\n", box+1, s->decrypted + 0x00B90 + box*0x24a4);
	return 0;
}

static int
vbody(char *dst, char *e, void *v)
{
	Pokemoncolo *p;

	p = v;
	dst = seprint(dst, e, "Name: %L\n", p->name);
	dst = seprint(dst, e, "OT Name: %L  OT ID: %ud  OT Secret ID: %d\n", p->otname, p->otid, p->otsecretid);
	dst = seprint(dst, e, "National Dex: %d\n", gen3speciestab[p->species]-1);
	dst = seprint(dst, e, "Exp: %d\n", p->exp);
	dst = seprint(dst, e, "Move 1: %s  Move 2: %s\n", movenametab[p->moves[0].id], movenametab[p->moves[1].id]);
	dst = seprint(dst, e, "Move 3: %s  Move 4: %s\n", movenametab[p->moves[2].id], movenametab[p->moves[3].id]);
	dst = seprint(dst, e, "[EV] HP: %d  Atk: %d  Def: %d  SpA: %d  SpD: %d  Spe: %d\n", p->ev.hp, p->ev.atk, p->ev.def, p->ev.spa, p->ev.spd, p->ev.spe);
	seprint(dst, e, "[IV] HP: %d  Atk: %d  Def: %d  SpA: %d  SpD: %d  Spe: %d\n", p->iv.hp, p->iv.atk, p->iv.def, p->iv.spa, p->iv.spd, p->iv.spe);
	return 0;
}

static void*
vbox(int box, int i, void *v)
{
	Colo *c;

	c = v;
	return c->pc + box*30 + i;
}

View vcolo = {
	vinit,
	vhdr,
	vdex,
	vbody,
	vbox,
};
