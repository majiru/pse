typedef struct Section Section;
typedef struct Trainer Trainer;
typedef struct Pokemon Pokemon;
typedef struct Invent Invent;
typedef struct InventPokemon InventPokemon;

typedef struct Blocka Blocka;
typedef struct Blockg Blockg;
typedef struct Blockm Blockm;
typedef struct Blocke Blocke;
typedef struct Pokedat Pokedat;

typedef struct Boxname Boxname;
typedef struct PC PC;

struct Section {
	uchar data[4084];
	u16int id;
	u16int chk;
	u32int pad;
	u32int index;
};

struct Trainer {
	uchar name[7];
	uchar gender;
	u16int pad0;
	u16int id;
	u16int secretid;
	u16int hours;
	uchar min;
	uchar sec;
	uchar frame;
	uchar button;
	uchar text;
	uchar misc;
	uchar pad1[150];
	u32int gamecode;
};

struct Blockg {
	u16int species;
	u16int item;
	u32int exp;
	uchar ppbonus;
	uchar friendship;
	u16int magic;
};

struct Blocka {
	u16int move1;
	u16int move2;
	u16int move3;
	u16int move4;
	uchar pp[4];
};

struct Blockm {
	uchar pokerus;
	uchar met;
	u16int origins;
	u32int iv;
	u32int ribbions;
};

struct Blocke {
	uchar hp;
	uchar atk;
	uchar def;
	uchar spd;
	uchar spatk;
	uchar spdef;
	uchar cool;
	uchar beauty;
	uchar cute;
	uchar smart;
	uchar tough;
	uchar feel;
};

struct Pokedat {
	Blockg g;
	Blocka a;
	Blockm m;
	Blocke e;
};

struct Pokemon {
	u32int personality;
	u16int otid;
	u16int otsecretid;
	uchar name[10];
	u16int lang;
	uchar otname[7];
	uchar marks;
	u16int chk;
	u16int magic;
	uchar data[48];
};

struct InventPokemon {
	Pokemon p;
	uchar derived[20];
};

struct Invent {
	u32int teamsz;
	InventPokemon team[6];
	u32int money;
	u16int coins;
};

struct Boxname {
	uchar n[9];
};

struct PC {
	u32int current;
	Pokemon box[420];
	Boxname name[14];
	uchar wallpaper[14];
};
