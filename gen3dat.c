#include <u.h>
#include <libc.h>
#include "gen3.h"

#define GET2(p) (u16int)(p)[0] | (u16int)(p)[1]<<8
#define PUT2(p, u) (p)[1] = (u)>>8, (p)[0] = (u)
#define GET3(p) (u32int)(p)[0] | (u32int)(p)[1]<<8 | (u32int)(p)[2]<<16
#define PUT3(p, u) (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)
#define GET4(p) (u32int)(p)[0] | (u32int)(p)[1]<<8 | (u32int)(p)[2]<<16 | (u32int)(p)[3]<<24
#define PUT4(p, u) (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)
#define GET5(p) (u64int)(p)[0] | (u64int)(p)[1]<<8 | (u64int)(p)[2]<<16 | (u64int)(p)[3]<<24 | (u64int)(p)[4]<<32
#define PUT5(p, u) (p)[4] = (u)>>32, (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)
#define GET6(p) (u64int)(p)[0] | (u64int)(p)[1]<<8 | (u64int)(p)[2]<<16 | (u64int)(p)[3]<<24 | (u64int)(p)[4]<<32 | (u64int)(p)[5]<<40
#define PUT6(p, u) (p)[5] = (u)>>40, (p)[4] = (u)>>32, (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)
#define GET7(p) (u64int)(p)[0] | (u64int)(p)[1]<<8 | (u64int)(p)[2]<<16 | (u64int)(p)[3]<<24 | (u64int)(p)[4]<<32 | (u64int)(p)[5]<<40 | (u64int)(p)[6]<<48
#define PUT7(p, u) (p)[6] = (u)>>48, (p)[5] = (u)>>40, (p)[4] = (u)>>32, (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)
#define GET8(p) (u64int)(p)[0] | (u64int)(p)[1]<<8 | (u64int)(p)[2]<<16 | (u64int)(p)[3]<<24 | (u64int)(p)[4]<<32 | (u64int)(p)[5]<<40 | (u64int)(p)[6]<<48 | (u64int)(p)[7]<<56
#define PUT8(p, u) (p)[7] = (u)>>56, (p)[6] = (u)>>48, (p)[5] = (u)>>40, (p)[4] = (u)>>32, (p)[3] = (u)>>24, (p)[2] = (u)>>16, (p)[1] = (u)>>8, (p)[0] = (u)

long
getsection(Section *ret, uchar *data)
{
	long n;

	n = 0;
	memcpy(ret->data, data+n, 4084);
	n += 4084;
	ret->id = GET2(data+n);
	n += 2;
	ret->chk = GET2(data+n);
	n += 2;
	ret->pad = GET4(data+n);
	n += 4;
	ret->index = GET4(data+n);
	n += 4;
	return n;
}

long
putsection(uchar *dst, Section *src)
{
	long n;

	n = 0;
	memcpy(dst+n, src->data, 4084);
	n += 4084;
	PUT2(dst+n, src->id);
	n += 2;
	PUT2(dst+n, src->chk);
	n += 2;
	PUT4(dst+n, src->pad);
	n += 4;
	PUT4(dst+n, src->index);
	n += 4;
	return n;
}

long
gettrainer(Trainer *ret, uchar *data)
{
	long n;

	n = 0;
	memcpy(ret->name, data+n, 7);
	n += 7;
	ret->gender = data[n];
	n += 1;
	ret->pad0 = GET2(data+n);
	n += 2;
	ret->id = GET2(data+n);
	n += 2;
	ret->secretid = GET2(data+n);
	n += 2;
	ret->hours = GET2(data+n);
	n += 2;
	ret->min = data[n];
	n += 1;
	ret->sec = data[n];
	n += 1;
	ret->frame = data[n];
	n += 1;
	ret->button = data[n];
	n += 1;
	ret->text = data[n];
	n += 1;
	ret->misc = data[n];
	n += 1;
	memcpy(ret->pad1, data+n, 150);
	n += 150;
	ret->gamecode = GET4(data+n);
	n += 4;
	return n;
}

long
puttrainer(uchar *dst, Trainer *src)
{
	long n;

	n = 0;
	memcpy(dst+n, src->name, 7);
	n += 7;
	dst[n] = src->gender;
	n += 1;
	PUT2(dst+n, src->pad0);
	n += 2;
	PUT2(dst+n, src->id);
	n += 2;
	PUT2(dst+n, src->secretid);
	n += 2;
	PUT2(dst+n, src->hours);
	n += 2;
	dst[n] = src->min;
	n += 1;
	dst[n] = src->sec;
	n += 1;
	dst[n] = src->frame;
	n += 1;
	dst[n] = src->button;
	n += 1;
	dst[n] = src->text;
	n += 1;
	dst[n] = src->misc;
	n += 1;
	memcpy(dst+n, src->pad1, 150);
	n += 150;
	PUT4(dst+n, src->gamecode);
	n += 4;
	return n;
}

long
getpokemon(Pokemon *ret, uchar *data)
{
	long n;

	n = 0;
	ret->personality = GET4(data+n);
	n += 4;
	ret->otid = GET4(data+n);
	n += 4;
	memcpy(ret->name, data+n, 10);
	n += 10;
	ret->lang = GET2(data+n);
	n += 2;
	memcpy(ret->otname, data+n, 7);
	n += 7;
	ret->marks = data[n];
	n += 1;
	ret->chk = GET2(data+n);
	n += 2;
	ret->magic = GET2(data+n);
	n += 2;
	memcpy(ret->data, data+n, 48);
	n += 48;
	memcpy(ret->derived, data+n, 20);
	n += 20;
	return n;
}

long
putpokemon(uchar *dst, Pokemon *src)
{
	long n;

	n = 0;
	PUT4(dst+n, src->personality);
	n += 4;
	PUT4(dst+n, src->otid);
	n += 4;
	memcpy(dst+n, src->name, 10);
	n += 10;
	PUT2(dst+n, src->lang);
	n += 2;
	memcpy(dst+n, src->otname, 7);
	n += 7;
	dst[n] = src->marks;
	n += 1;
	PUT2(dst+n, src->chk);
	n += 2;
	PUT2(dst+n, src->magic);
	n += 2;
	memcpy(dst+n, src->data, 48);
	n += 48;
	memcpy(dst+n, src->derived, 20);
	n += 20;
	return n;
}

long
getinvent(Invent *ret, uchar *data)
{
	long n;

	n = 0;
	ret->teamsz = GET4(data+n);
	n += 4;
	n += getpokemon(&ret->p1, data+n);
	return n;
}

long
putinvent(uchar *dst, Invent *src)
{
	long n;

	n = 0;
	PUT4(dst+n, src->teamsz);
	n += 4;
	n += putpokemon(dst+n, &src->p1);
	return n;
}

