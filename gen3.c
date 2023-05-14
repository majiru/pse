#include <u.h>
#include <libc.h>
#include "gen3.h"

typedef struct Save Save;
struct Save{
	Section bank1[14];
	Section bank2[14];
	Section *active;
	Trainer tr;
};

Save save;

long getsection(Section*,uchar*);
long gettrainer(Trainer*,uchar*);

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
	int i;
	uchar buf[8192];

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
		if(save.active[i].id != 0)
			continue;
		gettrainer(&save.tr, save.active[i].data);
		fprint(2, "%s\n", pkstr(save.tr.name, save.tr.name + 7));
		fprint(2, "%d %d %d\n", save.tr.hours, save.tr.min, save.tr.sec);
		fprint(2, "%d %d\n", save.tr.id, save.tr.secretid);
		fprint(2, "%d\n", save.tr.gamecode);
	}
}
