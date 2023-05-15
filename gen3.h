enum{
	/* Sections */
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

	/* Game Type */
	GRS,
	GFRLG,
	GEM,
};

long getsection(Section*,uchar*);
long gettrainer(Trainer*,uchar*);
long getinvent(Invent*,uchar*);
long getpokedat(Pokedat*,uchar*);
long getpc(PC*,uchar*);

typedef struct Gen3 Gen3;
struct Gen3{
	int type;
	Section bank1[14];
	Section bank2[14];
	Section *active;
	Trainer tr;
	Invent inv;
	PC pc;

	uchar pcbuf[3968*8 + 2000];
};

void gen3pkstr(uchar *d, uchar *s, int n);
void getgen3(int fd, Gen3 *save);
void decryptpokemon(Pokedat *dst, Pokemon *src);
