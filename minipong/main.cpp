#define ACCELERATION    0.4
#define CPU_ACCELERATION  0.22
#define B_FRICTION      0.98
#define FRICTION        0.9
#define DX_INC          0.04
#define INTRO_DELAY     400
#define DELAY           200
const int num_trail = 120;

#include "data\\data.h"
#include "game.h"
#include "key.h"
#include "timer.h"
#include "pong.h"
#include "ball.h"

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
     install_timer();
     install_mouse();
     install_keyboard();
     show_mouse(NULL);
     srand(time(NULL));
     install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
     
     LOCK_VARIABLE(ticks);
     LOCK_FUNCTION(ticker);
     install_int_ex(ticker,BPS_TO_TIMER(100));
     
     set_color_depth(SCREEND);
     set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREENW, SCREENH, 0, 0);
     
     DoubleBuffer = create_bitmap(SCREENW, SCREENH);
     clear_to_color(DoubleBuffer, BLACK);
     
     Black = create_bitmap(SCREENW, SCREENH);
     clear_to_color(Black, BLACK);
     
     data = load_datafile("data\\data.dat");
     
     game_state = S_TITLE;
     
     P1.setCoor (10, SCREENH/2 - 30);
     CPU.setCoor (SCREENW-25, SCREENH/2 - 30);
     ball.setCoor (SCREENW/2 - 5, SCREENH/2 - 5);
     ball.setVel (3, 0);
     
     between = DELAY;
     intro = INTRO_DELAY;
     delay1 = 100;
     delay2 = 200;
     intro_trans = 0;
     title_trans = 255;
     select = 0;
     
     score[0] = 0;
     score[1] = 0;
     
     LOCK_FUNCTION(close_button_handler);
     set_close_button_callback(close_button_handler);
     
     r_midi = rand()%3+1;
     set_volume(255, 150);
     
}

//------------------------------------------------------------------------------

void processGame(){
     
  int ntick;
     
  while(game_state != S_SHUTDOWN){
    
    ntick = ticks;
    ticks = 0;
    
    for(;ntick>0;ntick--){
      logicProcess();
      keyPresses();
    }
    
    drawProcess();
    
    if(close_button_pressed) game_state = S_SHUTDOWN;
    
  }
  
}

//------------------------------------------------------------------------------

void logicProcess(){
     
  if(score[0]==10 | score[1]==10){
    if(score[0]==10) winner = 1;
    else winner = 2;
    game_state = S_WIN;
  }
     
  switch(game_state){
     
  case S_GAME : {
       
    if(midi && midi_pos<0){
     
     if(shu_num>=3){
       shu_list[0] = false;
       shu_list[1] = false;
       shu_list[2] = false;
       shu_num = 0;
     }
       
     r_midi = rand()%3+1;
         
     while(shu_list[r_midi-1] == true){
       r_midi = rand()%3+1;
     }
         
     midi = false;
    }
    
    if(!midi){
     play_midi((MIDI*)data[MIDI1+r_midi].dat, 0);
     shu_list[r_midi-1] = true;
     shu_num++;
     midi = true;
    }
       
    P1.handle_input();
    P1.move();
  
    if(p2) CPU.handle_input();
    else ball.AI(&CPU, &P1);
    CPU.move();
  
    ball.move();
    ball.collide(&P1);
    ball.collide(&CPU);
    
  } break;
  
  case S_BETWEEN : {
       
    if(midi && midi_pos<0){
     
     if(shu_num>=3){
       shu_list[0] = false;
       shu_list[1] = false;
       shu_list[2] = false;
       shu_num = 0;
     }
       
     r_midi = rand()%3+1;
         
     while(shu_list[r_midi-1] == true){
       r_midi = rand()%3+1;
     }
         
     midi = false;
    }
    
    if(!midi){
     play_midi((MIDI*)data[MIDI1+r_midi].dat, 0);
     shu_list[r_midi-1] = true;
     shu_num++;
     midi = true;
    }
    
    P1.handle_input();
    P1.move();
  
    if(p2) CPU.handle_input();
    else ball.AI(&CPU, &P1);
    CPU.move();
  
    if(between>0){ between--; }
    else { between = DELAY; game_state = S_GAME; }
  
  } break;
  
  case S_WIN : {
       
    if(midi && midi_pos<0){
     
     if(shu_num>=3){
       shu_list[0] = false;
       shu_list[1] = false;
       shu_list[2] = false;
       shu_num = 0;
     }
       
     r_midi = rand()%3+1;
         
     while(shu_list[r_midi-1] == true){
       r_midi = rand()%3+1;
     }
         
     midi = false;
    }
    
    if(!midi){
     play_midi((MIDI*)data[MIDI1+r_midi].dat, 0);
     shu_list[r_midi-1] = true;
     shu_num++;
     midi = true;
    }
       
    P1.handle_input();
    P1.move();
  
    if(p2) CPU.handle_input();
    CPU.move();
    
  } break;
  
  case S_INTRO : {
       
    if(intro > 0 && intro_trans < 256) intro_trans += 2;
    else if(intro > 0 && intro_trans == 256) intro--;
    else if(intro == 0 && intro_trans > 0) intro_trans -= 2;
    else if(intro == 0 && intro_trans == 0) game_state = S_DELAY2;
       
  } break;
  
  case S_DELAY1 : {
       
    if(delay1>0) delay1--;
    else game_state = S_INTRO;
       
  } break;
  
  case S_DELAY2 : {
       
    if(delay2>0) delay2--;
    else game_state = S_DELAY3;
       
  } break;
  
  case S_DELAY3 : {
       
    if(title_trans>0) title_trans -= 2;
    else game_state = S_TITLE;
       
  } break;
  
  case S_DELAY4 : {
       
    if(title_trans<255) title_trans += 2;
    else game_state = S_DELAY5;
       
  } break;
  
  case S_DELAY5 : {
       
    if(title_trans>0) title_trans -= 2;
    else game_state = S_BETWEEN;
       
  } break;
  
  case S_DELAY6 : {
       
    if(title_trans<255) title_trans += 2;
    else game_state = S_SHUTDOWN;
       
  } break;
  
  }

}

//------------------------------------------------------------------------------

void drawProcess(){
  
  switch(game_state){
                     
    case S_GAME : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("%d", score[0], 70, 90, makecol(15,15,15));
      writeTextFromInt("%d", score[1], SCREENW-90, 90, makecol(15,15,15));
      
      P1.draw();
      CPU.draw();
      ball.draw();
      
      if(!thesound) writeTextFromInt2("sound:off", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("sound:on", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      
      if(!thebg) writeTextFromInt2("music:off", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("music:on", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      
    } break;
    
    case S_WIN : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("%d", score[0], 70, 90, WHITE);
      writeTextFromInt("%d", score[1], SCREENW-90, 90, WHITE);
      
      if(winner==1){
        writeTextFromInt("p1 wins", score[0], 92, 55, WHITE);
        writeTextFromInt("%d", score[0], 70, 90, WHITE);
        writeTextFromInt("%d", score[1], SCREENW-90, 90, makecol(15,15,15));
      }
      
      else if(winner==2 && p2){
        writeTextFromInt("p2 wins", score[0], 83, 55, WHITE);
        writeTextFromInt("%d", score[0], 70, 90, makecol(15,15,15));
        writeTextFromInt("%d", score[1], SCREENW-90, 90, WHITE);
      }
      
      else if(winner==2){
        writeTextFromInt("cpu wins", score[0], 74, 55, WHITE);
        writeTextFromInt("%d", score[0], 70, 90, makecol(15,15,15));
        writeTextFromInt("%d", score[1], SCREENW-90, 90, WHITE);
      }
      
      P1.draw();
      CPU.draw();
      
      if(!thesound) writeTextFromInt2("sound:off", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("sound:on", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      
      if(!thebg) writeTextFromInt2("music:off", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("music:on", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
         
    } break;
    
    case S_BETWEEN : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("%d", score[0], 70, 90, WHITE);
      writeTextFromInt("%d", score[1], SCREENW-90, 90, WHITE);
      
      P1.draw();
      CPU.draw();
      ball.draw();
      
      if(!thesound) writeTextFromInt2("sound:off", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("sound:on", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      
      if(!thebg) writeTextFromInt2("music:off", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("music:on", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      
    } break;
    
    case S_PAUSE : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("%d", score[0], 70, 90, makecol(15,15,15));
      writeTextFromInt("%d", score[1], SCREENW-90, 90, makecol(15,15,15));
      
      P1.draw();
      CPU.draw();
      ball.draw();
      
      if(!thesound) writeTextFromInt2("sound:off", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("sound:on", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      
      if(!thebg) writeTextFromInt2("music:off", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("music:on", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      
      fadeIn(Black, 180, 0, 0);
      
      writeTextFromInt("PAUSED", score[0], 87, 90, makecol(190,190,190));
      
    } break;
    
    case S_INTRO : {
         
      fadeIn((BITMAP*)data[INTRO].dat, intro_trans, 0, 0);
      
    } break;
    
    case S_TITLE : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("MINIPONG", score[0], 75, 84, makecol(220,220,220));
      writeTextFromInt2("v1.1", score[0], 240, 82, makecol(220,220,220));
      
      int index;
      
      for(index=0; index<2; index++){
        if(selection_1[index])
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(200,200,200));
        else
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(80,80,80));
      }
         
    } break;
    
    case S_TITLE2 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("MINIPONG", score[0], 75, 84, makecol(220,220,220));
      writeTextFromInt2("v1.1", score[0], 240, 82, makecol(220,220,220));
      
      int index;
      
      for(index=0; index<2; index++){
        if(selection_2[index])
          writeTextFromInt2(s_text_2[index], score[0], sx[index], 120, makecol(200,200,200));
        else
          writeTextFromInt2(s_text_2[index], score[0], sx[index], 120, makecol(80,80,80));
      }
         
    } break;
    
    case S_DELAY3 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  

      writeTextFromInt("MINIPONG", score[0], 75, 84, makecol(220,220,220));
      writeTextFromInt2("v1.1", score[0], 240, 82, makecol(220,220,220));
      
      int index;
      
      for(index=0; index<2; index++){
        if(selection_1[index])
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(200,200,200));
        else
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(80,80,80));
      }

      fadeIn(Black, title_trans, 0, 0);
         
    } break;
    
    case S_DELAY4 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("MINIPONG", score[0], 75, 84, makecol(220,220,220));
      writeTextFromInt2("v1.1", score[0], 240, 82, makecol(220,220,220));
      
      int index;
      
      for(index=0; index<2; index++){
        if(selection_2[index])
          writeTextFromInt2(s_text_2[index], score[0], sx[index], 120, makecol(200,200,200));
        else
          writeTextFromInt2(s_text_2[index], score[0], sx[index], 120, makecol(80,80,80));
      }

      fadeIn(Black, title_trans, 0, 0);
         
    } break;
    
    case S_DELAY5 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  
         
      writeTextFromInt("%d", score[0], 70, 90, makecol(15,15,15));
      writeTextFromInt("%d", score[1], SCREENW-90, 90, makecol(15,15,15));
      
      P1.draw();
      CPU.draw();
      ball.draw();
      
      if(!thesound) writeTextFromInt2("sound:off", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("sound:on", score[0], SCREENW-80, SCREENH-14, makecol(200,200,200));
      
      if(!thebg) writeTextFromInt2("music:off", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));
      writeTextFromInt2("music:on", score[0], SCREENW-160, SCREENH-14, makecol(200,200,200));

      fadeIn(Black, title_trans, 0, 0);
         
    } break;
    
    case S_DELAY6 : {
         
      blit((BITMAP*)data[BG].dat, DoubleBuffer, 0, 0, 0, 0, SCREENW, SCREENH);  

      writeTextFromInt("MINIPONG", score[0], 75, 84, makecol(220,220,220));
      writeTextFromInt2("v1.1", score[0], 240, 82, makecol(220,220,220));
      
      int index;
      
      for(index=0; index<2; index++){
        if(selection_1[index])
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(200,200,200));
        else
          writeTextFromInt2(s_text_1[index], score[0], sx[index], 120, makecol(80,80,80));
      }

      fadeIn(Black, title_trans, 0, 0);
         
    } break;
    
  }
  
  blitBuffer();
  
}

//------------------------------------------------------------------------------

void keyPresses(){
   
  //if(game_state != S_GAME | game_state != S_PAUSE | game_state != S_BETWEEN)  
  //EXIT_KEY game_state = S_SHUTDOWN;
  
  switch(game_state){
  
    case S_GAME : {
         
      if(keyPressed(KEY_P)){ game_state = S_PAUSE; midi_pause(); }
      EXIT_KEY { stop_midi(); game_state = S_DELAY3; restart(); }
      if(keyPressed(KEY_R)){ restart(); game_state = S_DELAY4; }
      
      if(keyPressed(KEY_1)){
        if(thesound) thesound = false;
        else thesound = true;
      }
      
      if(keyPressed(KEY_2)){
        if(thebg){ set_volume(255, 0); thebg = false; }
        else { set_volume(255, 150); thebg = true; }
      }
      
    } break;
    
    case S_WIN : {
         
      EXIT_KEY { stop_midi(); game_state = S_DELAY3; restart(); }
      if(keyPressed(KEY_R)){ restart(); game_state = S_DELAY4; }
      
      if(keyPressed(KEY_1)){
        if(thesound) thesound = false;
        else thesound = true;
      }
      
      if(keyPressed(KEY_2)){
        if(thebg){ set_volume(255, 0); thebg = false; }
        else { set_volume(255, 150); thebg = true; }
      }
    
    } break;
    
    case S_PAUSE : {
      
      if(keyPressed(KEY_P)){ game_state = S_GAME; midi_resume(); }
      EXIT_KEY { stop_midi(); game_state = S_DELAY3; restart(); }
      
      if(keyPressed(KEY_1)){
        if(thesound) thesound = false;
        else thesound = true;
      }
      
      if(keyPressed(KEY_2)){
        if(thebg){ set_volume(255, 0); thebg = false; }
        else { set_volume(255, 150); thebg = true; }
      }
      
    } break;
    
    case S_BETWEEN : {
         
      EXIT_KEY { stop_midi(); game_state = S_DELAY3; restart(); }
      if(keyPressed(KEY_R)){ restart(); game_state = S_DELAY4; }
      
      if(keyPressed(KEY_1)){
        if(thesound) thesound = false;
        else thesound = true;
      }
      
      if(keyPressed(KEY_2)){
        if(thebg){ set_volume(255, 0); thebg = false; }
        else { set_volume(255, 150); thebg = true; }
      }
      
    } break;
    
    case S_TITLE : {
      
      if(keyPressed(KEY_RIGHT)){
        if(select<1) select++;
        else select = 0;
        
        selection_1[0] = false;
        selection_1[1] = false;
        selection_1[select] = true;
        
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);
      }
        
      if(keyPressed(KEY_LEFT)){
        if(select>0) select--;
        else select = 1;
        
        selection_1[0] = false;
        selection_1[1] = false;
        selection_1[select] = true;
        
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);
      }
      
      if(keyPressed(KEY_ENTER)){
                          
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);    
        if(select == 0) game_state = S_TITLE2;
        else if(select ==  1) game_state = S_DELAY6;
                                
      }
      
      EXIT_KEY game_state = S_DELAY6;
      
    } break;
    
    case S_TITLE2 : {
      
      if(keyPressed(KEY_RIGHT)){
        if(select<1) select++;
        else select = 0;
        
        selection_2[0] = false;
        selection_2[1] = false;
        selection_2[select] = true;
        
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);
      }
        
      if(keyPressed(KEY_LEFT)){
        if(select>0) select--;
        else select = 1;
        
        selection_2[0] = false;
        selection_2[1] = false;
        selection_2[select] = true;
        
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);
      }
      
      if(keyPressed(KEY_ENTER)){
        
        play_sample((SAMPLE*)data[WAV2].dat, 255, 0, 1000, 0);
        if(select == 0){ game_state = S_DELAY4; p2 = false; }
        else if(select ==  1){ game_state = S_DELAY4; p2 = true; }
                                
      }
      
      EXIT_KEY game_state = S_TITLE;
      
    } break;
  
  }

}

//------------------------------------------------------------------------------

void shutdownGame(){
     
  destroy_bitmap(DoubleBuffer);
  destroy_bitmap(Black);
  unload_datafile(data);
  
}

//------------------------------------------------------------------------------

void blitBuffer(){
     
     blit(DoubleBuffer, screen, 0, 0, 0, 0, SCREENW, SCREENH);
     clear_to_color(DoubleBuffer, BLACK);
     
}

//------------------------------------------------------------------------------

void writeTextFromInt(char* text, int value, int x, int y, int col){
  char thetext[64];
  sprintf(thetext, text, value);
  
  textout_ex(DoubleBuffer,
             (FONT*)data[MYFONT].dat,
             thetext,
             x,
             y, 
             col,
             -1
             );
}

//------------------------------------------------------------------------------

void writeTextFromInt2(char* text, int value, int x, int y, int col){
  char thetext[64];
  sprintf(thetext, text, value);
  
  textout_ex(DoubleBuffer,
             (FONT*)data[MYFONT2].dat,
             thetext,
             x,
             y, 
             col,
             -1
             );
}

//------------------------------------------------------------------------------

void fadeIn(BITMAP* bmp, int trans, int x, int y){
     
  set_trans_blender(0, 0, 0, trans);
  drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
  draw_trans_sprite(DoubleBuffer, bmp, x, y);
  solid_mode();
  
}

//------------------------------------------------------------------------------

void restart(){
     
  P1.setCoor (10, SCREENH/2 - 30);
  CPU.setCoor (SCREENW-25, SCREENH/2 - 30);
  ball.setCoor (SCREENW/2 - 5, SCREENH/2 - 5);
  ball.setVel (3, 0);
     
  score[0] = 0;
  score[1] = 0;
  winner = 0;
     
  shu_list[0] = false;
  shu_list[1] = false;
  shu_list[2] = false;
  shu_num = 0;
  
  title_trans = 255;
  
  selection_1[0] = true;
  selection_1[1] = false;
  selection_2[0] = true;
  selection_2[1] = false;
  select = 0;
     
}
