// Manic Miner C Port Copyright 2017 Michael R. Cook
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

// character_set.c

extern uint8_t character_set[96][8];


// data.c

extern uint8_t WILLYDATA[256];
extern uint8_t MANDAT[64];
extern uint8_t WILLYR2[32];
extern uint8_t WILLYR3[160];
extern uint16_t SBUFADDRS[128];

extern int CHEATDT[16];

extern uint8_t THEMETUNE[95][3];
extern uint8_t GAMETUNE[64];

extern char *MESSINTRO;

extern uint8_t LOWERATTRS[512];
extern uint8_t TITLESCR1[2048];
extern uint8_t TITLESCR2[2048];

extern uint8_t CAVERN0[512];
extern uint8_t PLINTH[32];
extern uint8_t BOOT[256];
extern uint8_t EUGENEG[32];
extern uint8_t CAVERN19[512];



// functions
bool play_new_cavern();
bool MANDEAD();
void ENDGAM();
bool DECAIR();
void DRAWSHEET();
bool MOVEWILLY();
void MOVEWILLY_7(uint8_t y_coord);
void MOVEWILLY_8();
void MOVEWILLY_9();
void MOVEWILLY_10();
void CRUMBLE(uint16_t addr);
bool MOVEWILLY2(uint16_t addr);
void MOVEWILLY2_6();
void MOVEWILLY2_7();
void MOVEWILLY2_9();
void MOVEWILLY2_10();
bool KILLWILLY();
bool KILLWILLY_0();
bool KILLWILLY_1();
void MOVEHG();
void LIGHTBEAM();
bool DRAWHG();
bool EUGENE();
void EUGENE_3(uint16_t addr, uint8_t ink_color);
bool SKYLABS();
bool VGUARDIANS();
void DRAWITEMS();
bool CHKPORTAL();
bool DRWFIX(void *sprite, uint16_t addr, uint8_t mode);
bool NXSHEET();
void INCSCORE_0(uint16_t addr);
void MVCONVEYOR();
bool KONGBEAST();
bool KONGBEAST_8();
bool CHKSWITCH(uint16_t addr);
bool WILLYATTRS();
bool WILLYATTR(uint16_t addr, uint8_t ink);
void DRAWWILLY();
void PMESS(void *msg, uint16_t addr, uint8_t len);
void PRINTCHAR(char ch, uint16_t addr);
void PRINTCHAR_0(void *chr, uint16_t addr, uint8_t len);
bool PLAYTUNE();
uint16_t PIANOKEY(uint8_t frequency);
bool CHECKENTER();

void initialize_cavern0();
void draw_remaining_lives();
void tick();

// curses.c
void initialize_curses();
void restore_terminal();
void clear_screen();
void clear_to_bottom_from(int row);
void redraw_screen();
void put_tile(char ch, int row, int col);
void byte_to_bits(uint8_t byte, uint8_t *bits);
void memory_dump_to_buffer();
bool check_any_keypress();
bool check_enter_keypress();
bool check_left_keypress();
bool check_right_keypress();
bool check_jump_keypress();
bool check_quit_keypress();
bool check_exit_keypress();
bool check_pause_keypress();

// emulator.c
uint8_t MEM[1024 * 64];
uint8_t regA;
uint8_t regB;
uint8_t regC;
uint8_t regD;
uint8_t regE;
uint8_t regH;
uint8_t regL;
uint16_t regBC;
uint16_t regDE;
uint16_t regHL;
uint8_t IN(uint16_t addr);
void OUT(uint8_t value);


// helpers.c
void millisleep(int milliseconds);
int timediff(clock_t t1, clock_t t2);
void split_address(uint16_t addr, uint8_t *msb, uint8_t *lsb);
uint16_t build_address(uint8_t msb, uint8_t lsb);
