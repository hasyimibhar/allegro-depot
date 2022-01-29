#include <allegro.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include "data\data.h"

//------------------------------------------------------------------------------
//
// GAME DEFINITIONS
//
//------------------------------------------------------------------------------

#define SCREEN_DEPTH    32
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480

#define STATE_TITLE     0
#define STATE_GAME      1
#define STATE_GAMEOVER  2
#define STATE_READY     3
#define STATE_LOAD      4
#define STATE_BETWEEN   5
#define STATE_PAUSED    6
#define STATE_INS       7
#define STATE_SHUTDOWN  255

#define LAYER_COUNT     3
#define BASE_L          0
#define OBJECT_L        1
#define SNAKE_L         2

#define START_X         3
#define START_Y         24

#define TILE_COUNT      3
#define TILE_SIZE       16
#define FLOOR           1
#define FLOOR_WALL      2
#define WALL            3

#define MAP_WIDTH       40
#define MAP_HEIGHT      29

#define UP_KEY          if(key[KEY_UP] && snake[0].dir != DOWN)
#define DOWN_KEY        if(key[KEY_DOWN] && snake[0].dir != UP)
#define RIGHT_KEY       if(key[KEY_RIGHT] && snake[0].dir != LEFT)
#define LEFT_KEY        if(key[KEY_LEFT] && snake[0].dir != RIGHT)
#define EXIT_KEY        if( keyPressed(KEY_ESC) )
#define RESTART_KEY     if( keyPressed(KEY_R) )
#define CONTINUE_KEY    if( keyPressed(KEY_ENTER) )
#define PAUSE_KEY       if( keyPressed(KEY_P) )

#define UP              0
#define RIGHT           1
#define DOWN            3
#define LEFT            2
#define NOMOVE          4    

#define S_SLOW          1   
#define S_NORMAL        1.5
#define S_FAST          2
#define S_VFAST         2.5

#define ITEM_TIME       70
#define ITEM_COUNT      10
#define APPLE           1
#define ORANGE          2
#define PIZZA           3
#define STRAWBERRY      4
#define RICE            5
#define MUSHROOM        6
#define GROW            7
#define SHRINK          8
#define SLOW            9
#define FAST            10

#define OPTION_COUNT       3
#define OP_YPOS            280
#define OP_STARTGAME       1
#define OP_INSTRUCTIONS    2
#define OP_EXITGAME        3

//------------------------------------------------------------------------------
//
// STRUCT
//
//------------------------------------------------------------------------------

typedef struct TILE {
  //unsigned char x, y;
  unsigned char id;
} TILE;

typedef struct LAYER {
  TILE cell[MAP_HEIGHT][MAP_WIDTH];
} LAYER;

typedef struct WORLD {
  unsigned int x, y;
  LAYER layer[LAYER_COUNT];
} WORLD;

WORLD map;

typedef struct SNAKE {
  unsigned int x,y;
  unsigned char dir, ldir, corner;
} SNAKE;

SNAKE snake[ MAP_HEIGHT*MAP_WIDTH ];

typedef struct OPTION {
  unsigned char id;
  unsigned int x,y,w,h;
} OPTION;

OPTION options[OPTION_COUNT];

//------------------------------------------------------------------------------
//
// GAME VARIABLES
//
//------------------------------------------------------------------------------

BITMAP *DoubleBuffer;
DATAFILE *data;
unsigned char game_state;
unsigned int score;
unsigned int body_length;
unsigned char current_level;
float speed;
bool music;

unsigned int target_score[10] = {
         200, 600, 1000, 1400, 1900, 2500, 3000, 3500, 4200, 5500 };

int corners[8][3] = {
    {2,0,3},{3,1,3},{1,0,4},{3,2,4},{2,3,1},{0,1,1},{1,3,2},{0,2,2}
};

unsigned int ready;
unsigned int item_delay;

//------------------------------------------------------------------------------
//
// FUNCTIONS
//
//------------------------------------------------------------------------------

void setupGame();
void processGame();
void shutdownGame();
void blitBuffer();
void writeTextFromInt(char* text, int value, int x, int y);
void drawTitle();
void writeGameInfo();
void processTitle();
void stretchSprite(BITMAP* dest, BITMAP* bmp, int x, int y, float k);
bool isMouseOption(int id);
void drawIns();
