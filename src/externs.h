// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "speccy.h"
#include "cavern.h"
#include "conveyor.h"
#include "game.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "item.h"
#include "portal.h"
#include "willy.h"

// Setup all game objects
extern Speccy speccy;
extern Game game;
extern Cavern cavern;
extern GuardianHorizontal HGUARDS[4];
extern GuardianVertical VGUARDS[4];
extern Willy willy;

// Text and graphics
extern uint8_t EUGDIR;
extern uint8_t EUGHGT;
extern uint8_t GGDATA[256]; // now only used for Kong Beast
extern uint8_t SWORDFISH[32];
extern char MESSAIR[];
extern char HGHSCOR[];
extern char SCORE[];
extern char SCORBUF[];
extern char MESSHSSC[];
extern char MESSG[];
extern char MESSO[];

extern uint8_t LRMOVEMENT[16];

// character_set.c

extern uint8_t character_set[96][8];

// data.c
extern uint8_t WILLYDATA[256];
extern uint16_t SBUFADDRS[128];

extern int CHEATDT[7];

extern uint8_t THEMETUNE[95][3];
extern uint8_t GAMETUNE[64];

extern uint8_t LOWERATTRS[512];
extern uint8_t TITLESCR1[2048];
extern uint8_t TITLESCR2[2048];

extern uint8_t PLINTH[32];
extern uint8_t BOOT[32];
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

// helpers.c
void millisleep(int milliseconds);
int timediff(clock_t t1, clock_t t2);
void split_address(uint16_t addr, uint8_t *msb, uint8_t *lsb);
uint16_t build_address(uint8_t msb, uint8_t lsb);
uint8_t rotl (uint8_t a, uint8_t n);
uint8_t rotr (uint8_t a, uint8_t n);
