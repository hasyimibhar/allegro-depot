#include "game.h"
#include "timer.h"
#include "map.h"
#include "snake.h"

//------------------------------------------------------------------------------
//
// MAIN FUNCTION
//
//------------------------------------------------------------------------------

int main(){
    
  setupGame();
  processGame();
  shutdownGame();
  return 0;
  
}
END_OF_MAIN();

//------------------------------------------------------------------------------
//
// SETUP GAME
//
//------------------------------------------------------------------------------

void setupGame(){
     
  // ALLEGRO, KEYBOARD, TIMER, MOUSE, SRAND INIT, SOUND
  allegro_init();
  install_keyboard();
  install_timer();
  install_mouse();
  show_mouse(NULL);
  srand(time(NULL));
  install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
  
  // COLOR DEPTH, SCREEN, DOUBLEBUFFER SETTING
  set_color_depth(SCREEN_DEPTH);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
  DoubleBuffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
  clear_to_color(DoubleBuffer, makecol(255,255,255));
  
  // LOAD DATAFILE
  data = load_datafile("data\\data.dat");
  
  // TIMER SETTING
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker,BPS_TO_TIMER(10));
  
  // GAME VARIABLES SETTING
  body_length = 2;
  game_state = STATE_TITLE;
  current_level = 1;
  speed = S_SLOW;
  
  int o;
  for(o=0; o < OPTION_COUNT; o++){
    options[o].id = OPTION1+o;
    options[o].w = ((BITMAP*)data[OPTION1+o].dat)->w;
    options[o].h = ((BITMAP*)data[OPTION1+o].dat)->h;
    options[o].x = SCREEN_WIDTH/2 - (((BITMAP*)data[OPTION1+o].dat)->w/2);
    options[o].y = OP_YPOS + o*50;
  }
  
  play_sample((SAMPLE*)data[SOUND1].dat, 150, 0, 1000, 1);
  music = true;

}

//------------------------------------------------------------------------------
//
// PROCESS GAME; MAIN LOOP
//
//------------------------------------------------------------------------------

void processGame(){
     
  int ntick;
  
  while(game_state != STATE_SHUTDOWN){
                   
    if(keyPressed(KEY_1) && !music){
      play_sample((SAMPLE*)data[SOUND1].dat, 150, 0, 1000, 1);
      music = true;
    }
                   
    if(keyPressed(KEY_2) && music){
      stop_sample((SAMPLE*)data[SOUND1].dat);
      music = false;
    }
                   
  switch(game_state){
                     
    case STATE_LOAD : {
      loadMap(current_level);
      putRandom(5);
      ready = 20;
      item_delay = ITEM_TIME;
      game_state = STATE_READY;
    } break;
                     
    case STATE_TITLE : {
      processTitle();
      writeTextFromInt("Copyright Fishcake 2007. All right reserved.", current_level, 10, SCREEN_HEIGHT - 10);
    } break;
    
    case STATE_INS : {
      drawIns();
    } break;
    
    case STATE_GAME : {
         
      drawMap();
      snakeDraw();
      writeGameInfo();
      
    } break;
    
    case STATE_PAUSED : {
      drawMap();
      snakeDraw();
      draw_sprite(DoubleBuffer,
                  (BITMAP*)data[PAUSED].dat,
                  SCREEN_WIDTH/2 - 85,
                  SCREEN_HEIGHT/2 - 17
                  );
      writeGameInfo();
    } break;
    
    case STATE_READY : {
         
      drawMap();
      snakeDraw();
      
      writeGameInfo();
    
      // READY...GO!!
      if(ready>0){
        if(ready>10){
          draw_sprite(DoubleBuffer,
                      (BITMAP*)data[READY].dat,
                      SCREEN_WIDTH/2 - 85,
                      SCREEN_HEIGHT/2 - 17
                      );
        } else {
          draw_sprite(DoubleBuffer,
                      (BITMAP*)data[GO].dat,
                      SCREEN_WIDTH/2 - 85,
                      SCREEN_HEIGHT/2 - 17
                      );
        }
        
      }
    
    } break;
    
    case STATE_GAMEOVER : {
    
      drawMap();
      snakeDraw();
      writeGameInfo();
      draw_sprite(DoubleBuffer, (BITMAP*)data[GAME_OVER].dat, SCREEN_WIDTH/2 - 85, SCREEN_HEIGHT/2 - 17 );
      
    } break;
    
    case STATE_BETWEEN : { 
      drawMap();
      snakeDraw();
      draw_sprite(DoubleBuffer, (BITMAP*)data[BETWEEN].dat, SCREEN_WIDTH/2 - 85, SCREEN_HEIGHT/2 - 17 );
      writeGameInfo();
    } break;
    
  }
    
    ntick=ticks;
    ticks=0;
    
    for(;ntick>0;ntick--){   
      
      switch(game_state){
                         
        case STATE_TITLE : { EXIT_KEY game_state = STATE_SHUTDOWN; } break;
                         
        case STATE_READY : {
          
          if(ready>0){
            ready--;
          } else {
            game_state = STATE_GAME;
          }
          
        } break;
        
        case STATE_PAUSED : { EXIT_KEY resetGame(); PAUSE_KEY game_state = STATE_GAME; } break;
                         
        case STATE_GAME : {

          if(item_delay>0){
            item_delay--;
          } else {
            clearItem();   
            putRandom(5);   
            item_delay = ITEM_TIME;
          }
          
          UP_KEY { snakeMove(UP); } else
          DOWN_KEY { snakeMove(DOWN); } else
          RIGHT_KEY { snakeMove(RIGHT); } else
          LEFT_KEY { snakeMove(LEFT); } else
          { snakeMove( snake[0].dir ); }
          
          PAUSE_KEY game_state = STATE_PAUSED;
          EXIT_KEY resetGame();
          
          if(score >= target_score[current_level-1]) game_state = STATE_BETWEEN;
          
        } break;
        
        case STATE_BETWEEN : { EXIT_KEY game_state = STATE_TITLE; CONTINUE_KEY { restartGame(); if(current_level<10)current_level++; else resetGame(); } } break;
        
        case STATE_GAMEOVER : { EXIT_KEY resetGame(); RESTART_KEY { restartGame(); current_level = 1; score = 0; } } break;
        
      }
      
    }
    
    blitBuffer();
    
  }
  
}

//------------------------------------------------------------------------------
//
// SHUTDOWN GAME
//
//------------------------------------------------------------------------------

void shutdownGame(){

  destroy_bitmap(DoubleBuffer);
  
  // UNLOAD DATAFILE
  unload_datafile(data);

  
}

//------------------------------------------------------------------------------ 
//
// BLIT BUFFER
//
//------------------------------------------------------------------------------

void blitBuffer(){
     
  draw_sprite(DoubleBuffer, mouse_sprite, mouse_x, mouse_y);
  blit(DoubleBuffer, screen, 0, 0, 0, 0, DoubleBuffer->w, DoubleBuffer->h);
  clear_to_color(DoubleBuffer, makecol(255,255,255));
  
}

//------------------------------------------------------------------------------
//
// WRITE TEXT FROM INT
//
//------------------------------------------------------------------------------

void writeTextFromInt(char* text, int value, int x, int y){
  char thetext[64];
  sprintf(thetext, text, value);
  
  textout_ex(DoubleBuffer,
             font,
             thetext,
             x,
             y, 
             makecol(0,0,0),
             -1
             );
}

//------------------------------------------------------------------------------
//
// PROCESS TITLE SCREEN
//
//------------------------------------------------------------------------------

void processTitle(){
     
  blit((BITMAP*)data[TITLE].dat,
       DoubleBuffer,
       0,0,0,0, SCREEN_WIDTH, SCREEN_HEIGHT
       );
       
  unsigned char active;
  
  int index;
  for(index=0; index < OPTION_COUNT; index++)
  
  if(!isMouseOption(index)){
  
    draw_sprite(DoubleBuffer,
                (BITMAP*)data[options[index].id].dat,
                options[index].x,
                options[index].y
                );
              
  } else {
  
    stretchSprite(DoubleBuffer,
                   (BITMAP*)data[options[index].id].dat,
                   options[index].x,
                   options[index].y,
                   1.4
                   );
                   
    active = index+1;
                   
  }
  
  if(mouse_b & 1)
  switch(active){
  
    case OP_STARTGAME : { game_state = STATE_LOAD; play_sample((SAMPLE*)data[SOUND3].dat, 150, 0, 1000, 0); } break;
    case OP_INSTRUCTIONS : { game_state = STATE_INS; play_sample((SAMPLE*)data[SOUND3].dat, 150, 0, 1000, 0); } break;
    case OP_EXITGAME : { game_state = STATE_SHUTDOWN; play_sample((SAMPLE*)data[SOUND3].dat, 150, 0, 1000, 0); } break;
  
  }

}

//------------------------------------------------------------------------------
//
// DRAW INSTRUCTIONS
//
//------------------------------------------------------------------------------

void drawIns(){
  
  blit((BITMAP*)data[TITLE].dat,
       DoubleBuffer,
       0,0,0,0, SCREEN_WIDTH, SCREEN_HEIGHT
       );
       
       
  int index;
  for(index=0; index < OPTION_COUNT; index++)
    draw_sprite(DoubleBuffer,
                (BITMAP*)data[options[index].id].dat,
                options[index].x,
                options[index].y
                );
                
  draw_sprite(DoubleBuffer,
                (BITMAP*)data[IN1].dat,
                50,
                100
                );
                
  draw_sprite(DoubleBuffer,
                (BITMAP*)data[IN2].dat,
                330,
                100
                );
                
  EXIT_KEY game_state = STATE_TITLE;
  
}

//------------------------------------------------------------------------------
//
// WRITE GAME INFO
//
//------------------------------------------------------------------------------

void writeGameInfo(){
  writeTextFromInt("LEVEL %d", current_level, 10, SCREEN_HEIGHT - 10);
  writeTextFromInt("SCORE : %d", score, 100, SCREEN_HEIGHT - 10);
  writeTextFromInt("NEXT LEVEL SCORE : %d", target_score[current_level-1], 250, SCREEN_HEIGHT - 10);
}

//------------------------------------------------------------------------------
//
// SCRETCH SPRITE
//
//------------------------------------------------------------------------------

void stretchSprite(BITMAP* dest, BITMAP* bmp, int x, int y, float k){
     
  int width = (bmp->w)*k;
  int height = (bmp->h)*k;
  
  int rx = x - (width - bmp->w)/2;
  int ry = y - (height - bmp->h)/2;
  
  stretch_sprite(dest, bmp, rx, ry, width, height);
  
}

//------------------------------------------------------------------------------
//
// IF MOUSE OVER
//
//------------------------------------------------------------------------------

bool isMouseOption(int id){
  
  int the_x = options[id].x;
  int the_y = options[id].y;
  int most_left = options[id].x + options[id].w;
  int most_bottom = options[id].y + options[id].h;
  
  if( (mouse_x > the_x) && (mouse_x < most_left) && (mouse_y > the_y) && (mouse_y < most_bottom) )
  return true;
  else return false;
  
}
