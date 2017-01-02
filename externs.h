// Manic Miner C Port Copyright 2016 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.


// globals.c

extern char *CAVERNNAME;
extern uint8_t BACKGROUND[9];
extern uint8_t FLOOR[9];
extern uint8_t CRUMBLING[9];
extern uint8_t WALL[9];
extern uint8_t CONVEYOR[9];
extern uint8_t NASTY1[9];
extern uint8_t NASTY2[9];
extern uint8_t EXTRA[9];

extern uint8_t PIXEL_Y;
extern uint8_t FRAME;
extern uint8_t DMFLAGS;
extern uint8_t AIRBORNE;
extern uint16_t LOCATION;
extern uint8_t JUMPING;
extern uint8_t CONVDIR;
extern uint16_t CONVLOC;
extern uint8_t CONVLEN;
extern uint8_t BORDER;
extern uint8_t ITEMATTR;
extern uint16_t ITEMS[5][5];
extern uint8_t PORTAL;
extern uint8_t PORTALG[32];
extern uint16_t PORTALLOC1;
extern uint16_t PORTALLOC2;
extern uint8_t ITEM[8];
extern uint8_t AIR;
extern uint8_t CLOCK;
extern uint16_t HGUARDS[4][6];
extern uint8_t EUGDIR;
extern uint8_t EUGHGT;
extern uint8_t VGUARDS[4][7];
extern uint8_t GGDATA[256];
extern uint8_t SWORDFISH[32];

extern uint8_t SHEET;
extern uint8_t LRMOVEMENT[16];

extern char MESSAIR[];
extern char HGHSCOR[];
extern char SCORE[];
extern char SCORBUF[];
extern char MESSHSSC[];
extern int highscore;
extern int current_score;

extern char MESSG[];
extern char MESSO[];

extern uint8_t NOMEN;
extern uint8_t FLASH;
extern uint8_t KEMP;
extern uint8_t DEMO;
extern uint8_t NOTEINDEX;
extern uint8_t MUSICFLAGS;
extern uint8_t CHEAT;


// data.c

extern uint8_t MANDAT[64];
extern uint8_t WILLYR2[32];
extern uint8_t WILLYR3[160];
extern uint16_t SBUFADDRS[128];

extern int CHEATDT[16];

extern uint8_t THEMETUNE[298];
extern uint8_t GAMETUNE[64];

extern char MESSINTRO[];

extern uint8_t LOWERATTRS[512];
extern uint8_t TITLESCR1[2048];
extern uint8_t TITLESCR2[2048];

extern uint8_t CAVERN0[512];
extern uint8_t BOOT[256];
extern uint8_t EUGENEG[32];
extern uint8_t CAVERN19[512];


// functions
void DRAWWILLY();
void PMESS();
void PRINTCHAR();
bool PLAYTUNE();
void PIANOKEY();
bool CHECKENTER();


void initialize_cavern0();

// helpers.c
void millisleep(int milliseconds);
