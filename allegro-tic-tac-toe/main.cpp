/* Tic-Tac-Toe -- Allegro 4.2.1 / MinGW32
 *
 * RECONSTRUCTED from Tic-Tac-Toe.exe by disassembly. The binary was built
 * unoptimised (-O0) and kept its COFF symbol table, so every function name,
 * signature and global slot below is recovered, not guessed. What could NOT
 * be recovered: comments, local variable names, #include layout, and the
 * original formatting. Local names here are inferred from use.
 */

#include <allegro.h>
#include "data/data.h"

/* ---- globals (recovered .bss slots, in address order) ---- */
BITMAP  *buffer;                /* 0x405010 */
DATAFILE *data;                 /* 0x405014 */

int  board[3][3];               /* 0x405030  0 = empty, 1 = player, 2 = CPU */
int  turn;                      /* 0x405054  1 = player, 2 = CPU           */
int  movesLeft;                 /* 0x405058  9 down to 0                   */
int  lastX;                     /* 0x40505c  last move, written never read */
int  lastY;                     /* 0x405060                                */
int  gameOver;                  /* 0x405064  0 = running, 255 = quit       */
int  winner;                    /* 0x405068  0 = none, else 1 or 2         */
int  difficulty;                /* 0x40506c  0 = stupid, 1 = normal        */
int  aiChoices;                 /* 0x405070  candidate moves this AI turn  */
volatile int counter;           /* 0x405074  ticker only writes it (unused)*/
char MyKeyArray[KEY_MAX];       /* 0x405078  key-debounce state            */

/* ---- the 8 winning lines, as {x,y} cell triples ---- */
/* Recovered byte-exact from .data at 0x403000. */
int winLine[8][3][2] = {
    { {0,0}, {0,1}, {0,2} },    /* column 0   */
    { {0,0}, {1,0}, {2,0} },    /* row 0      */
    { {0,2}, {1,2}, {2,2} },    /* row 2      */
    { {2,0}, {2,1}, {2,2} },    /* column 2   */
    { {0,0}, {1,1}, {2,2} },    /* diagonal \ */
    { {2,0}, {1,1}, {0,2} },    /* diagonal / */
    { {0,1}, {1,1}, {2,1} },    /* row 1      */
    { {1,0}, {1,1}, {1,2} }     /* column 1   */
};

/* ---- forward declarations ---- */
int  keyPressed(int k);
void ticker();
void drawBoard(int bx, int by);
void checkMouse();
void putSprite();
void putPiece();
void checkWin();
void restartGame();
void actAI();
void nextTurn();
void drawLine(int t, int x1, int y1, int x2, int y2, int col);
void printText(int x, int y, int col, int bg, char *s);
void playSound(int n, int loop);
void putRandom();
void normalAi();
void changeDiff(int d);
void setupGame();
void processGame();
void shutdownGame();
void drawScreen();
void drawRect(int x, int y);
void drawcolRect(int x, int y, int col);

/* ---- input ---- */
int keyPressed(int k)
{
    if (key[k] && !MyKeyArray[k]) {
        MyKeyArray[k] = 1;
        return 1;
    }
    if (!key[k] && MyKeyArray[k])
        MyKeyArray[k] = 0;
    return 0;
}

void ticker()
{
    counter++;
}

/* ---- drawing ---- */
void printText(int x, int y, int col, int bg, char *s)
{
    textout_ex(buffer, font, s, x, y, col, bg);
}

void drawRect(int x, int y)
{
    rect(buffer, x, y, x + 40, y + 40, makecol(0, 0, 0));
}

void drawcolRect(int x, int y, int col)
{
    rect(buffer, x,     y,     x + 40, y + 40, col);
    rect(buffer, x + 1, y + 1, x + 39, y + 39, col);
}

/* Draws a t-pixel-thick line between the centres of two board cells.
   Cell centre is (x*40+120, y*40+95): board origin (100,75) plus half a cell. */
void drawLine(int t, int x1, int y1, int x2, int y2, int col)
{
    int half = t / 2;

    for (int i = 0; i < t; i++) {
        if (y1 < y2 && x1 == x2)
            line(buffer, x1 * 40 - half + i + 120, y1 * 40 + 95,
                         x2 * 40 - half + i + 120, y2 * 40 + 95, col);
        else
            line(buffer, x1 * 40 + 120, y1 * 40 - half + i + 95,
                         x2 * 40 + 120, y2 * 40 - half + i + 95, col);
    }
}

void drawBoard(int bx, int by)
{
    char buf[64];
    char *mode;
    int col;

    if (difficulty == 0) {
        mode = "STUPID";
        col  = makecol(11, 157, 244);
    } else {
        mode = "NORMAL";
        col  = makecol(255, 0, 0);
    }

    sprintf(buf, "%s MODE", mode);
    printText(100, 65, col, makecol(255, 255, 255), buf);

    printText(15, 220, makecol(70, 70, 70), makecol(255, 255, 255),
              "[1] Stupid Mode      [2] Normal Mode");
    printText(15, 230, makecol(70, 70, 70), makecol(255, 255, 255),
              "[R] Restart Game     [ESC] Quit Game");

    blit((BITMAP *)data[TITLE].dat, buffer, 0, 0, 0, 0,
         ((BITMAP *)data[TITLE].dat)->w, ((BITMAP *)data[TITLE].dat)->h);

    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
            drawRect(bx + j * 40, by + i * 40);

    checkMouse();
    putSprite();
}

void checkMouse()
{
    int mx = (mouse_x - 100) / 40;
    int my = (mouse_y - 75) / 40;

    if (mx >= 0 && mx <= 2 && my >= 0 && my <= 2) {
        int col = makecol(15, 116, 240);
        drawcolRect(mx * 40 + 100, my * 40 + 75, col);
    }
}

void putSprite()
{
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            int p = board[i][j];
            if (p > 0) {
                int t = TILE0 - 1;      /* 1, so p==1 -> TILE0, p==2 -> TILE1 */
                /* NOTE: the binary centres on ->w for BOTH axes. Kept as-is. */
                draw_sprite(buffer, (BITMAP *)data[t + p].dat,
                            j * 40 + (40 - ((BITMAP *)data[t + p].dat)->w) / 2 + 100,
                            i * 40 + (40 - ((BITMAP *)data[t + p].dat)->w) / 2 + 75);
            }
        }
    }
}

void drawScreen()
{
    draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
    blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);
    clear_to_color(buffer, makecol(255, 255, 255));
}

/* ---- sound ---- */
void playSound(int n, int loop)
{
    int s = SOUND1;
    play_sample((SAMPLE *)data[s + n].dat, 255, 0, 1000, loop);
}

/* ---- rules ---- */
void nextTurn()
{
    if (winner == 0) {
        turn++;
        if (turn > 2)
            turn = 1;
    }
}

void checkWin()
{
    for (int n = 0; n <= 7; n++) {
        int x1 = winLine[n][0][0], y1 = winLine[n][0][1];
        int x2 = winLine[n][1][0], y2 = winLine[n][1][1];
        int x3 = winLine[n][2][0], y3 = winLine[n][2][1];

        if (board[y1][x1] == turn &&
            board[y2][x2] == turn &&
            board[y3][x3] == turn) {
            winner = turn;
            int col = makecol(255, 0, 0);
            drawLine(5, x1, y1, x3, y3, col);
        }

        /* NOTE: these sit inside the loop in the binary, so the banner is
           drawn up to 8 times per call -- same spot, so it is invisible. */
        if (winner == 1)
            printText(100, 199, makecol(255, 0, 0), makecol(255, 255, 255), "P1 WINS!");
        else if (winner == 2)
            printText(100, 199, makecol(255, 0, 0), makecol(255, 255, 255), "CPU WINS!");
        else if (winner == 0 && movesLeft == 0)
            printText(100, 199, makecol(255, 0, 0), makecol(255, 255, 255), "DRAW!");
    }
}

void putPiece()
{
    int mx = (mouse_x - 100) / 40;
    int my = (mouse_y - 75) / 40;

    if (mx >= 0 && mx <= 2 && my >= 0 && my <= 2 &&
        board[my][mx] == 0 && winner == 0) {
        playSound(1, 0);
        board[my][mx] = turn;
        lastX = mx;
        lastY = my;
        movesLeft--;
        checkWin();
        nextTurn();
    }
}

void restartGame()
{
    for (int i = 0; i <= 2; i++)
        for (int j = 0; j <= 2; j++)
            board[i][j] = 0;

    /* loser of the last round starts; first round is a coin flip */
    if (winner > 0)
        turn = winner;
    else
        turn = rand() % 2 + 1;

    winner    = 0;
    movesLeft = 9;
}

/* ---- AI ---- */
void putRandom()
{
    bool done = false;

    while (!done) {
        int x = rand() % 3;
        int y = rand() % 3;

        if (board[y][x] == 0) {
            board[y][x] = 2;
            lastX = x;
            lastY = y;
            movesLeft--;
            done = true;
            checkWin();
            nextTurn();
        }
    }
}

/* For each of the 8 lines, count +1 per player piece and -1 per CPU piece.
   A count of exactly 2 means "two player pieces and one empty cell", i.e. a
   line the player is about to win -- so block it. Note this only ever blocks;
   it never takes its own win. */
void normalAi()
{
    int cand[8][2];
    int count = 0;

    aiChoices = 0;

    for (int n = 0; n <= 7; n++) {
        int ex = 0, ey = 0;
        count = 0;

        for (int m = 0; m <= 2; m++) {
            int x = winLine[n][m][0];
            int y = winLine[n][m][1];

            if (board[y][x] == 1)
                count++;
            else if (board[y][x] == 0) {
                ex = x;
                ey = y;
            } else
                count--;
        }

        if (count == 2) {
            cand[n][0] = ex;
            cand[n][1] = ey;
            aiChoices++;
        } else {
            cand[n][0] = -1;
            cand[n][1] = -1;
        }
    }

    bool done = false;

    if (aiChoices > 0) {
        while (!done) {
            int k  = rand() % 8;
            int cx = cand[k][0];
            int cy = cand[k][1];

            if (cx != -1 && cy != -1) {
                board[cy][cx] = 2;
                lastX = cx;
                lastY = cy;
                movesLeft--;
                done = true;
                checkWin();
                nextTurn();
            }
        }
    } else {
        putRandom();
    }
}

void actAI()
{
    int d = difficulty;

    switch (d) {
    case 0:
        putRandom();
        break;
    case 1:
        normalAi();
        break;
    }

    rest(200);
}

void changeDiff(int d)
{
    if (difficulty != d) {
        difficulty = d;
        restartGame();
    }
}

/* ---- lifecycle ---- */
void setupGame()
{
    allegro_init();
    install_keyboard();
    install_timer();
    install_mouse();
    show_mouse(NULL);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

    set_color_depth(16);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 240, 0, 0);

    buffer = create_bitmap(320, 240);
    clear_to_color(buffer, makecol(255, 255, 255));

    data = load_datafile("data\\data.dat");

    install_int_ex(ticker, BPS_TO_TIMER(10));

    gameOver   = 0;
    difficulty = 0;
    turn       = 1;
    movesLeft  = 9;

    playSound(0, 1);            /* looping background music */
}

void processGame()
{
    /* The binary guards entry on gameOver == 0 and loops while it != 255.
       Several source spellings compile to this; behaviour is exact. */
    if (!gameOver) {
        while (gameOver != 255) {
            drawBoard(100, 75);
            checkWin();

            if (key[KEY_1])
                changeDiff(0);
            if (key[KEY_2])
                changeDiff(1);

            if (mouse_b & 1)
                putPiece();

            if (keyPressed(KEY_R))
                restartGame();

            if (winner == 0 && turn == 2 && movesLeft > 0)
                actAI();

            drawScreen();

            if (key[KEY_ESC])
                gameOver = 255;
        }
    }
}

void shutdownGame()
{
    unload_datafile(data);
    destroy_bitmap(buffer);
}

int main()
{
    setupGame();
    processGame();
    shutdownGame();
    return 0;
}
END_OF_MAIN()
