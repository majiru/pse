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

extern char* gen3gnametab[];

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

typedef struct Gen3iv Gen3iv;
struct Gen3iv {
	uchar hp;
	uchar atk;
	uchar def;
	uchar spe;
	uchar spatk;
	uchar spdef;
	uchar egg;
	uchar ability;
};

void gen3pkstr(uchar *d, uchar *s, int n);
int gen3strfmt(Fmt*);
void getgen3(int fd, Gen3 *save);
void decryptpokemon(Pokedat *dst, Pokemon *src);
void getgen3iv(Gen3iv *dst, u32int src);
int getgen3dex(u16int species);
int gen3shiny(Pokemon*);
