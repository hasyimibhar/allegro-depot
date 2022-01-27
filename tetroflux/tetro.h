//------------------------------------------------------------------------------

typedef struct TILE {
  unsigned char id;
} TILE;

typedef struct MAP {
  TILE cell[24][20];
} MAP;

MAP g_area;

void resetArea(){
  int i,j;
  for(i=0; i<24; i++)
    for(j=0; j<20; j++)
      g_area.cell[i][j].id = 0;
}

//------------------------------------------------------------------------------

typedef struct Coors {
  char x[4];
  char y[4];
} Coors;

//------------------------------------------------------------------------------

class Tetro {
      
  private:
    unsigned char x, y, id, rot;
    Coors coor;
    Coors w_coor;
    
  public:
    Tetro();
    void draw();
    void drop();
    void handle_input();
    void setCoors();
    void changePiece(int,int);
    bool detectSolid(int);
    bool rotatable();
    void checkRow();
    void tetroFlash();
         
};

//------------------------------------------------------------------------------

Tetro::Tetro (){
  x = 8;
  y = 0;
  id = rand()%7;
  rot = rand()%4;
  int index;
  for(index=0; index<4; index++){
    coor.x[index] = -1;
    coor.y[index] = -1;
  }
  
}

//------------------------------------------------------------------------------

void Tetro::draw (){
     
  int index;
  for(index=0; index<4; index++)
    draw_sprite(DoubleBuffer, (BITMAP*)data[PIECE].dat, 70 + ((x+coor.x[index])*10), (y+coor.y[index])*10);
    
  for(index=0; index<4; index++)
    draw_sprite(DoubleBuffer,
                (BITMAP*)data[PIECE].dat,
                262 + (puzzle[next_id][next_rot][index*2]*10),
                45 + (puzzle[next_id][next_rot][(2*index)+1]*10)
                );     
    
}

void Tetro::drop (){
  if(!detectSolid(DOWN)) y++;
}

//------------------------------------------------------------------------------

void Tetro::handle_input (){
     
  int index;
  for(index=0; index<4; index++){
    w_coor.x[index] = coor.x[index] + x;
    w_coor.y[index] = coor.y[index] + y;
  }
     
  if(key[KEY_LEFT] && !detectSolid(LEFT)) x--;
  else if(key[KEY_RIGHT] && !detectSolid(RIGHT)) x++;
  
  if(key[KEY_DOWN] && !detectSolid(DOWN)) y++;
  
  if(keyPressed(KEY_UP) && !rotatable() ){
  
    if(rot==3) rot = 0;
    else rot++;
    
    changePiece(id,rot);
  
  }
  
}

//------------------------------------------------------------------------------

void Tetro::setCoors (){
  coor.x[0] = puzzle[id][rot][0];
  coor.y[0] = puzzle[id][rot][1];
  coor.x[1] = puzzle[id][rot][2];
  coor.y[1] = puzzle[id][rot][3];
  coor.x[2] = puzzle[id][rot][4];
  coor.y[2] = puzzle[id][rot][5];
  coor.x[3] = puzzle[id][rot][6];
  coor.y[3] = puzzle[id][rot][7];
}

//------------------------------------------------------------------------------

bool Tetro::detectSolid (int dir){

  int index;
  for(index=0; index<4; index++){
    w_coor.x[index] = coor.x[index] + x;
    w_coor.y[index] = coor.y[index] + y;
  }
  
  switch(dir){    
     
     case DOWN: {
     
      if(g_area.cell[ w_coor.y[0]+1 ][ w_coor.x[0] ].id == 1 |
         g_area.cell[ w_coor.y[1]+1 ][ w_coor.x[1] ].id == 1 |
         g_area.cell[ w_coor.y[2]+1 ][ w_coor.x[2] ].id == 1 |
         g_area.cell[ w_coor.y[3]+1 ][ w_coor.x[3] ].id == 1 |
         w_coor.y[0]+1 == 24 | w_coor.y[1]+1 == 24 | w_coor.y[2]+1 == 24 | w_coor.y[3]+1 == 24){
           
           for(index=0; index<4; index++){
             g_area.cell[ w_coor.y[index] ][ w_coor.x[index] ].id = 1;
           }
           
           x = 8;
           y = 0;
           id = next_id;
           rot = next_rot;
           changePiece(id,rot);
           next_id = rand()%7;
           next_rot = rand()%4;
           return true;
      
      } else {
        return false;   
      }
      
     } break;
     
     case RIGHT: {
     
      if(g_area.cell[ w_coor.y[0] ][ w_coor.x[0]+1 ].id == 1 |
         g_area.cell[ w_coor.y[1] ][ w_coor.x[1]+1 ].id == 1 |
         g_area.cell[ w_coor.y[2] ][ w_coor.x[2]+1 ].id == 1 |
         g_area.cell[ w_coor.y[3] ][ w_coor.x[3]+1 ].id == 1 |
         w_coor.x[0]+1 == 18 | w_coor.x[1]+1 == 18 | w_coor.x[2]+1 == 18 | w_coor.x[3]+1 == 18){
           
           return true;
      
      } else {
        return false;   
      }
      
     } break;
     
     case LEFT: {
     
      if(g_area.cell[ w_coor.y[0] ][ w_coor.x[0]-1 ].id == 1 |
         g_area.cell[ w_coor.y[1] ][ w_coor.x[1]-1 ].id == 1 |
         g_area.cell[ w_coor.y[2] ][ w_coor.x[2]-1 ].id == 1 |
         g_area.cell[ w_coor.y[3] ][ w_coor.x[3]-1 ].id == 1 |
         w_coor.x[0]-1 == -1 | w_coor.x[1]-1 == -1 | w_coor.x[2]-1 == -1 | w_coor.x[3]-1 == -1){
           
           return true;
      
      } else {
        return false;   
      }
      
     } break;
  
  }
  
}

//------------------------------------------------------------------------------

void Tetro::changePiece (int a, int b){
  id = a;
  rot = b;
  setCoors();
}

//------------------------------------------------------------------------------

bool Tetro::rotatable (){
     
  int index;
  for(index=0; index<4; index++){
    if(rot==3){
      w_coor.x[index] = puzzle[id][0][index*2] + x;
      w_coor.y[index] = puzzle[id][0][(2*index)+1] + y;
    } else {
      w_coor.x[index] = puzzle[id][rot+1][index*2] + x;
      w_coor.y[index] = puzzle[id][rot+1][(2*index)+1] + y;
    }
  }
     
  if(g_area.cell[ w_coor.y[0] ][ w_coor.x[0] ].id == 1 |
     g_area.cell[ w_coor.y[1] ][ w_coor.x[1] ].id == 1 |
     g_area.cell[ w_coor.y[2] ][ w_coor.x[2] ].id == 1 |
     g_area.cell[ w_coor.y[3] ][ w_coor.x[3] ].id == 1 |
     w_coor.x[0] == 18 | w_coor.x[1] == 18 | w_coor.x[2] == 18 | w_coor.x[3] == 18 |
     w_coor.x[0] == -1 | w_coor.x[1] == -1 | w_coor.x[2] == -1 | w_coor.x[3] == -1)
       return true;
       
  else return false;
       
}

//------------------------------------------------------------------------------

void Tetro::checkRow (){
     
  int a,b,c;
  int index = 0;
  clear_row = 0;
  for(a=24; a>0; a--){
    index++;
    c=0;
    for(b=0; b<18; b++) if(g_area.cell[a][b].id==1) c++;
    
    if(c==18){
      
      tetro_row = TETRO_ROW;
      tetro_row2 = 255;
      clear_row++;
      
      if(tetro_wipe>0){
        tetro_wipe--;
        tetro_flash += 6;
      } else {
        tetro_wipe = WIPE_DELAY;
        tetro_flash = 0;
        int d;
        for(b=a; b>1; b--)
          for(d=0; d<18; d++)
            g_area.cell[b][d].id = g_area.cell[b-1][d].id;
      }
      
    }
       
  }
  
  for(a=0; a<24; a++){
    c=0;
    for(b=0; b<18; b++) if(g_area.cell[a][b].id==1) c++;
    if(c==18) c_a = a;    
  }
  
  //if(tetro_flash==0 && index==24) tetro_wipe = WIPE_DELAY;
  
  if(clear_row>0 && index==24 && tetro_row2==255 && clear_row2==0){
    clear_row2 = clear_row;
    if(clear_row==1) score += 10;
    else score += clear_row*30;
  }
  
  if(tetro_row>0) {
    tetro_row--;
  } else if(tetro_row==0 && tetro_row2>0){
    tetro_row2 -= 4;
  } else if(tetro_row2<=0){
    clear_row2 = 0;
  }
  
  int e=0;
  for(b=0; b<18; b++) if(g_area.cell[3][b].id==1) e++;
  if(e>0) game_state = S_OVER;
     
}

//------------------------------------------------------------------------------

void Tetro::tetroFlash(){
  drawTrans((BITMAP*)data[LINE].dat, tetro_flash, 50, c_a*10);
  
  //int a,b,c;
  //for(a=0; a<24; a++){
  //
  //  c=0;
  //  for(b=0; b<18; b++) if(g_area.cell[a][b].id==1) c++;
  //  
  //  if(c==18) drawTrans((BITMAP*)data[LINE].dat, tetro_flash, 50, a*10);
  //     
  //}
  
  drawTrans((BITMAP*)data[ONE-1+clear_row2].dat, tetro_row2, row_pos[clear_row2-1], c_a*10);
}

//------------------------------------------------------------------------------

Tetro tetro;
