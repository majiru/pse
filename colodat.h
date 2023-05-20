typedef struct Movecolo Movecolo;
typedef struct Statcolo Statcolo;
typedef struct Pokemoncolo Pokemoncolo;
typedef struct Slotcolo Slotcolo;
typedef struct Trainercolo Trainercolo;

struct Movecolo {
	u16int id;
	uchar pp;
	uchar up;
};

struct Statcolo {
	u16int hp;
	u16int atk;
	u16int def;
	u16int spa;
	u16int spd;
	u16int spe;
};

struct Pokemoncolo {
	u16int species;
	u16int pad0;
	u32int pid;
	uchar version;
	uchar curregion;
	uchar oriregion;
	uchar lang;
	u16int metloc;
	uchar metlvl;
	uchar ball;
	uchar otgender;
	uchar pad3[3];
	u16int otid;
	u16int otsecretid;
	uchar otname[22];
	uchar name[22];
	uchar namecopy[22];
	u16int pad1;
	u32int exp;
	uchar statlvl;
	uchar battle[23];
	Movecolo moves[4];
	u16int item;
	uchar derived[14];
	Statcolo ev;
	Statcolo iv;
	u16int friendship;
	uchar contest[11];
	uchar ribbon[13];
	uchar pkrs;
	uchar isegg;
	uchar ability;
	uchar valid;
	uchar pad2[9];
	uchar slot;
	u16int shadowid;
	u16int pad4;
	u32int purification;
	uchar extra[88];
};

struct Trainercolo {
	uchar name[20];
	uchar namecopy[20];
	u32int pad0;
	u16int id;
	u16int secretid;
};
