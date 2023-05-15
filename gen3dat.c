#include <u.h>
#include <libc.h>
#include "gen3dat.h"

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
getblockg(Blockg *ret, uchar *data)
{
	long n;

	n = 0;
	ret->species = GET2(data+n);
	n += 2;
	ret->item = GET2(data+n);
	n += 2;
	ret->exp = GET4(data+n);
	n += 4;
	ret->ppbonus = data[n];
	n += 1;
	ret->friendship = data[n];
	n += 1;
	ret->magic = GET2(data+n);
	n += 2;
	return n;
}

long
putblockg(uchar *dst, Blockg *src)
{
	long n;

	n = 0;
	PUT2(dst+n, src->species);
	n += 2;
	PUT2(dst+n, src->item);
	n += 2;
	PUT4(dst+n, src->exp);
	n += 4;
	dst[n] = src->ppbonus;
	n += 1;
	dst[n] = src->friendship;
	n += 1;
	PUT2(dst+n, src->magic);
	n += 2;
	return n;
}

long
getblocka(Blocka *ret, uchar *data)
{
	long n;

	n = 0;
	ret->move1 = GET2(data+n);
	n += 2;
	ret->move2 = GET2(data+n);
	n += 2;
	ret->move3 = GET2(data+n);
	n += 2;
	ret->move4 = GET2(data+n);
	n += 2;
	memcpy(ret->pp, data+n, 4);
	n += 4;
	return n;
}

long
putblocka(uchar *dst, Blocka *src)
{
	long n;

	n = 0;
	PUT2(dst+n, src->move1);
	n += 2;
	PUT2(dst+n, src->move2);
	n += 2;
	PUT2(dst+n, src->move3);
	n += 2;
	PUT2(dst+n, src->move4);
	n += 2;
	memcpy(dst+n, src->pp, 4);
	n += 4;
	return n;
}

long
getblockm(Blockm *ret, uchar *data)
{
	long n;

	n = 0;
	ret->pokerus = data[n];
	n += 1;
	ret->met = data[n];
	n += 1;
	ret->origins = GET2(data+n);
	n += 2;
	ret->iv = GET4(data+n);
	n += 4;
	ret->ribbions = GET4(data+n);
	n += 4;
	return n;
}

long
putblockm(uchar *dst, Blockm *src)
{
	long n;

	n = 0;
	dst[n] = src->pokerus;
	n += 1;
	dst[n] = src->met;
	n += 1;
	PUT2(dst+n, src->origins);
	n += 2;
	PUT4(dst+n, src->iv);
	n += 4;
	PUT4(dst+n, src->ribbions);
	n += 4;
	return n;
}

long
getblocke(Blocke *ret, uchar *data)
{
	long n;

	n = 0;
	ret->hp = data[n];
	n += 1;
	ret->atk = data[n];
	n += 1;
	ret->def = data[n];
	n += 1;
	ret->spd = data[n];
	n += 1;
	ret->spatk = data[n];
	n += 1;
	ret->spdef = data[n];
	n += 1;
	ret->cool = data[n];
	n += 1;
	ret->beauty = data[n];
	n += 1;
	ret->cute = data[n];
	n += 1;
	ret->smart = data[n];
	n += 1;
	ret->tough = data[n];
	n += 1;
	ret->feel = data[n];
	n += 1;
	return n;
}

long
putblocke(uchar *dst, Blocke *src)
{
	long n;

	n = 0;
	dst[n] = src->hp;
	n += 1;
	dst[n] = src->atk;
	n += 1;
	dst[n] = src->def;
	n += 1;
	dst[n] = src->spd;
	n += 1;
	dst[n] = src->spatk;
	n += 1;
	dst[n] = src->spdef;
	n += 1;
	dst[n] = src->cool;
	n += 1;
	dst[n] = src->beauty;
	n += 1;
	dst[n] = src->cute;
	n += 1;
	dst[n] = src->smart;
	n += 1;
	dst[n] = src->tough;
	n += 1;
	dst[n] = src->feel;
	n += 1;
	return n;
}

long
getpokedat(Pokedat *ret, uchar *data)
{
	long n;

	n = 0;
	n += getblockg(&ret->g, data+n);
	n += getblocka(&ret->a, data+n);
	n += getblockm(&ret->m, data+n);
	n += getblocke(&ret->e, data+n);
	return n;
}

long
putpokedat(uchar *dst, Pokedat *src)
{
	long n;

	n = 0;
	n += putblockg(dst+n, &src->g);
	n += putblocka(dst+n, &src->a);
	n += putblockm(dst+n, &src->m);
	n += putblocke(dst+n, &src->e);
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
	return n;
}

long
getinventpokemon(InventPokemon *ret, uchar *data)
{
	long n;

	n = 0;
	n += getpokemon(&ret->p, data+n);
	memcpy(ret->derived, data+n, 20);
	n += 20;
	return n;
}

long
putinventpokemon(uchar *dst, InventPokemon *src)
{
	long n;

	n = 0;
	n += putpokemon(dst+n, &src->p);
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
	n += getinventpokemon(&ret->team[0], data+n);
	n += getinventpokemon(&ret->team[1], data+n);
	n += getinventpokemon(&ret->team[2], data+n);
	n += getinventpokemon(&ret->team[3], data+n);
	n += getinventpokemon(&ret->team[4], data+n);
	n += getinventpokemon(&ret->team[5], data+n);
	ret->money = GET4(data+n);
	n += 4;
	ret->coins = GET2(data+n);
	n += 2;
	return n;
}

long
putinvent(uchar *dst, Invent *src)
{
	long n;

	n = 0;
	PUT4(dst+n, src->teamsz);
	n += 4;
	n += putinventpokemon(dst+n, &src->team[0]);
	n += putinventpokemon(dst+n, &src->team[1]);
	n += putinventpokemon(dst+n, &src->team[2]);
	n += putinventpokemon(dst+n, &src->team[3]);
	n += putinventpokemon(dst+n, &src->team[4]);
	n += putinventpokemon(dst+n, &src->team[5]);
	PUT4(dst+n, src->money);
	n += 4;
	PUT2(dst+n, src->coins);
	n += 2;
	return n;
}

long
getboxname(Boxname *ret, uchar *data)
{
	long n;

	n = 0;
	memcpy(ret->n, data+n, 9);
	n += 9;
	return n;
}

long
putboxname(uchar *dst, Boxname *src)
{
	long n;

	n = 0;
	memcpy(dst+n, src->n, 9);
	n += 9;
	return n;
}

long
getpc(PC *ret, uchar *data)
{
	long n;

	n = 0;
	ret->current = GET4(data+n);
	n += 4;
	n += getpokemon(&ret->box[0], data+n);
	n += getpokemon(&ret->box[1], data+n);
	n += getpokemon(&ret->box[2], data+n);
	n += getpokemon(&ret->box[3], data+n);
	n += getpokemon(&ret->box[4], data+n);
	n += getpokemon(&ret->box[5], data+n);
	n += getpokemon(&ret->box[6], data+n);
	n += getpokemon(&ret->box[7], data+n);
	n += getpokemon(&ret->box[8], data+n);
	n += getpokemon(&ret->box[9], data+n);
	n += getpokemon(&ret->box[10], data+n);
	n += getpokemon(&ret->box[11], data+n);
	n += getpokemon(&ret->box[12], data+n);
	n += getpokemon(&ret->box[13], data+n);
	n += getpokemon(&ret->box[14], data+n);
	n += getpokemon(&ret->box[15], data+n);
	n += getpokemon(&ret->box[16], data+n);
	n += getpokemon(&ret->box[17], data+n);
	n += getpokemon(&ret->box[18], data+n);
	n += getpokemon(&ret->box[19], data+n);
	n += getpokemon(&ret->box[20], data+n);
	n += getpokemon(&ret->box[21], data+n);
	n += getpokemon(&ret->box[22], data+n);
	n += getpokemon(&ret->box[23], data+n);
	n += getpokemon(&ret->box[24], data+n);
	n += getpokemon(&ret->box[25], data+n);
	n += getpokemon(&ret->box[26], data+n);
	n += getpokemon(&ret->box[27], data+n);
	n += getpokemon(&ret->box[28], data+n);
	n += getpokemon(&ret->box[29], data+n);
	n += getpokemon(&ret->box[30], data+n);
	n += getpokemon(&ret->box[31], data+n);
	n += getpokemon(&ret->box[32], data+n);
	n += getpokemon(&ret->box[33], data+n);
	n += getpokemon(&ret->box[34], data+n);
	n += getpokemon(&ret->box[35], data+n);
	n += getpokemon(&ret->box[36], data+n);
	n += getpokemon(&ret->box[37], data+n);
	n += getpokemon(&ret->box[38], data+n);
	n += getpokemon(&ret->box[39], data+n);
	n += getpokemon(&ret->box[40], data+n);
	n += getpokemon(&ret->box[41], data+n);
	n += getpokemon(&ret->box[42], data+n);
	n += getpokemon(&ret->box[43], data+n);
	n += getpokemon(&ret->box[44], data+n);
	n += getpokemon(&ret->box[45], data+n);
	n += getpokemon(&ret->box[46], data+n);
	n += getpokemon(&ret->box[47], data+n);
	n += getpokemon(&ret->box[48], data+n);
	n += getpokemon(&ret->box[49], data+n);
	n += getpokemon(&ret->box[50], data+n);
	n += getpokemon(&ret->box[51], data+n);
	n += getpokemon(&ret->box[52], data+n);
	n += getpokemon(&ret->box[53], data+n);
	n += getpokemon(&ret->box[54], data+n);
	n += getpokemon(&ret->box[55], data+n);
	n += getpokemon(&ret->box[56], data+n);
	n += getpokemon(&ret->box[57], data+n);
	n += getpokemon(&ret->box[58], data+n);
	n += getpokemon(&ret->box[59], data+n);
	n += getpokemon(&ret->box[60], data+n);
	n += getpokemon(&ret->box[61], data+n);
	n += getpokemon(&ret->box[62], data+n);
	n += getpokemon(&ret->box[63], data+n);
	n += getpokemon(&ret->box[64], data+n);
	n += getpokemon(&ret->box[65], data+n);
	n += getpokemon(&ret->box[66], data+n);
	n += getpokemon(&ret->box[67], data+n);
	n += getpokemon(&ret->box[68], data+n);
	n += getpokemon(&ret->box[69], data+n);
	n += getpokemon(&ret->box[70], data+n);
	n += getpokemon(&ret->box[71], data+n);
	n += getpokemon(&ret->box[72], data+n);
	n += getpokemon(&ret->box[73], data+n);
	n += getpokemon(&ret->box[74], data+n);
	n += getpokemon(&ret->box[75], data+n);
	n += getpokemon(&ret->box[76], data+n);
	n += getpokemon(&ret->box[77], data+n);
	n += getpokemon(&ret->box[78], data+n);
	n += getpokemon(&ret->box[79], data+n);
	n += getpokemon(&ret->box[80], data+n);
	n += getpokemon(&ret->box[81], data+n);
	n += getpokemon(&ret->box[82], data+n);
	n += getpokemon(&ret->box[83], data+n);
	n += getpokemon(&ret->box[84], data+n);
	n += getpokemon(&ret->box[85], data+n);
	n += getpokemon(&ret->box[86], data+n);
	n += getpokemon(&ret->box[87], data+n);
	n += getpokemon(&ret->box[88], data+n);
	n += getpokemon(&ret->box[89], data+n);
	n += getpokemon(&ret->box[90], data+n);
	n += getpokemon(&ret->box[91], data+n);
	n += getpokemon(&ret->box[92], data+n);
	n += getpokemon(&ret->box[93], data+n);
	n += getpokemon(&ret->box[94], data+n);
	n += getpokemon(&ret->box[95], data+n);
	n += getpokemon(&ret->box[96], data+n);
	n += getpokemon(&ret->box[97], data+n);
	n += getpokemon(&ret->box[98], data+n);
	n += getpokemon(&ret->box[99], data+n);
	n += getpokemon(&ret->box[100], data+n);
	n += getpokemon(&ret->box[101], data+n);
	n += getpokemon(&ret->box[102], data+n);
	n += getpokemon(&ret->box[103], data+n);
	n += getpokemon(&ret->box[104], data+n);
	n += getpokemon(&ret->box[105], data+n);
	n += getpokemon(&ret->box[106], data+n);
	n += getpokemon(&ret->box[107], data+n);
	n += getpokemon(&ret->box[108], data+n);
	n += getpokemon(&ret->box[109], data+n);
	n += getpokemon(&ret->box[110], data+n);
	n += getpokemon(&ret->box[111], data+n);
	n += getpokemon(&ret->box[112], data+n);
	n += getpokemon(&ret->box[113], data+n);
	n += getpokemon(&ret->box[114], data+n);
	n += getpokemon(&ret->box[115], data+n);
	n += getpokemon(&ret->box[116], data+n);
	n += getpokemon(&ret->box[117], data+n);
	n += getpokemon(&ret->box[118], data+n);
	n += getpokemon(&ret->box[119], data+n);
	n += getpokemon(&ret->box[120], data+n);
	n += getpokemon(&ret->box[121], data+n);
	n += getpokemon(&ret->box[122], data+n);
	n += getpokemon(&ret->box[123], data+n);
	n += getpokemon(&ret->box[124], data+n);
	n += getpokemon(&ret->box[125], data+n);
	n += getpokemon(&ret->box[126], data+n);
	n += getpokemon(&ret->box[127], data+n);
	n += getpokemon(&ret->box[128], data+n);
	n += getpokemon(&ret->box[129], data+n);
	n += getpokemon(&ret->box[130], data+n);
	n += getpokemon(&ret->box[131], data+n);
	n += getpokemon(&ret->box[132], data+n);
	n += getpokemon(&ret->box[133], data+n);
	n += getpokemon(&ret->box[134], data+n);
	n += getpokemon(&ret->box[135], data+n);
	n += getpokemon(&ret->box[136], data+n);
	n += getpokemon(&ret->box[137], data+n);
	n += getpokemon(&ret->box[138], data+n);
	n += getpokemon(&ret->box[139], data+n);
	n += getpokemon(&ret->box[140], data+n);
	n += getpokemon(&ret->box[141], data+n);
	n += getpokemon(&ret->box[142], data+n);
	n += getpokemon(&ret->box[143], data+n);
	n += getpokemon(&ret->box[144], data+n);
	n += getpokemon(&ret->box[145], data+n);
	n += getpokemon(&ret->box[146], data+n);
	n += getpokemon(&ret->box[147], data+n);
	n += getpokemon(&ret->box[148], data+n);
	n += getpokemon(&ret->box[149], data+n);
	n += getpokemon(&ret->box[150], data+n);
	n += getpokemon(&ret->box[151], data+n);
	n += getpokemon(&ret->box[152], data+n);
	n += getpokemon(&ret->box[153], data+n);
	n += getpokemon(&ret->box[154], data+n);
	n += getpokemon(&ret->box[155], data+n);
	n += getpokemon(&ret->box[156], data+n);
	n += getpokemon(&ret->box[157], data+n);
	n += getpokemon(&ret->box[158], data+n);
	n += getpokemon(&ret->box[159], data+n);
	n += getpokemon(&ret->box[160], data+n);
	n += getpokemon(&ret->box[161], data+n);
	n += getpokemon(&ret->box[162], data+n);
	n += getpokemon(&ret->box[163], data+n);
	n += getpokemon(&ret->box[164], data+n);
	n += getpokemon(&ret->box[165], data+n);
	n += getpokemon(&ret->box[166], data+n);
	n += getpokemon(&ret->box[167], data+n);
	n += getpokemon(&ret->box[168], data+n);
	n += getpokemon(&ret->box[169], data+n);
	n += getpokemon(&ret->box[170], data+n);
	n += getpokemon(&ret->box[171], data+n);
	n += getpokemon(&ret->box[172], data+n);
	n += getpokemon(&ret->box[173], data+n);
	n += getpokemon(&ret->box[174], data+n);
	n += getpokemon(&ret->box[175], data+n);
	n += getpokemon(&ret->box[176], data+n);
	n += getpokemon(&ret->box[177], data+n);
	n += getpokemon(&ret->box[178], data+n);
	n += getpokemon(&ret->box[179], data+n);
	n += getpokemon(&ret->box[180], data+n);
	n += getpokemon(&ret->box[181], data+n);
	n += getpokemon(&ret->box[182], data+n);
	n += getpokemon(&ret->box[183], data+n);
	n += getpokemon(&ret->box[184], data+n);
	n += getpokemon(&ret->box[185], data+n);
	n += getpokemon(&ret->box[186], data+n);
	n += getpokemon(&ret->box[187], data+n);
	n += getpokemon(&ret->box[188], data+n);
	n += getpokemon(&ret->box[189], data+n);
	n += getpokemon(&ret->box[190], data+n);
	n += getpokemon(&ret->box[191], data+n);
	n += getpokemon(&ret->box[192], data+n);
	n += getpokemon(&ret->box[193], data+n);
	n += getpokemon(&ret->box[194], data+n);
	n += getpokemon(&ret->box[195], data+n);
	n += getpokemon(&ret->box[196], data+n);
	n += getpokemon(&ret->box[197], data+n);
	n += getpokemon(&ret->box[198], data+n);
	n += getpokemon(&ret->box[199], data+n);
	n += getpokemon(&ret->box[200], data+n);
	n += getpokemon(&ret->box[201], data+n);
	n += getpokemon(&ret->box[202], data+n);
	n += getpokemon(&ret->box[203], data+n);
	n += getpokemon(&ret->box[204], data+n);
	n += getpokemon(&ret->box[205], data+n);
	n += getpokemon(&ret->box[206], data+n);
	n += getpokemon(&ret->box[207], data+n);
	n += getpokemon(&ret->box[208], data+n);
	n += getpokemon(&ret->box[209], data+n);
	n += getpokemon(&ret->box[210], data+n);
	n += getpokemon(&ret->box[211], data+n);
	n += getpokemon(&ret->box[212], data+n);
	n += getpokemon(&ret->box[213], data+n);
	n += getpokemon(&ret->box[214], data+n);
	n += getpokemon(&ret->box[215], data+n);
	n += getpokemon(&ret->box[216], data+n);
	n += getpokemon(&ret->box[217], data+n);
	n += getpokemon(&ret->box[218], data+n);
	n += getpokemon(&ret->box[219], data+n);
	n += getpokemon(&ret->box[220], data+n);
	n += getpokemon(&ret->box[221], data+n);
	n += getpokemon(&ret->box[222], data+n);
	n += getpokemon(&ret->box[223], data+n);
	n += getpokemon(&ret->box[224], data+n);
	n += getpokemon(&ret->box[225], data+n);
	n += getpokemon(&ret->box[226], data+n);
	n += getpokemon(&ret->box[227], data+n);
	n += getpokemon(&ret->box[228], data+n);
	n += getpokemon(&ret->box[229], data+n);
	n += getpokemon(&ret->box[230], data+n);
	n += getpokemon(&ret->box[231], data+n);
	n += getpokemon(&ret->box[232], data+n);
	n += getpokemon(&ret->box[233], data+n);
	n += getpokemon(&ret->box[234], data+n);
	n += getpokemon(&ret->box[235], data+n);
	n += getpokemon(&ret->box[236], data+n);
	n += getpokemon(&ret->box[237], data+n);
	n += getpokemon(&ret->box[238], data+n);
	n += getpokemon(&ret->box[239], data+n);
	n += getpokemon(&ret->box[240], data+n);
	n += getpokemon(&ret->box[241], data+n);
	n += getpokemon(&ret->box[242], data+n);
	n += getpokemon(&ret->box[243], data+n);
	n += getpokemon(&ret->box[244], data+n);
	n += getpokemon(&ret->box[245], data+n);
	n += getpokemon(&ret->box[246], data+n);
	n += getpokemon(&ret->box[247], data+n);
	n += getpokemon(&ret->box[248], data+n);
	n += getpokemon(&ret->box[249], data+n);
	n += getpokemon(&ret->box[250], data+n);
	n += getpokemon(&ret->box[251], data+n);
	n += getpokemon(&ret->box[252], data+n);
	n += getpokemon(&ret->box[253], data+n);
	n += getpokemon(&ret->box[254], data+n);
	n += getpokemon(&ret->box[255], data+n);
	n += getpokemon(&ret->box[256], data+n);
	n += getpokemon(&ret->box[257], data+n);
	n += getpokemon(&ret->box[258], data+n);
	n += getpokemon(&ret->box[259], data+n);
	n += getpokemon(&ret->box[260], data+n);
	n += getpokemon(&ret->box[261], data+n);
	n += getpokemon(&ret->box[262], data+n);
	n += getpokemon(&ret->box[263], data+n);
	n += getpokemon(&ret->box[264], data+n);
	n += getpokemon(&ret->box[265], data+n);
	n += getpokemon(&ret->box[266], data+n);
	n += getpokemon(&ret->box[267], data+n);
	n += getpokemon(&ret->box[268], data+n);
	n += getpokemon(&ret->box[269], data+n);
	n += getpokemon(&ret->box[270], data+n);
	n += getpokemon(&ret->box[271], data+n);
	n += getpokemon(&ret->box[272], data+n);
	n += getpokemon(&ret->box[273], data+n);
	n += getpokemon(&ret->box[274], data+n);
	n += getpokemon(&ret->box[275], data+n);
	n += getpokemon(&ret->box[276], data+n);
	n += getpokemon(&ret->box[277], data+n);
	n += getpokemon(&ret->box[278], data+n);
	n += getpokemon(&ret->box[279], data+n);
	n += getpokemon(&ret->box[280], data+n);
	n += getpokemon(&ret->box[281], data+n);
	n += getpokemon(&ret->box[282], data+n);
	n += getpokemon(&ret->box[283], data+n);
	n += getpokemon(&ret->box[284], data+n);
	n += getpokemon(&ret->box[285], data+n);
	n += getpokemon(&ret->box[286], data+n);
	n += getpokemon(&ret->box[287], data+n);
	n += getpokemon(&ret->box[288], data+n);
	n += getpokemon(&ret->box[289], data+n);
	n += getpokemon(&ret->box[290], data+n);
	n += getpokemon(&ret->box[291], data+n);
	n += getpokemon(&ret->box[292], data+n);
	n += getpokemon(&ret->box[293], data+n);
	n += getpokemon(&ret->box[294], data+n);
	n += getpokemon(&ret->box[295], data+n);
	n += getpokemon(&ret->box[296], data+n);
	n += getpokemon(&ret->box[297], data+n);
	n += getpokemon(&ret->box[298], data+n);
	n += getpokemon(&ret->box[299], data+n);
	n += getpokemon(&ret->box[300], data+n);
	n += getpokemon(&ret->box[301], data+n);
	n += getpokemon(&ret->box[302], data+n);
	n += getpokemon(&ret->box[303], data+n);
	n += getpokemon(&ret->box[304], data+n);
	n += getpokemon(&ret->box[305], data+n);
	n += getpokemon(&ret->box[306], data+n);
	n += getpokemon(&ret->box[307], data+n);
	n += getpokemon(&ret->box[308], data+n);
	n += getpokemon(&ret->box[309], data+n);
	n += getpokemon(&ret->box[310], data+n);
	n += getpokemon(&ret->box[311], data+n);
	n += getpokemon(&ret->box[312], data+n);
	n += getpokemon(&ret->box[313], data+n);
	n += getpokemon(&ret->box[314], data+n);
	n += getpokemon(&ret->box[315], data+n);
	n += getpokemon(&ret->box[316], data+n);
	n += getpokemon(&ret->box[317], data+n);
	n += getpokemon(&ret->box[318], data+n);
	n += getpokemon(&ret->box[319], data+n);
	n += getpokemon(&ret->box[320], data+n);
	n += getpokemon(&ret->box[321], data+n);
	n += getpokemon(&ret->box[322], data+n);
	n += getpokemon(&ret->box[323], data+n);
	n += getpokemon(&ret->box[324], data+n);
	n += getpokemon(&ret->box[325], data+n);
	n += getpokemon(&ret->box[326], data+n);
	n += getpokemon(&ret->box[327], data+n);
	n += getpokemon(&ret->box[328], data+n);
	n += getpokemon(&ret->box[329], data+n);
	n += getpokemon(&ret->box[330], data+n);
	n += getpokemon(&ret->box[331], data+n);
	n += getpokemon(&ret->box[332], data+n);
	n += getpokemon(&ret->box[333], data+n);
	n += getpokemon(&ret->box[334], data+n);
	n += getpokemon(&ret->box[335], data+n);
	n += getpokemon(&ret->box[336], data+n);
	n += getpokemon(&ret->box[337], data+n);
	n += getpokemon(&ret->box[338], data+n);
	n += getpokemon(&ret->box[339], data+n);
	n += getpokemon(&ret->box[340], data+n);
	n += getpokemon(&ret->box[341], data+n);
	n += getpokemon(&ret->box[342], data+n);
	n += getpokemon(&ret->box[343], data+n);
	n += getpokemon(&ret->box[344], data+n);
	n += getpokemon(&ret->box[345], data+n);
	n += getpokemon(&ret->box[346], data+n);
	n += getpokemon(&ret->box[347], data+n);
	n += getpokemon(&ret->box[348], data+n);
	n += getpokemon(&ret->box[349], data+n);
	n += getpokemon(&ret->box[350], data+n);
	n += getpokemon(&ret->box[351], data+n);
	n += getpokemon(&ret->box[352], data+n);
	n += getpokemon(&ret->box[353], data+n);
	n += getpokemon(&ret->box[354], data+n);
	n += getpokemon(&ret->box[355], data+n);
	n += getpokemon(&ret->box[356], data+n);
	n += getpokemon(&ret->box[357], data+n);
	n += getpokemon(&ret->box[358], data+n);
	n += getpokemon(&ret->box[359], data+n);
	n += getpokemon(&ret->box[360], data+n);
	n += getpokemon(&ret->box[361], data+n);
	n += getpokemon(&ret->box[362], data+n);
	n += getpokemon(&ret->box[363], data+n);
	n += getpokemon(&ret->box[364], data+n);
	n += getpokemon(&ret->box[365], data+n);
	n += getpokemon(&ret->box[366], data+n);
	n += getpokemon(&ret->box[367], data+n);
	n += getpokemon(&ret->box[368], data+n);
	n += getpokemon(&ret->box[369], data+n);
	n += getpokemon(&ret->box[370], data+n);
	n += getpokemon(&ret->box[371], data+n);
	n += getpokemon(&ret->box[372], data+n);
	n += getpokemon(&ret->box[373], data+n);
	n += getpokemon(&ret->box[374], data+n);
	n += getpokemon(&ret->box[375], data+n);
	n += getpokemon(&ret->box[376], data+n);
	n += getpokemon(&ret->box[377], data+n);
	n += getpokemon(&ret->box[378], data+n);
	n += getpokemon(&ret->box[379], data+n);
	n += getpokemon(&ret->box[380], data+n);
	n += getpokemon(&ret->box[381], data+n);
	n += getpokemon(&ret->box[382], data+n);
	n += getpokemon(&ret->box[383], data+n);
	n += getpokemon(&ret->box[384], data+n);
	n += getpokemon(&ret->box[385], data+n);
	n += getpokemon(&ret->box[386], data+n);
	n += getpokemon(&ret->box[387], data+n);
	n += getpokemon(&ret->box[388], data+n);
	n += getpokemon(&ret->box[389], data+n);
	n += getpokemon(&ret->box[390], data+n);
	n += getpokemon(&ret->box[391], data+n);
	n += getpokemon(&ret->box[392], data+n);
	n += getpokemon(&ret->box[393], data+n);
	n += getpokemon(&ret->box[394], data+n);
	n += getpokemon(&ret->box[395], data+n);
	n += getpokemon(&ret->box[396], data+n);
	n += getpokemon(&ret->box[397], data+n);
	n += getpokemon(&ret->box[398], data+n);
	n += getpokemon(&ret->box[399], data+n);
	n += getpokemon(&ret->box[400], data+n);
	n += getpokemon(&ret->box[401], data+n);
	n += getpokemon(&ret->box[402], data+n);
	n += getpokemon(&ret->box[403], data+n);
	n += getpokemon(&ret->box[404], data+n);
	n += getpokemon(&ret->box[405], data+n);
	n += getpokemon(&ret->box[406], data+n);
	n += getpokemon(&ret->box[407], data+n);
	n += getpokemon(&ret->box[408], data+n);
	n += getpokemon(&ret->box[409], data+n);
	n += getpokemon(&ret->box[410], data+n);
	n += getpokemon(&ret->box[411], data+n);
	n += getpokemon(&ret->box[412], data+n);
	n += getpokemon(&ret->box[413], data+n);
	n += getpokemon(&ret->box[414], data+n);
	n += getpokemon(&ret->box[415], data+n);
	n += getpokemon(&ret->box[416], data+n);
	n += getpokemon(&ret->box[417], data+n);
	n += getpokemon(&ret->box[418], data+n);
	n += getpokemon(&ret->box[419], data+n);
	n += getboxname(&ret->name[0], data+n);
	n += getboxname(&ret->name[1], data+n);
	n += getboxname(&ret->name[2], data+n);
	n += getboxname(&ret->name[3], data+n);
	n += getboxname(&ret->name[4], data+n);
	n += getboxname(&ret->name[5], data+n);
	n += getboxname(&ret->name[6], data+n);
	n += getboxname(&ret->name[7], data+n);
	n += getboxname(&ret->name[8], data+n);
	n += getboxname(&ret->name[9], data+n);
	n += getboxname(&ret->name[10], data+n);
	n += getboxname(&ret->name[11], data+n);
	n += getboxname(&ret->name[12], data+n);
	n += getboxname(&ret->name[13], data+n);
	memcpy(ret->wallpaper, data+n, 14);
	n += 14;
	return n;
}

long
putpc(uchar *dst, PC *src)
{
	long n;

	n = 0;
	PUT4(dst+n, src->current);
	n += 4;
	n += putpokemon(dst+n, &src->box[0]);
	n += putpokemon(dst+n, &src->box[1]);
	n += putpokemon(dst+n, &src->box[2]);
	n += putpokemon(dst+n, &src->box[3]);
	n += putpokemon(dst+n, &src->box[4]);
	n += putpokemon(dst+n, &src->box[5]);
	n += putpokemon(dst+n, &src->box[6]);
	n += putpokemon(dst+n, &src->box[7]);
	n += putpokemon(dst+n, &src->box[8]);
	n += putpokemon(dst+n, &src->box[9]);
	n += putpokemon(dst+n, &src->box[10]);
	n += putpokemon(dst+n, &src->box[11]);
	n += putpokemon(dst+n, &src->box[12]);
	n += putpokemon(dst+n, &src->box[13]);
	n += putpokemon(dst+n, &src->box[14]);
	n += putpokemon(dst+n, &src->box[15]);
	n += putpokemon(dst+n, &src->box[16]);
	n += putpokemon(dst+n, &src->box[17]);
	n += putpokemon(dst+n, &src->box[18]);
	n += putpokemon(dst+n, &src->box[19]);
	n += putpokemon(dst+n, &src->box[20]);
	n += putpokemon(dst+n, &src->box[21]);
	n += putpokemon(dst+n, &src->box[22]);
	n += putpokemon(dst+n, &src->box[23]);
	n += putpokemon(dst+n, &src->box[24]);
	n += putpokemon(dst+n, &src->box[25]);
	n += putpokemon(dst+n, &src->box[26]);
	n += putpokemon(dst+n, &src->box[27]);
	n += putpokemon(dst+n, &src->box[28]);
	n += putpokemon(dst+n, &src->box[29]);
	n += putpokemon(dst+n, &src->box[30]);
	n += putpokemon(dst+n, &src->box[31]);
	n += putpokemon(dst+n, &src->box[32]);
	n += putpokemon(dst+n, &src->box[33]);
	n += putpokemon(dst+n, &src->box[34]);
	n += putpokemon(dst+n, &src->box[35]);
	n += putpokemon(dst+n, &src->box[36]);
	n += putpokemon(dst+n, &src->box[37]);
	n += putpokemon(dst+n, &src->box[38]);
	n += putpokemon(dst+n, &src->box[39]);
	n += putpokemon(dst+n, &src->box[40]);
	n += putpokemon(dst+n, &src->box[41]);
	n += putpokemon(dst+n, &src->box[42]);
	n += putpokemon(dst+n, &src->box[43]);
	n += putpokemon(dst+n, &src->box[44]);
	n += putpokemon(dst+n, &src->box[45]);
	n += putpokemon(dst+n, &src->box[46]);
	n += putpokemon(dst+n, &src->box[47]);
	n += putpokemon(dst+n, &src->box[48]);
	n += putpokemon(dst+n, &src->box[49]);
	n += putpokemon(dst+n, &src->box[50]);
	n += putpokemon(dst+n, &src->box[51]);
	n += putpokemon(dst+n, &src->box[52]);
	n += putpokemon(dst+n, &src->box[53]);
	n += putpokemon(dst+n, &src->box[54]);
	n += putpokemon(dst+n, &src->box[55]);
	n += putpokemon(dst+n, &src->box[56]);
	n += putpokemon(dst+n, &src->box[57]);
	n += putpokemon(dst+n, &src->box[58]);
	n += putpokemon(dst+n, &src->box[59]);
	n += putpokemon(dst+n, &src->box[60]);
	n += putpokemon(dst+n, &src->box[61]);
	n += putpokemon(dst+n, &src->box[62]);
	n += putpokemon(dst+n, &src->box[63]);
	n += putpokemon(dst+n, &src->box[64]);
	n += putpokemon(dst+n, &src->box[65]);
	n += putpokemon(dst+n, &src->box[66]);
	n += putpokemon(dst+n, &src->box[67]);
	n += putpokemon(dst+n, &src->box[68]);
	n += putpokemon(dst+n, &src->box[69]);
	n += putpokemon(dst+n, &src->box[70]);
	n += putpokemon(dst+n, &src->box[71]);
	n += putpokemon(dst+n, &src->box[72]);
	n += putpokemon(dst+n, &src->box[73]);
	n += putpokemon(dst+n, &src->box[74]);
	n += putpokemon(dst+n, &src->box[75]);
	n += putpokemon(dst+n, &src->box[76]);
	n += putpokemon(dst+n, &src->box[77]);
	n += putpokemon(dst+n, &src->box[78]);
	n += putpokemon(dst+n, &src->box[79]);
	n += putpokemon(dst+n, &src->box[80]);
	n += putpokemon(dst+n, &src->box[81]);
	n += putpokemon(dst+n, &src->box[82]);
	n += putpokemon(dst+n, &src->box[83]);
	n += putpokemon(dst+n, &src->box[84]);
	n += putpokemon(dst+n, &src->box[85]);
	n += putpokemon(dst+n, &src->box[86]);
	n += putpokemon(dst+n, &src->box[87]);
	n += putpokemon(dst+n, &src->box[88]);
	n += putpokemon(dst+n, &src->box[89]);
	n += putpokemon(dst+n, &src->box[90]);
	n += putpokemon(dst+n, &src->box[91]);
	n += putpokemon(dst+n, &src->box[92]);
	n += putpokemon(dst+n, &src->box[93]);
	n += putpokemon(dst+n, &src->box[94]);
	n += putpokemon(dst+n, &src->box[95]);
	n += putpokemon(dst+n, &src->box[96]);
	n += putpokemon(dst+n, &src->box[97]);
	n += putpokemon(dst+n, &src->box[98]);
	n += putpokemon(dst+n, &src->box[99]);
	n += putpokemon(dst+n, &src->box[100]);
	n += putpokemon(dst+n, &src->box[101]);
	n += putpokemon(dst+n, &src->box[102]);
	n += putpokemon(dst+n, &src->box[103]);
	n += putpokemon(dst+n, &src->box[104]);
	n += putpokemon(dst+n, &src->box[105]);
	n += putpokemon(dst+n, &src->box[106]);
	n += putpokemon(dst+n, &src->box[107]);
	n += putpokemon(dst+n, &src->box[108]);
	n += putpokemon(dst+n, &src->box[109]);
	n += putpokemon(dst+n, &src->box[110]);
	n += putpokemon(dst+n, &src->box[111]);
	n += putpokemon(dst+n, &src->box[112]);
	n += putpokemon(dst+n, &src->box[113]);
	n += putpokemon(dst+n, &src->box[114]);
	n += putpokemon(dst+n, &src->box[115]);
	n += putpokemon(dst+n, &src->box[116]);
	n += putpokemon(dst+n, &src->box[117]);
	n += putpokemon(dst+n, &src->box[118]);
	n += putpokemon(dst+n, &src->box[119]);
	n += putpokemon(dst+n, &src->box[120]);
	n += putpokemon(dst+n, &src->box[121]);
	n += putpokemon(dst+n, &src->box[122]);
	n += putpokemon(dst+n, &src->box[123]);
	n += putpokemon(dst+n, &src->box[124]);
	n += putpokemon(dst+n, &src->box[125]);
	n += putpokemon(dst+n, &src->box[126]);
	n += putpokemon(dst+n, &src->box[127]);
	n += putpokemon(dst+n, &src->box[128]);
	n += putpokemon(dst+n, &src->box[129]);
	n += putpokemon(dst+n, &src->box[130]);
	n += putpokemon(dst+n, &src->box[131]);
	n += putpokemon(dst+n, &src->box[132]);
	n += putpokemon(dst+n, &src->box[133]);
	n += putpokemon(dst+n, &src->box[134]);
	n += putpokemon(dst+n, &src->box[135]);
	n += putpokemon(dst+n, &src->box[136]);
	n += putpokemon(dst+n, &src->box[137]);
	n += putpokemon(dst+n, &src->box[138]);
	n += putpokemon(dst+n, &src->box[139]);
	n += putpokemon(dst+n, &src->box[140]);
	n += putpokemon(dst+n, &src->box[141]);
	n += putpokemon(dst+n, &src->box[142]);
	n += putpokemon(dst+n, &src->box[143]);
	n += putpokemon(dst+n, &src->box[144]);
	n += putpokemon(dst+n, &src->box[145]);
	n += putpokemon(dst+n, &src->box[146]);
	n += putpokemon(dst+n, &src->box[147]);
	n += putpokemon(dst+n, &src->box[148]);
	n += putpokemon(dst+n, &src->box[149]);
	n += putpokemon(dst+n, &src->box[150]);
	n += putpokemon(dst+n, &src->box[151]);
	n += putpokemon(dst+n, &src->box[152]);
	n += putpokemon(dst+n, &src->box[153]);
	n += putpokemon(dst+n, &src->box[154]);
	n += putpokemon(dst+n, &src->box[155]);
	n += putpokemon(dst+n, &src->box[156]);
	n += putpokemon(dst+n, &src->box[157]);
	n += putpokemon(dst+n, &src->box[158]);
	n += putpokemon(dst+n, &src->box[159]);
	n += putpokemon(dst+n, &src->box[160]);
	n += putpokemon(dst+n, &src->box[161]);
	n += putpokemon(dst+n, &src->box[162]);
	n += putpokemon(dst+n, &src->box[163]);
	n += putpokemon(dst+n, &src->box[164]);
	n += putpokemon(dst+n, &src->box[165]);
	n += putpokemon(dst+n, &src->box[166]);
	n += putpokemon(dst+n, &src->box[167]);
	n += putpokemon(dst+n, &src->box[168]);
	n += putpokemon(dst+n, &src->box[169]);
	n += putpokemon(dst+n, &src->box[170]);
	n += putpokemon(dst+n, &src->box[171]);
	n += putpokemon(dst+n, &src->box[172]);
	n += putpokemon(dst+n, &src->box[173]);
	n += putpokemon(dst+n, &src->box[174]);
	n += putpokemon(dst+n, &src->box[175]);
	n += putpokemon(dst+n, &src->box[176]);
	n += putpokemon(dst+n, &src->box[177]);
	n += putpokemon(dst+n, &src->box[178]);
	n += putpokemon(dst+n, &src->box[179]);
	n += putpokemon(dst+n, &src->box[180]);
	n += putpokemon(dst+n, &src->box[181]);
	n += putpokemon(dst+n, &src->box[182]);
	n += putpokemon(dst+n, &src->box[183]);
	n += putpokemon(dst+n, &src->box[184]);
	n += putpokemon(dst+n, &src->box[185]);
	n += putpokemon(dst+n, &src->box[186]);
	n += putpokemon(dst+n, &src->box[187]);
	n += putpokemon(dst+n, &src->box[188]);
	n += putpokemon(dst+n, &src->box[189]);
	n += putpokemon(dst+n, &src->box[190]);
	n += putpokemon(dst+n, &src->box[191]);
	n += putpokemon(dst+n, &src->box[192]);
	n += putpokemon(dst+n, &src->box[193]);
	n += putpokemon(dst+n, &src->box[194]);
	n += putpokemon(dst+n, &src->box[195]);
	n += putpokemon(dst+n, &src->box[196]);
	n += putpokemon(dst+n, &src->box[197]);
	n += putpokemon(dst+n, &src->box[198]);
	n += putpokemon(dst+n, &src->box[199]);
	n += putpokemon(dst+n, &src->box[200]);
	n += putpokemon(dst+n, &src->box[201]);
	n += putpokemon(dst+n, &src->box[202]);
	n += putpokemon(dst+n, &src->box[203]);
	n += putpokemon(dst+n, &src->box[204]);
	n += putpokemon(dst+n, &src->box[205]);
	n += putpokemon(dst+n, &src->box[206]);
	n += putpokemon(dst+n, &src->box[207]);
	n += putpokemon(dst+n, &src->box[208]);
	n += putpokemon(dst+n, &src->box[209]);
	n += putpokemon(dst+n, &src->box[210]);
	n += putpokemon(dst+n, &src->box[211]);
	n += putpokemon(dst+n, &src->box[212]);
	n += putpokemon(dst+n, &src->box[213]);
	n += putpokemon(dst+n, &src->box[214]);
	n += putpokemon(dst+n, &src->box[215]);
	n += putpokemon(dst+n, &src->box[216]);
	n += putpokemon(dst+n, &src->box[217]);
	n += putpokemon(dst+n, &src->box[218]);
	n += putpokemon(dst+n, &src->box[219]);
	n += putpokemon(dst+n, &src->box[220]);
	n += putpokemon(dst+n, &src->box[221]);
	n += putpokemon(dst+n, &src->box[222]);
	n += putpokemon(dst+n, &src->box[223]);
	n += putpokemon(dst+n, &src->box[224]);
	n += putpokemon(dst+n, &src->box[225]);
	n += putpokemon(dst+n, &src->box[226]);
	n += putpokemon(dst+n, &src->box[227]);
	n += putpokemon(dst+n, &src->box[228]);
	n += putpokemon(dst+n, &src->box[229]);
	n += putpokemon(dst+n, &src->box[230]);
	n += putpokemon(dst+n, &src->box[231]);
	n += putpokemon(dst+n, &src->box[232]);
	n += putpokemon(dst+n, &src->box[233]);
	n += putpokemon(dst+n, &src->box[234]);
	n += putpokemon(dst+n, &src->box[235]);
	n += putpokemon(dst+n, &src->box[236]);
	n += putpokemon(dst+n, &src->box[237]);
	n += putpokemon(dst+n, &src->box[238]);
	n += putpokemon(dst+n, &src->box[239]);
	n += putpokemon(dst+n, &src->box[240]);
	n += putpokemon(dst+n, &src->box[241]);
	n += putpokemon(dst+n, &src->box[242]);
	n += putpokemon(dst+n, &src->box[243]);
	n += putpokemon(dst+n, &src->box[244]);
	n += putpokemon(dst+n, &src->box[245]);
	n += putpokemon(dst+n, &src->box[246]);
	n += putpokemon(dst+n, &src->box[247]);
	n += putpokemon(dst+n, &src->box[248]);
	n += putpokemon(dst+n, &src->box[249]);
	n += putpokemon(dst+n, &src->box[250]);
	n += putpokemon(dst+n, &src->box[251]);
	n += putpokemon(dst+n, &src->box[252]);
	n += putpokemon(dst+n, &src->box[253]);
	n += putpokemon(dst+n, &src->box[254]);
	n += putpokemon(dst+n, &src->box[255]);
	n += putpokemon(dst+n, &src->box[256]);
	n += putpokemon(dst+n, &src->box[257]);
	n += putpokemon(dst+n, &src->box[258]);
	n += putpokemon(dst+n, &src->box[259]);
	n += putpokemon(dst+n, &src->box[260]);
	n += putpokemon(dst+n, &src->box[261]);
	n += putpokemon(dst+n, &src->box[262]);
	n += putpokemon(dst+n, &src->box[263]);
	n += putpokemon(dst+n, &src->box[264]);
	n += putpokemon(dst+n, &src->box[265]);
	n += putpokemon(dst+n, &src->box[266]);
	n += putpokemon(dst+n, &src->box[267]);
	n += putpokemon(dst+n, &src->box[268]);
	n += putpokemon(dst+n, &src->box[269]);
	n += putpokemon(dst+n, &src->box[270]);
	n += putpokemon(dst+n, &src->box[271]);
	n += putpokemon(dst+n, &src->box[272]);
	n += putpokemon(dst+n, &src->box[273]);
	n += putpokemon(dst+n, &src->box[274]);
	n += putpokemon(dst+n, &src->box[275]);
	n += putpokemon(dst+n, &src->box[276]);
	n += putpokemon(dst+n, &src->box[277]);
	n += putpokemon(dst+n, &src->box[278]);
	n += putpokemon(dst+n, &src->box[279]);
	n += putpokemon(dst+n, &src->box[280]);
	n += putpokemon(dst+n, &src->box[281]);
	n += putpokemon(dst+n, &src->box[282]);
	n += putpokemon(dst+n, &src->box[283]);
	n += putpokemon(dst+n, &src->box[284]);
	n += putpokemon(dst+n, &src->box[285]);
	n += putpokemon(dst+n, &src->box[286]);
	n += putpokemon(dst+n, &src->box[287]);
	n += putpokemon(dst+n, &src->box[288]);
	n += putpokemon(dst+n, &src->box[289]);
	n += putpokemon(dst+n, &src->box[290]);
	n += putpokemon(dst+n, &src->box[291]);
	n += putpokemon(dst+n, &src->box[292]);
	n += putpokemon(dst+n, &src->box[293]);
	n += putpokemon(dst+n, &src->box[294]);
	n += putpokemon(dst+n, &src->box[295]);
	n += putpokemon(dst+n, &src->box[296]);
	n += putpokemon(dst+n, &src->box[297]);
	n += putpokemon(dst+n, &src->box[298]);
	n += putpokemon(dst+n, &src->box[299]);
	n += putpokemon(dst+n, &src->box[300]);
	n += putpokemon(dst+n, &src->box[301]);
	n += putpokemon(dst+n, &src->box[302]);
	n += putpokemon(dst+n, &src->box[303]);
	n += putpokemon(dst+n, &src->box[304]);
	n += putpokemon(dst+n, &src->box[305]);
	n += putpokemon(dst+n, &src->box[306]);
	n += putpokemon(dst+n, &src->box[307]);
	n += putpokemon(dst+n, &src->box[308]);
	n += putpokemon(dst+n, &src->box[309]);
	n += putpokemon(dst+n, &src->box[310]);
	n += putpokemon(dst+n, &src->box[311]);
	n += putpokemon(dst+n, &src->box[312]);
	n += putpokemon(dst+n, &src->box[313]);
	n += putpokemon(dst+n, &src->box[314]);
	n += putpokemon(dst+n, &src->box[315]);
	n += putpokemon(dst+n, &src->box[316]);
	n += putpokemon(dst+n, &src->box[317]);
	n += putpokemon(dst+n, &src->box[318]);
	n += putpokemon(dst+n, &src->box[319]);
	n += putpokemon(dst+n, &src->box[320]);
	n += putpokemon(dst+n, &src->box[321]);
	n += putpokemon(dst+n, &src->box[322]);
	n += putpokemon(dst+n, &src->box[323]);
	n += putpokemon(dst+n, &src->box[324]);
	n += putpokemon(dst+n, &src->box[325]);
	n += putpokemon(dst+n, &src->box[326]);
	n += putpokemon(dst+n, &src->box[327]);
	n += putpokemon(dst+n, &src->box[328]);
	n += putpokemon(dst+n, &src->box[329]);
	n += putpokemon(dst+n, &src->box[330]);
	n += putpokemon(dst+n, &src->box[331]);
	n += putpokemon(dst+n, &src->box[332]);
	n += putpokemon(dst+n, &src->box[333]);
	n += putpokemon(dst+n, &src->box[334]);
	n += putpokemon(dst+n, &src->box[335]);
	n += putpokemon(dst+n, &src->box[336]);
	n += putpokemon(dst+n, &src->box[337]);
	n += putpokemon(dst+n, &src->box[338]);
	n += putpokemon(dst+n, &src->box[339]);
	n += putpokemon(dst+n, &src->box[340]);
	n += putpokemon(dst+n, &src->box[341]);
	n += putpokemon(dst+n, &src->box[342]);
	n += putpokemon(dst+n, &src->box[343]);
	n += putpokemon(dst+n, &src->box[344]);
	n += putpokemon(dst+n, &src->box[345]);
	n += putpokemon(dst+n, &src->box[346]);
	n += putpokemon(dst+n, &src->box[347]);
	n += putpokemon(dst+n, &src->box[348]);
	n += putpokemon(dst+n, &src->box[349]);
	n += putpokemon(dst+n, &src->box[350]);
	n += putpokemon(dst+n, &src->box[351]);
	n += putpokemon(dst+n, &src->box[352]);
	n += putpokemon(dst+n, &src->box[353]);
	n += putpokemon(dst+n, &src->box[354]);
	n += putpokemon(dst+n, &src->box[355]);
	n += putpokemon(dst+n, &src->box[356]);
	n += putpokemon(dst+n, &src->box[357]);
	n += putpokemon(dst+n, &src->box[358]);
	n += putpokemon(dst+n, &src->box[359]);
	n += putpokemon(dst+n, &src->box[360]);
	n += putpokemon(dst+n, &src->box[361]);
	n += putpokemon(dst+n, &src->box[362]);
	n += putpokemon(dst+n, &src->box[363]);
	n += putpokemon(dst+n, &src->box[364]);
	n += putpokemon(dst+n, &src->box[365]);
	n += putpokemon(dst+n, &src->box[366]);
	n += putpokemon(dst+n, &src->box[367]);
	n += putpokemon(dst+n, &src->box[368]);
	n += putpokemon(dst+n, &src->box[369]);
	n += putpokemon(dst+n, &src->box[370]);
	n += putpokemon(dst+n, &src->box[371]);
	n += putpokemon(dst+n, &src->box[372]);
	n += putpokemon(dst+n, &src->box[373]);
	n += putpokemon(dst+n, &src->box[374]);
	n += putpokemon(dst+n, &src->box[375]);
	n += putpokemon(dst+n, &src->box[376]);
	n += putpokemon(dst+n, &src->box[377]);
	n += putpokemon(dst+n, &src->box[378]);
	n += putpokemon(dst+n, &src->box[379]);
	n += putpokemon(dst+n, &src->box[380]);
	n += putpokemon(dst+n, &src->box[381]);
	n += putpokemon(dst+n, &src->box[382]);
	n += putpokemon(dst+n, &src->box[383]);
	n += putpokemon(dst+n, &src->box[384]);
	n += putpokemon(dst+n, &src->box[385]);
	n += putpokemon(dst+n, &src->box[386]);
	n += putpokemon(dst+n, &src->box[387]);
	n += putpokemon(dst+n, &src->box[388]);
	n += putpokemon(dst+n, &src->box[389]);
	n += putpokemon(dst+n, &src->box[390]);
	n += putpokemon(dst+n, &src->box[391]);
	n += putpokemon(dst+n, &src->box[392]);
	n += putpokemon(dst+n, &src->box[393]);
	n += putpokemon(dst+n, &src->box[394]);
	n += putpokemon(dst+n, &src->box[395]);
	n += putpokemon(dst+n, &src->box[396]);
	n += putpokemon(dst+n, &src->box[397]);
	n += putpokemon(dst+n, &src->box[398]);
	n += putpokemon(dst+n, &src->box[399]);
	n += putpokemon(dst+n, &src->box[400]);
	n += putpokemon(dst+n, &src->box[401]);
	n += putpokemon(dst+n, &src->box[402]);
	n += putpokemon(dst+n, &src->box[403]);
	n += putpokemon(dst+n, &src->box[404]);
	n += putpokemon(dst+n, &src->box[405]);
	n += putpokemon(dst+n, &src->box[406]);
	n += putpokemon(dst+n, &src->box[407]);
	n += putpokemon(dst+n, &src->box[408]);
	n += putpokemon(dst+n, &src->box[409]);
	n += putpokemon(dst+n, &src->box[410]);
	n += putpokemon(dst+n, &src->box[411]);
	n += putpokemon(dst+n, &src->box[412]);
	n += putpokemon(dst+n, &src->box[413]);
	n += putpokemon(dst+n, &src->box[414]);
	n += putpokemon(dst+n, &src->box[415]);
	n += putpokemon(dst+n, &src->box[416]);
	n += putpokemon(dst+n, &src->box[417]);
	n += putpokemon(dst+n, &src->box[418]);
	n += putpokemon(dst+n, &src->box[419]);
	n += putboxname(dst+n, &src->name[0]);
	n += putboxname(dst+n, &src->name[1]);
	n += putboxname(dst+n, &src->name[2]);
	n += putboxname(dst+n, &src->name[3]);
	n += putboxname(dst+n, &src->name[4]);
	n += putboxname(dst+n, &src->name[5]);
	n += putboxname(dst+n, &src->name[6]);
	n += putboxname(dst+n, &src->name[7]);
	n += putboxname(dst+n, &src->name[8]);
	n += putboxname(dst+n, &src->name[9]);
	n += putboxname(dst+n, &src->name[10]);
	n += putboxname(dst+n, &src->name[11]);
	n += putboxname(dst+n, &src->name[12]);
	n += putboxname(dst+n, &src->name[13]);
	memcpy(dst+n, src->wallpaper, 14);
	n += 14;
	return n;
}

