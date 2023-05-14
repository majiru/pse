typedef struct Section Section;
typedef struct Trainer Trainer;
typedef struct Pokemon Pokemon;
typedef struct Invent Invent;

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

struct Pokemon {
	u32int personality;
	u32int otid;
	uchar name[10];
	u16int lang;
	uchar otname[7];
	uchar marks;
	u16int chk;
	u16int magic;
	uchar data[48];
	uchar derived[20];
};

struct Invent {
	u32int teamsz;
	Pokemon p1;
};
