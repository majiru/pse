#include <u.h>
#include <libc.h>
#include "colodat.h"

#define GET2(p) (u16int)(p)[1] | (u16int)(p)[0]<<8
#define PUT2(p, u) (p)[0] = (u)>>8, (p)[1] = (u)
#define GET3(p) (u32int)(p)[2] | (u32int)(p)[1]<<8 | (u32int)(p)[0]<<16
#define PUT3(p, u) (p)[0] = (u)>>16, (p)[1] = (u)>>8, (p)[2] = (u)
#define GET4(p) (u32int)(p)[3] | (u32int)(p)[2]<<8 | (u32int)(p)[1]<<16 | (u32int)(p)[0]<<24
#define PUT4(p, u) (p)[0] = (u)>>24, (p)[1] = (u)>>16, (p)[2] = (u)>>8, (p)[3] = (u)
#define GET5(p) (u64int)(p)[4] | (u64int)(p)[3]<<8 | (u64int)(p)[2]<<16 | (u64int)(p)[1]<<24 | (u64int)(p)[0]<<32
#define PUT5(p, u) (p)[0] = (u)>>32, (p)[1] = (u)>>24, (p)[2] = (u)>>16, (p)[3] = (u)>>8, (p)[4] = (u)
#define GET6(p) (u64int)(p)[5] | (u64int)(p)[4]<<8 | (u64int)(p)[3]<<16 | (u64int)(p)[2]<<24 | (u64int)(p)[1]<<32 | (u64int)(p)[0]<<40
#define PUT6(p, u) (p)[0] = (u)>>40, (p)[1] = (u)>>32, (p)[2] = (u)>>24, (p)[3] = (u)>>16, (p)[4] = (u)>>8, (p)[5] = (u)
#define GET7(p) (u64int)(p)[6] | (u64int)(p)[5]<<8 | (u64int)(p)[4]<<16 | (u64int)(p)[3]<<24 | (u64int)(p)[2]<<32 | (u64int)(p)[1]<<40 | (u64int)(p)[0]<<48
#define PUT7(p, u) (p)[0] = (u)>>48, (p)[1] = (u)>>40, (p)[2] = (u)>>32, (p)[3] = (u)>>24, (p)[4] = (u)>>16, (p)[5] = (u)>>8, (p)[6] = (u)
#define GET8(p) (u64int)(p)[7] | (u64int)(p)[6]<<8 | (u64int)(p)[5]<<16 | (u64int)(p)[4]<<24 | (u64int)(p)[3]<<32 | (u64int)(p)[2]<<40 | (u64int)(p)[1]<<48 | (u64int)(p)[0]<<56
#define PUT8(p, u) (p)[0] = (u)>>56, (p)[1] = (u)>>48, (p)[2] = (u)>>40, (p)[3] = (u)>>32, (p)[4] = (u)>>24, (p)[5] = (u)>>16, (p)[6] = (u)>>8, (p)[7] = (u)

long
getmovecolo(Movecolo *ret, uchar *data)
{
	long n;

	n = 0;
	ret->id = GET2(data+n);
	n += 2;
	ret->pp = data[n];
	n += 1;
	ret->up = data[n];
	n += 1;
	return n;
}

long
putmovecolo(uchar *dst, Movecolo *src)
{
	long n;

	n = 0;
	PUT2(dst+n, src->id);
	n += 2;
	dst[n] = src->pp;
	n += 1;
	dst[n] = src->up;
	n += 1;
	return n;
}

long
getstatcolo(Statcolo *ret, uchar *data)
{
	long n;

	n = 0;
	ret->hp = GET2(data+n);
	n += 2;
	ret->atk = GET2(data+n);
	n += 2;
	ret->def = GET2(data+n);
	n += 2;
	ret->spa = GET2(data+n);
	n += 2;
	ret->spd = GET2(data+n);
	n += 2;
	ret->spe = GET2(data+n);
	n += 2;
	return n;
}

long
putstatcolo(uchar *dst, Statcolo *src)
{
	long n;

	n = 0;
	PUT2(dst+n, src->hp);
	n += 2;
	PUT2(dst+n, src->atk);
	n += 2;
	PUT2(dst+n, src->def);
	n += 2;
	PUT2(dst+n, src->spa);
	n += 2;
	PUT2(dst+n, src->spd);
	n += 2;
	PUT2(dst+n, src->spe);
	n += 2;
	return n;
}

long
getpokemoncolo(Pokemoncolo *ret, uchar *data)
{
	long n;

	n = 0;
	ret->species = GET2(data+n);
	n += 2;
	ret->pad0 = GET2(data+n);
	n += 2;
	ret->pid = GET4(data+n);
	n += 4;
	ret->version = data[n];
	n += 1;
	ret->curregion = data[n];
	n += 1;
	ret->oriregion = data[n];
	n += 1;
	ret->lang = data[n];
	n += 1;
	ret->metloc = GET2(data+n);
	n += 2;
	ret->metlvl = data[n];
	n += 1;
	ret->ball = data[n];
	n += 1;
	ret->otgender = data[n];
	n += 1;
	memcpy(ret->pad3, data+n, 3);
	n += 3;
	ret->otid = GET2(data+n);
	n += 2;
	ret->otsecretid = GET2(data+n);
	n += 2;
	memcpy(ret->otname, data+n, 22);
	n += 22;
	memcpy(ret->name, data+n, 22);
	n += 22;
	memcpy(ret->namecopy, data+n, 22);
	n += 22;
	ret->pad1 = GET2(data+n);
	n += 2;
	ret->exp = GET4(data+n);
	n += 4;
	ret->statlvl = data[n];
	n += 1;
	memcpy(ret->battle, data+n, 23);
	n += 23;
	n += getmovecolo(&ret->moves[0], data+n);
	n += getmovecolo(&ret->moves[1], data+n);
	n += getmovecolo(&ret->moves[2], data+n);
	n += getmovecolo(&ret->moves[3], data+n);
	ret->item = GET2(data+n);
	n += 2;
	memcpy(ret->derived, data+n, 14);
	n += 14;
	n += getstatcolo(&ret->ev, data+n);
	n += getstatcolo(&ret->iv, data+n);
	ret->friendship = GET2(data+n);
	n += 2;
	memcpy(ret->contest, data+n, 11);
	n += 11;
	memcpy(ret->ribbon, data+n, 13);
	n += 13;
	ret->pkrs = data[n];
	n += 1;
	ret->isegg = data[n];
	n += 1;
	ret->ability = data[n];
	n += 1;
	ret->valid = data[n];
	n += 1;
	memcpy(ret->pad2, data+n, 9);
	n += 9;
	ret->slot = data[n];
	n += 1;
	ret->shadowid = GET2(data+n);
	n += 2;
	ret->pad4 = GET2(data+n);
	n += 2;
	ret->purification = GET4(data+n);
	n += 4;
	memcpy(ret->extra, data+n, 88);
	n += 88;
	return n;
}

long
putpokemoncolo(uchar *dst, Pokemoncolo *src)
{
	long n;

	n = 0;
	PUT2(dst+n, src->species);
	n += 2;
	PUT2(dst+n, src->pad0);
	n += 2;
	PUT4(dst+n, src->pid);
	n += 4;
	dst[n] = src->version;
	n += 1;
	dst[n] = src->curregion;
	n += 1;
	dst[n] = src->oriregion;
	n += 1;
	dst[n] = src->lang;
	n += 1;
	PUT2(dst+n, src->metloc);
	n += 2;
	dst[n] = src->metlvl;
	n += 1;
	dst[n] = src->ball;
	n += 1;
	dst[n] = src->otgender;
	n += 1;
	memcpy(dst+n, src->pad3, 3);
	n += 3;
	PUT2(dst+n, src->otid);
	n += 2;
	PUT2(dst+n, src->otsecretid);
	n += 2;
	memcpy(dst+n, src->otname, 22);
	n += 22;
	memcpy(dst+n, src->name, 22);
	n += 22;
	memcpy(dst+n, src->namecopy, 22);
	n += 22;
	PUT2(dst+n, src->pad1);
	n += 2;
	PUT4(dst+n, src->exp);
	n += 4;
	dst[n] = src->statlvl;
	n += 1;
	memcpy(dst+n, src->battle, 23);
	n += 23;
	n += putmovecolo(dst+n, &src->moves[0]);
	n += putmovecolo(dst+n, &src->moves[1]);
	n += putmovecolo(dst+n, &src->moves[2]);
	n += putmovecolo(dst+n, &src->moves[3]);
	PUT2(dst+n, src->item);
	n += 2;
	memcpy(dst+n, src->derived, 14);
	n += 14;
	n += putstatcolo(dst+n, &src->ev);
	n += putstatcolo(dst+n, &src->iv);
	PUT2(dst+n, src->friendship);
	n += 2;
	memcpy(dst+n, src->contest, 11);
	n += 11;
	memcpy(dst+n, src->ribbon, 13);
	n += 13;
	dst[n] = src->pkrs;
	n += 1;
	dst[n] = src->isegg;
	n += 1;
	dst[n] = src->ability;
	n += 1;
	dst[n] = src->valid;
	n += 1;
	memcpy(dst+n, src->pad2, 9);
	n += 9;
	dst[n] = src->slot;
	n += 1;
	PUT2(dst+n, src->shadowid);
	n += 2;
	PUT2(dst+n, src->pad4);
	n += 2;
	PUT4(dst+n, src->purification);
	n += 4;
	memcpy(dst+n, src->extra, 88);
	n += 88;
	return n;
}

long
gettrainercolo(Trainercolo *ret, uchar *data)
{
	long n;

	n = 0;
	memcpy(ret->name, data+n, 20);
	n += 20;
	memcpy(ret->namecopy, data+n, 20);
	n += 20;
	ret->pad0 = GET4(data+n);
	n += 4;
	ret->id = GET2(data+n);
	n += 2;
	ret->secretid = GET2(data+n);
	n += 2;
	return n;
}

long
puttrainercolo(uchar *dst, Trainercolo *src)
{
	long n;

	n = 0;
	memcpy(dst+n, src->name, 20);
	n += 20;
	memcpy(dst+n, src->namecopy, 20);
	n += 20;
	PUT4(dst+n, src->pad0);
	n += 4;
	PUT2(dst+n, src->id);
	n += 2;
	PUT2(dst+n, src->secretid);
	n += 2;
	return n;
}

