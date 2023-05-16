#include <u.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>
#include <mouse.h>
#include <cursor.h>
#include <keyboard.h>
#include <ctype.h>
#include "gen3dat.h"
#include "gen3.h"

Gen3 gen3;

static void
mklower(char *d, char *e, char *s)
{
	assert(d <= e-1);
	for(; *s != 0 && d < e-1; s++){
		if(!isascii(*s))
			continue;
		*d++ = tolower(*s);
	}
	*d = 0;
}

int currentbox = 0;
Pokemon *currentpk = nil;
Point spwd;
Image *background, *light;

Image *spritecache[1024];

extern char *dexfiletab[];
extern char *movenametab[];

static void
chbox(int x)
{
	currentbox += x;
	if(currentbox < 0)
		currentbox = 0;
	else if(currentbox > 13)
		currentbox = 13;
}

static void
redraw(void)
{
	char buf[32];
	char path[128];
	Image *image;
	Rectangle r, r2;
	Pokedat pd;
	Gen3iv iv;
	int i;
	int fd;

	draw(screen, screen->r, background, nil, ZP);
	r = screen->r;
	r2 = r;
	spwd = Pt(68*2, 56*2);

	gen3pkstr((uchar*)buf, gen3.tr.name, sizeof gen3.tr.name);
	snprint(path, sizeof path, "Name: %s  ID: %d  Secret ID: %d", buf, gen3.tr.id, gen3.tr.secretid);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;
	snprint(path, sizeof path, "Time Played: %dhr %dmin", gen3.tr.hours, gen3.tr.min);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;
	gen3pkstr((uchar*)buf, gen3.pc.name[currentbox].n, sizeof gen3.pc.name[currentbox].n);
	snprint(path, sizeof path, "Box %d: %s", currentbox+1, buf);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;

	if(currentpk == nil)
		currentpk = gen3.pc.box;
	for(i = 0; i < 30; i++){
		if(gen3.pc.box[currentbox*30 + i].otid == 0)
			continue;
		decryptpokemon(&pd, gen3.pc.box + currentbox*30 + i);
		//fprint(2, "%d %s\n", pd.g.species, dexfiletab[pd.g.species]);
		snprint(path, sizeof path, "/sys/games/lib/pokesprite/regular/%s.png", dexfiletab[getgen3dex(pd.g.species)]);
		r2.min.x = r.min.x + (i%6) * spwd.x;
		r2.min.y = r.min.y + (i/6) * spwd.y;
		r2.max.x = r2.min.x + spwd.x;
		r2.max.y = r2.min.y + spwd.y;

		if(gen3.pc.box + currentbox*30 + i == currentpk)
			draw(screen, r2, light, nil, ZP);

		image = spritecache[pd.g.species-1];
		if(image == nil){
			fd = open(path, OREAD);
			if(fd < 0){
				fprint(2, "could not open %s\n", path);
				continue;
			}
			image = readimage(display, fd, 0);
			close(fd);
			if(image == nil)
				continue;
		}
		draw(screen, r2, image, nil, ZP);
		spritecache[pd.g.species-1] = image;
	}

	decryptpokemon(&pd, currentpk);
	r = screen->r;
	r.min.x += 6*spwd.x;

	snprint(path, sizeof path, "Species: %d", pd.g.species);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;
	snprint(path, sizeof path, "Exp: %d", pd.g.exp);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;
	snprint(path, sizeof path, "Move 1: %s  Move 2: %s", movenametab[pd.a.move1], movenametab[pd.a.move2]);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;

	snprint(path, sizeof path, "Move 3: %s  Move 4: %s", movenametab[pd.a.move3], movenametab[pd.a.move4]);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;

	snprint(path, sizeof path, "[EV] HP: %d  Atk: %d  Def: %d  SpA: %d  SpD: %d  Spe: %d", pd.e.hp, pd.e.atk, pd.e.def, pd.e.spatk, pd.e.spdef, pd.e.spd);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;

	getgen3iv(&iv, pd.m.iv);
	snprint(path, sizeof path, "[IV] HP: %d  Atk: %d  Def: %d  SpA: %d  SpD: %d  Spe: %d", iv.hp, iv.atk, iv.def, iv.spatk, iv.spdef, iv.spe);
	string(screen, r.min, display->black, ZP, display->defaultfont, path);
	r.min.y += display->defaultfont->height;

	flushimage(display, 1);
}

static int
click(Mouse *m)
{
	Point p;

	if((m->buttons&(1<<3)) != 0){
		chbox(-1);
		return 1;
	} else if((m->buttons&(1<<4)) != 0){
		chbox(1);
		return 1;
	}
	if((m->buttons&1) == 0)
		return 0;
	p = m->xy;

	p.y -= screen->r.min.y;
	p.x -= screen->r.min.x;

	p.y -= display->defaultfont->height*3;

	p.x /= spwd.x;
	p.y /= spwd.y;
	if(p.x + (p.y*6) > 30)
		return 0;
	currentpk = gen3.pc.box +currentbox*30 + (p.x + (p.y*6));
	return 1;
}

enum{
	Ckey,
	Cmouse,
	Cresize,
	Numchan,
};

void
usage(void)
{
	fprint(2, "usage: %s game.sav\n", argv0);
	threadexitsall("usage");
}

mainstacksize = 8192*16;

void
threadmain(int argc, char **argv)
{
	Mousectl *mctl;
	Keyboardctl *kctl;
	Rune r;
	Mouse m;
	int fd;
	Alt a[Numchan+1] = {
		[Ckey] = {nil, &r, CHANRCV},
		[Cmouse] = {nil, &m, CHANRCV },
		[Cresize] = {nil, nil, CHANRCV},
		{nil, nil, CHANEND},
	};

	ARGBEGIN{
	default:
		usage();
	}ARGEND;
	if(argc < 1)
		usage();

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	getgen3(fd, &gen3);

	if(initdraw(nil, nil, "pse") < 0)
		sysfatal("initdraw: %r");
	if((kctl = initkeyboard(nil)) == nil)
		sysfatal("initkeyboard: %r");
	a[Ckey].c = kctl->c;
	if((mctl = initmouse(nil, screen)) == nil)
		sysfatal("initmouse: %r");
	a[Cmouse].c = mctl->c;
	a[Cresize].c = mctl->resizec;

	light = allocimagemix(display, DMedblue, DWhite);
	background = allocimagemix(display, DPaleyellow, DWhite);

	redraw();
	for(;;){
		switch(alt(a)){
		case -1:
			goto end;
		case Ckey:
			switch (r) {
			case 'w':
				chbox(-1);
				redraw();
				break;
			case 'e':
				chbox(1);
				redraw();
				break;
			case Kdel:
			case 'q':
				goto end;
				break;
			}
			break;

		case Cmouse:
			if(click(&m))
				redraw();
			break;

		case Cresize:
			getwindow(display, Refnone);
			redraw();
			break;
		}
	}
end:
	threadexitsall(nil);
}
