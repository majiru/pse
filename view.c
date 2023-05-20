#include <u.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>
#include <mouse.h>
#include <cursor.h>
#include <keyboard.h>
#include <ctype.h>
#include "gen3dat.h"
#include "colodat.h"
#include "pse.h"

Save save;

int currentbox = 0;
void *currentpk = nil;
Point spwd;
Image *background, *light;

Image *spritecache[1024];

extern char *dexfiletab[];
extern char *movenametab[];

static void
chbox(int x)
{
	int max;

	switch(save.type){
	default:
	case GG3:
		max = 13;
		break;
	case GCOLO:
		max = 2;
		break;
	}
	currentbox += x;
	if(currentbox < 0)
		currentbox = max;
	else if(currentbox > max)
		currentbox = 0;
}

static int
screenprint(Point p, char *s)
{
	char *y, *dot;
	Point op;

	op = p;
	for(y = s; (dot = strchr(y, '\n')) != nil; y = dot+1){
		*dot = 0;
		string(screen, p, display->black, ZP, display->defaultfont, y);
		p.y += display->defaultfont->height;
	}
	return p.y - op.y;
}

static void
redraw(void)
{
	char path[128];
	char buf[512];
	Image *image;
	Rectangle r, r2;
	int i;
	int fd;
	int dex;
	void *p;

	draw(screen, screen->r, background, nil, ZP);
	r = screen->r;
	r2 = r;
	spwd = Pt(68*2, 56*2);
	save.view->hdr(buf, buf + sizeof buf, &save.gen3, currentbox);
	r.min.y += screenprint(r.min, buf);

	if(currentpk == nil)
		currentpk = save.view->box(0, 0, &save.gen3);
	for(i = 0; i < 30; i++){
		r2.min.x = r.min.x + (i%6) * spwd.x;
		r2.min.y = r.min.y + (i/6) * spwd.y;
		r2.max.x = r2.min.x + spwd.x;
		r2.max.y = r2.min.y + spwd.y;
		p = save.view->box(currentbox, i, &save.gen3);
		if(p == currentpk)
			draw(screen, r2, light, nil, ZP);
		dex = save.view->dex(p);
		if(dex > 411 || dex == -1)
			continue;
		snprint(path, sizeof path, "/sys/games/lib/pokesprite/regular/%s.png", dexfiletab[dex]);

		image = spritecache[dex];
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
		spritecache[dex] = image;
	}

	r = screen->r;
	r.min.x += 6*spwd.x;
	save.view->body(buf, buf + sizeof buf, currentpk);
	r.min.y += screenprint(r.min, buf);
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

	/* FIXME */
	p.y -= display->defaultfont->height*3;

	p.x /= spwd.x;
	p.y /= spwd.y;
	if(p.x + (p.y*6) > 30)
		return 0;
	currentpk = save.view->box(currentbox, (p.x + (p.y*6)), &save.gen3);
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
	fprint(2, "usage: %s [-3c] file\n", argv0);
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

	save.type = GNONE;
	ARGBEGIN{
	case '3':
		save.view = &vgen3;
		save.type = GG3;
		break;
	case 'c':
		save.view = &vcolo;
		save.type = GCOLO;
		break;
	default:
		usage();
	}ARGEND;
	if(argc < 1 || save.type == GNONE)
		usage();

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");

	save.view->init();
	switch(save.type){
	case GG3:
		getgen3(fd, &save.gen3);
		break;
	case GCOLO:
		getcolo(fd, &save.colo);
		break;
	}

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
