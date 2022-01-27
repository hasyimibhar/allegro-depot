#define MOVE_DELAY  20
#define WIPE_DELAY  40
#define TETRO_FLASH 240
#define TETRO_ROW   30
#define DELAY       200
#define C_DELAY     16

#include "game.h"
#include "puzzle.h"
#include "key.h"
#include "timer.h"
#include "tetro.h"
#include "circle.h"
#include "score.h"

//------------------------------------------------------------------------------

int main(){
    
    initGame();
    processGame();
    shutdownGame();
    return 0;
    
}
END_OF_MAIN();

//------------------------------------------------------------------------------

void initGame(){
     
     allegro_init();
     alfont_init();
     install_timer();
     install_mouse();
     install_keyboard();
     show_mouse(NULL);
     srand(time(NULL));
     install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
     
     register_png_datafile_object(DAT_PNG);
     myfont = alfont_load_font("data\\myfont.ttf");
     alfont_set_font_size(myfont, 26);
     
     LOCK_VARIABLE(ticks);
     LOCK_FUNCTION(ticker);
     install_int_ex(ticker,BPS_TO_TIMER(70));
     
     LOCK_VARIABLE(keyticks);
     LOCK_FUNCTION(keyticker);
     install_int_ex(keyticker,BPS_TO_TIMER(15));
     
     set_color_depth(SCREEND);
     set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREENW, SCREENH, 0, 0);
     
     DoubleBuffer = create_bitmap(SCREENW, SCREENH);
     clear_to_color(DoubleBuffer, WHITE);
     
     backRect = create_bitmap(180, 200);
     clear_to_color(backRect, WHITE);
     
     OB = create_bitmap(180, 40);
     clear_to_color(OB, makecol(137,167,205));
     
     next = create_bitmap(50, 50);
     clear_to_color(next, WHITE);
     
     ScoreBg = create_bitmap(60, 15);
     clear_to_color(ScoreBg, WHITE);
     
     PauseBg = create_bitmap(SCREENW, SCREENH);
     clear_to_color(PauseBg, BLACK);
     
     game_state = S_DELAY1;
     tetro.setCoors();
     tetro_move = MOVE_DELAY;
     tetro_wipe = WIPE_DELAY;
     tetro_flash = 0;
     tetro_row = TETRO_ROW;
     tetro_row2 = 0;
     c_a = 0;
     clear_row2 = 0;
     next_id = rand()%7;
     next_rot = rand()%4;
     
     LOCK_FUNCTION(close_button_handler);
     set_close_button_callback(close_button_handler);
     
     data = load_datafile("data\\data.dat");
     
     int a,b;
     for(a=0; a<24; a++)
       for(b=0; b<18; b++)
         g_area.cell[a][b].id = 0;
         
     for(a=0; a<16; a++){
       for(b=0; b<12; b++){
       
         //c[a][b].setR(15);
         c[a][b].setCol(makecol(30,30,30));
         c[a][b].setCoor(10 + (a*20), 10 + (b*20));
                
       }
     }
     
     circle_frame = 120;
     delay = DELAY;
     space_trans = 0;
     space_trans2 = 0;
     score = 0;
     
     resetHighScore();
     loadHighScore();
     reg_score = false;
     //saveHighScore();
     
}

//------------------------------------------------------------------------------

void processGame(){
     
  int ntick;
     
  while(game_state != S_SHUTDOWN){
    
    ntick = ticks;
    ticks = 0;
    
    for(;ntick>0;ntick--){
      logicProcess();
    }
    
    ntick = keyticks;
    keyticks = 0;
    
    for(;ntick>0;ntick--){
      keyPresses();
    }
    
    drawProcess();
    
    if(close_button_pressed) game_state = S_SHUTDOWN;
    
  }
  
}

//------------------------------------------------------------------------------

void logicProcess(){
     
  switch(game_state){
                     
    case S_GAME : {
     
      if(tetro_move>0){tetro_move--;}
      else {
        tetro_move = MOVE_DELAY;
        tetro.drop();
      }
      
      tetro.checkRow();
      
    } break;
    
    case S_DELAY1 : {
    
      if(circle_frame>-16) circle_frame --;
         
      int a,b;
      for(a=0; a<16; a++){
        for(b=0; b<12; b++){
          int e = 16 - a;
          if((e+circle_frame)>0) c[a][b].setR(e+circle_frame);
          else c[a][b].setR(0);
        }
      }
      
      if(circle_frame<=-16) game_state = S_TITLE;
    
    } break;
    
    case S_DELAY2 : {
         
      if(circle_frame<C_DELAY) circle_frame ++;
         
      int a,b;
      for(a=0; a<16; a++){
        for(b=0; b<12; b++){
          int e = 16 - a;
          if((e+circle_frame)>0) c[15-a][11-b].setR(e+circle_frame);
          else c[15-a][11-b].setR(0);
        }
      }
      
      if(circle_frame>=C_DELAY) game_state = S_DELAY3;
         
    } break;
    
    case S_DELAY3 : {
         
      if(circle_frame>-16) circle_frame --;
         
      int a,b;
      for(a=0; a<16; a++){
        for(b=0; b<12; b++){
          int e = 16 - a;
          if((e+circle_frame)>0) c[a][b].setR(e+circle_frame);
          else c[a][b].setR(0);
        }
      }
      
      if(circle_frame<=-16) game_state = S_MENU;
         
    } break;
    
    case S_DELAY4 : {
         
      if(circle_frame<C_DELAY) circle_frame ++;
         
      int a,b;
      for(a=0; a<16; a++){
        for(b=0; b<12; b++){
          int e = 16 - a;
          if((e+circle_frame)>0) c[15-a][11-b].setR(e+circle_frame);
          else c[15-a][11-b].setR(0);
        }
      }
      
      if(circle_frame>=C_DELAY) game_state = S_DELAY5;
      
      delay = 40;
         
    } break;
    
    case S_DELAY5 : {
         
      if(circle_frame>-16) circle_frame --;
         
      int a,b;
      for(a=0; a<16; a++){
        for(b=0; b<12; b++){
          int e = 16 - a;
          if((e+circle_frame)>0) c[a][b].setR(e+circle_frame);
          else c[a][b].setR(0);
        }
      }
      
      if(circle_frame<=-16 && delay>0) delay--;
      else if(circle_frame<=-16 && delay==0) game_state = S_GAME;
         
    } break;
    
    case S_MENU : {
         
      if(space_trans==0) space_trans2=0;
      if(space_trans==255) space_trans2=1;
      
      if(space_trans<255 && space_trans2==0) space_trans += 5;
      if(space_trans>0 && space_trans2==1) space_trans -= 5;
         
    } break;
    
    case S_TITLE : {
         
      if(delay>0){ delay--; }
      else { delay = DELAY; game_state = S_DELAY2; }
         
    } break;
    
    case S_OVER : {
         
      if(!reg_score){
        saveHighScore();
        reg_score = true;
      }
         
    } break;
    
  }

}

//------------------------------------------------------------------------------

void drawProcess(){
     
  switch(game_state){
                     
    case S_GAME : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans(backRect, 76, 70, 40);
      drawTrans(next, 76, 257, 40);
      drawTrans((BITMAP*)data[NEXT].dat, 153, 262, 27);
      
      drawTrans((BITMAP*)data[SCORE].dat, 153, 265, 147);
      drawTrans(ScoreBg, 76, 257, 159);
      writeScore(score, 262, 152, makecol(216,228,243));
      
      drawTrans((BITMAP*)data[HISCORE].dat, 153, 255, 180);
      drawTrans(ScoreBg, 76, 257, 192);
      writeScore(h_score, 262, 185, makecol(216,228,243));
      
      blit(OB, DoubleBuffer, 0, 0, 70, 0, 180, 40);
      
      tetro.draw ();
      drawPieces();
      tetro.tetroFlash();
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
        
    } break;
    
    case S_OVER : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans(backRect, 76, 70, 40);
      drawTrans(next, 76, 257, 40);
      drawTrans(ScoreBg, 76, 257, 159);
      drawTrans((BITMAP*)data[NEXT].dat, 153, 262, 27);
      drawTrans((BITMAP*)data[SCORE].dat, 153, 265, 147);
      blit(OB, DoubleBuffer, 0, 0, 70, 0, 180, 40);
      
      drawTrans((BITMAP*)data[HISCORE].dat, 153, 255, 180);
      drawTrans(ScoreBg, 76, 257, 192);
      writeScore(h_score, 262, 185, makecol(216,228,243));
      
      writeScore(score, 262, 152, makecol(216,228,243));
      
      drawPieces();
      drawTrans((BITMAP*)data[GAMEOVER].dat, 153, 120, 118);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
         
    } break;
    
    case S_DELAY1 : {
         
      blit((BITMAP*)data[INTRO].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      int a,b;
      for(a=0; a<16; a++)
        for(b=0; b<12; b++)
          c[a][b].draw();
    
    } break;
    
    case S_DELAY2 : {
         
      blit((BITMAP*)data[INTRO].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      int a,b;
      for(a=0; a<16; a++)
        for(b=0; b<12; b++)
          c[a][b].draw();
    
    } break;
    
    case S_DELAY3 : {
         
      blit((BITMAP*)data[MAINMENU].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
      int a,b;
      for(a=0; a<16; a++)
        for(b=0; b<12; b++)
          c[a][b].draw();
    
    } break;
    
    case S_DELAY4 : {
         
      blit((BITMAP*)data[MAINMENU].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
      int a,b;
      for(a=0; a<16; a++)
        for(b=0; b<12; b++)
          c[a][b].draw();
    
    } break;
    
    case S_DELAY5 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans(backRect, 76, 70, 40);
      drawTrans(next, 76, 257, 40);
      drawTrans(ScoreBg, 76, 257, 159);
      drawTrans((BITMAP*)data[NEXT].dat, 153, 262, 27);
      drawTrans((BITMAP*)data[SCORE].dat, 153, 265, 147);
      
      drawTrans((BITMAP*)data[HISCORE].dat, 153, 255, 180);
      drawTrans(ScoreBg, 76, 257, 192);
      
      blit(OB, DoubleBuffer, 0, 0, 70, 0, 180, 40);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
      int a,b;
      for(a=0; a<16; a++)
        for(b=0; b<12; b++)
          c[a][b].draw();
    
    } break;
    
    case S_MENU : {
         
      blit((BITMAP*)data[MAINMENU].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans((BITMAP*)data[SPACE].dat, space_trans, 74, 151);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
         
    } break;
    
    case S_TITLE : {
         
      blit((BITMAP*)data[INTRO].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
     
    } break;
    
    case S_PAUSE : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);
      drawTrans(backRect, 76, 70, 40);
      drawTrans(next, 76, 257, 40);
      drawTrans(ScoreBg, 76, 257, 159);
      drawTrans((BITMAP*)data[NEXT].dat, 153, 262, 27);
      drawTrans((BITMAP*)data[SCORE].dat, 153, 265, 147);
      
      drawTrans((BITMAP*)data[HISCORE].dat, 153, 255, 180);
      drawTrans(ScoreBg, 76, 257, 192);
      writeScore(h_score, 262, 185, makecol(216,228,243));
      
      blit(OB, DoubleBuffer, 0, 0, 70, 0, 180, 40);
      
      writeScore(score, 262, 152, makecol(216,228,243));
      
      tetro.draw ();
      drawPieces();
      tetro.tetroFlash();
      drawTrans(PauseBg, 38, 0, 0);
      drawTrans((BITMAP*)data[PAUSE].dat, 180, 112, 117);
      drawTrans((BITMAP*)data[SCAN].dat, 51, 0, 0);
        
    } break;
                     
  }
  
  blitBuffer();
  
}

//------------------------------------------------------------------------------

void keyPresses(){
  
  switch(game_state){
                     
    case S_GAME : {
         
      tetro.handle_input();
      if(keyPressed(KEY_P)) game_state = S_PAUSE;
         
    } break;
    
    case S_MENU : {
         
      if(keyPressed(KEY_SPACE)) game_state = S_DELAY4;
         
    } break;
    
    case S_PAUSE : {
         
      if(keyPressed(KEY_P)) game_state = S_GAME;
         
    } break;
    
    case S_OVER : {
         
      if(keyPressed(KEY_R)){
        resetArea();
        score = 0;
        game_state = S_DELAY5;
      }
         
    }
                     
  }

  EXIT_KEY game_state = S_SHUTDOWN;

}

//------------------------------------------------------------------------------

void shutdownGame(){
     
  destroy_bitmap(DoubleBuffer);
  destroy_bitmap(backRect);
  destroy_bitmap(OB);
  destroy_bitmap(next);
  destroy_bitmap(ScoreBg);
  destroy_bitmap(PauseBg);
  unload_datafile(data);
  alfont_destroy_font(myfont);
  alfont_exit();
  allegro_exit();
  
}

//------------------------------------------------------------------------------

void blitBuffer(){
     
     blit(DoubleBuffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
     clear_to_color(DoubleBuffer, WHITE);
     
}

//------------------------------------------------------------------------------

void drawTrans(BITMAP* bmp, int trans, int x, int y){
     
  set_trans_blender(0, 0, 0, trans);
  drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
  draw_trans_sprite(DoubleBuffer, bmp, x, y);
  solid_mode();
  
}

//------------------------------------------------------------------------------

void drawPieces(){
     
  int a,b;
  for(a=0; a<24; a++)
    for(b=0; b<18; b++)
      if(g_area.cell[a][b].id==1)
        blit((BITMAP*)data[PIECE].dat, DoubleBuffer, 0, 0, 70 + (b*10), a*10, 10, 10);
     
}

//------------------------------------------------------------------------------

void writeScore(int value, int x, int y, int col){
  char thetext[64];
  sprintf(thetext, "%d", value);
  alfont_textout_aa_ex(DoubleBuffer, myfont, thetext, x, y, col, -1);
}

//------------------------------------------------------------------------------
