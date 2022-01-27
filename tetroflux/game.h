#include <allegro.h>
#include <alfont.h>
#include <png.h>
#include "loadpng.h"
#include <stdio.h>
#include <math.h>
#include "data\\data.h"

//------------------------------------------------------------------------------

#define DAT_PNG  DAT_ID('P','N','G',' ')

#define SCREEND    32
#define SCREENW    320
#define SCREENH    240

#define WHITE           makecol(255,255,255)
#define BLACK           makecol(0,0,0)

#define S_TITLE           0
#define S_GAME            1
#define S_OVER            2
#define S_DELAY1          3
#define S_DELAY2          4
#define S_DELAY3          5
#define S_MENU            6
#define S_DELAY4          7
#define S_DELAY5          8
#define S_PAUSE           9 
#define S_SHUTDOWN        255

#define DOWN              1
#define RIGHT             2
#define LEFT              3

#define EXIT_KEY        if(keyPressed(KEY_ESC))

//------------------------------------------------------------------------------

int game_state;
int tetro_move;
int tetro_wipe;
int tetro_flash;
int tetro_row;
int tetro_row2;
int clear_row;
int clear_row2;
int c_a;
int circle_frame;
int delay;
int space_trans, space_trans2;
int score;
int h_score;
bool reg_score;
unsigned char next_id, next_rot;
BITMAP* DoubleBuffer;
BITMAP* backRect;
BITMAP* OB;
BITMAP* next;
BITMAP* ScoreBg;
BITMAP* PauseBg;
DATAFILE* data;
ALFONT_FONT *myfont;

//------------------------------------------------------------------------------

void initGame();
void processGame();
void shutdownGame();
void blitBuffer();
void logicProcess();
void drawProcess();
void keyPresses();
void drawTrans(BITMAP* bmp, int trans, int x, int y);
void drawPieces();
void writeScore(int value, int x, int y, int col);
