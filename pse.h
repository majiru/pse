enum {
	Slotszcolo = 0x1E000,
};

typedef struct Colo Colo;
struct Colo {
	uchar slots[3][Slotszcolo];
	u32int index[3];
	uchar gcihdr[0x40];
	uchar decrypted[Slotszcolo];

	uchar *active;
	Pokemoncolo pc[30*3];
	Trainercolo tr;
	u32int money;
	u32int coupons;
};

long getpokemoncolo(Pokemoncolo*,uchar*);
long gettrainercolo(Trainercolo*,uchar*);
void getcolo(int fd, Colo *dst);

enum{
	/* Game Data Type */
	GNONE,
	GG3,
	GCOLO,

	/* Gen3 Cart Type */
	GRS,
	GFRLG,
	GEM,
};

extern char* gnametab[];

long getsection(Section*,uchar*);
long gettrainer(Trainer*,uchar*);
long getinvent(Invent*,uchar*);
long getpokedat(Pokedat*,uchar*);
long getpc(PC*,uchar*);

typedef struct Gen3 Gen3;
struct Gen3 {
	int type;
	Section bank1[14];
	Section bank2[14];
	Section *active;
	Trainer tr;
	Invent inv;
	PC pc;

	uchar pcbuf[3968*8 + 2000];
};

void gen3pkstr(uchar *d, uchar *p, int n);
void getgen3(int fd, Gen3 *save);
void decryptpokemon(Pokedat *dst, Pokemon *src);
int gen3shiny(Pokemon*);

typedef struct View View;
struct View {
	void (*init)(void);
	int (*hdr)(char *dst, char *e, void *v, int box);
	int (*dex)(void *v);
	int (*body)(char *dst, char *e, void *v);
	void* (*box)(int box, int i, void *v);
};

extern View vgen3;
extern View vcolo;

typedef struct Save Save;
struct Save {
	View *view;
	int type;
	union {
		Gen3 gen3;
		Colo colo;
	};
};
