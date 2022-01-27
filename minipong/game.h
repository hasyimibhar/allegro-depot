#include <allegro.h>
#include <stdio.h>
#include <math.h>

//------------------------------------------------------------------------------

#define SCREEND    32
#define SCREENW    320
#define SCREENH    240

#define WHITE           makecol(255,255,255)
#define BLACK           makecol(0,0,0)

#define S_TITLE           0
#define S_GAME            1
#define S_INTRO           2
#define S_BETWEEN         3
#define S_PAUSE           4
#define S_DELAY1          5
#define S_DELAY2          6
#define S_DELAY3          7
#define S_DELAY4          8
#define S_DELAY5          9
#define S_DELAY6          10
#define S_TITLE2          11
#define S_WIN             12
#define S_SHUTDOWN        255

#define EXIT_KEY        if(keyPressed(KEY_ESC))

//------------------------------------------------------------------------------

int game_state;
unsigned int score[2];
BITMAP* DoubleBuffer;
BITMAP* Black;
DATAFILE* data;

int intro, intro_trans, between, delay1, delay2, title_trans;

bool selection_1[2] = { true, false };
char* s_text_1[2] = {"start game", "exit game"};

bool selection_2[2] = { true, false };
char* s_text_2[2] = {"1 player", "2 players"};

int sx[2] = {60, 200};
unsigned char select;

bool midi = false;
unsigned char cmid;

bool p2 = false;

bool thesound = true;
bool thebg = true;

int r_midi;
bool shu_list[3] = { false, false, false };
int shu_num = 0;

unsigned char winner = 0;

//------------------------------------------------------------------------------

void initGame();
void processGame();
void shutdownGame();
void blitBuffer();
void logicProcess();
void drawProcess();
void keyPresses();
void writeTextFromInt(char* text, int value, int x, int y, int col);
void writeTextFromInt2(char* text, int value, int x, int y, int col);
void fadeIn(BITMAP* bmp, int trans, int x, int y);
void restart();
