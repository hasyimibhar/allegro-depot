bool isHit(int x, int y);
void itemEffect(int x, int y);
void setSpeed(float number);

//------------------------------------------------------------------------------
//
// RANDOM ITEM PLACEMENT
//
//------------------------------------------------------------------------------

void putRandom(int number){
     
  int index;
  for(index=0; index < number; index++){
  
  int x,y,d;
  
  x = rand() % MAP_WIDTH;
  y = rand() % MAP_HEIGHT;
  
  while(map.layer[BASE_L].cell[y][x].id != 1 | map.layer[OBJECT_L].cell[y][x].id != 0 | map.layer[SNAKE_L].cell[y][x].id != 0){
    x = rand() % MAP_WIDTH;
    y = rand() % MAP_HEIGHT;
  }
  
  d = rand() % 14 + 1;
  
  if(d>=1 && d<=10) map.layer[OBJECT_L].cell[y][x].id = rand() % 4 + 1;
  else if(d==11) map.layer[OBJECT_L].cell[y][x].id = rand() % 2 + 5;
  else map.layer[OBJECT_L].cell[y][x].id = rand() % 4 + 7;
  
  }
  
}

//------------------------------------------------------------------------------
//
// SNAKE SETUP
//
//------------------------------------------------------------------------------

void snakeSetup(){
  
  // Reset snake's 2d array
  int i,j;
  for(j=0; j < MAP_HEIGHT; j++){
    for(i=0; i < MAP_WIDTH; i++){
      map.layer[SNAKE_L].cell[j][i].id = 0;
    }
  }
     
  int index;
  for(index=0; index < body_length; index++){
    
    // Set snake's x,y and direction if haven't been set
    switch(snake[index].x){
                           
      case NULL : {
        if(index==0){
          snake[index].dir = UP;
          snake[index].x = START_X;
          snake[index].y = START_Y;
        } else {
          if(snake[index].dir != NOMOVE) snake[index].dir = snake[index-1].dir;
          
          switch(snake[index].dir){
                                   
            case UP: {
              snake[index].x = snake[index-1].x;
              snake[index].y = snake[index-1].y + 1;
            } break;
            
            case DOWN: {
              snake[index].x = snake[index-1].x;
              snake[index].y = snake[index-1].y - 1;
            } break;
            
            case RIGHT: {
              snake[index].x = snake[index-1].x - 1;
              snake[index].y = snake[index-1].y;
            } break;
            
            case LEFT: {
              snake[index].x = snake[index-1].x + 1;
              snake[index].y = snake[index-1].y;
            } break;
            
            case NOMOVE: {
              snake[index].x = snake[index-1].x;
              snake[index].y = snake[index-1].y;
            } break;
            
          }
        }
      } break;
      
    }
    
    // Fill in snake's 2d array
    map.layer[SNAKE_L].cell[ snake[index].y ][ snake[index].x ].id = 1;
               
  }
     
}

//------------------------------------------------------------------------------
//
// DRAW THE SNAKE
//
//------------------------------------------------------------------------------

void snakeDraw(){
     
  snakeSetup();
     
  // draw the snake's head
  int first_head = HEAD0;
  draw_sprite(DoubleBuffer,
              (BITMAP*)data[ first_head + snake[0].dir ].dat,
              snake[0].x*TILE_SIZE,
              snake[0].y*TILE_SIZE
              );
  
  // draw the snake's body
  int index;
  int first_body = BODY0;
  int first_corner = CORNER1 - 1;
  
  for(index=1; index < body_length - 1; index++){
               
    if(snake[index].x == snake[body_length-1].x && snake[index].y == snake[body_length-1].y){
    } else {
  
      if(snake[index].corner == 0){
                           
        draw_sprite(DoubleBuffer,
                  (BITMAP*)data[ first_body + snake[index].dir ].dat,
                  snake[index].x*TILE_SIZE,
                  snake[index].y*TILE_SIZE
                  );
                  
      } else {
           
        draw_sprite(DoubleBuffer,
                  (BITMAP*)data[ first_corner + snake[index].corner ].dat,
                  snake[index].x*TILE_SIZE,
                  snake[index].y*TILE_SIZE
                  );  
                  
      }
    
    }
               
  }
  
  // draw the snake's tail
  int first_tail = TAIL0;
  
  if(snake[body_length-1].dir == NOMOVE){
                              
    int check = 2;
    
    while(snake[body_length-check].dir == NOMOVE) check++;
                              
    draw_sprite(DoubleBuffer,
              (BITMAP*)data[ first_tail + snake[body_length-check].dir ].dat,
              snake[body_length-1].x*TILE_SIZE,
              snake[body_length-1].y*TILE_SIZE
              );
              
  } else {
  
    draw_sprite(DoubleBuffer,
              (BITMAP*)data[ first_tail + snake[body_length-1].dir ].dat,
              snake[body_length-1].x*TILE_SIZE,
              snake[body_length-1].y*TILE_SIZE
              );
              
  }
  
}

//------------------------------------------------------------------------------
//
// MOVE THE SNAKE
//
//------------------------------------------------------------------------------

void snakeMove(int dir){
  
  // snake's head movement logic
  snake[0].ldir = snake[0].dir;
  snake[0].dir = dir;
  
  switch(snake[0].dir){
                       
    case UP: { 
      if(isHit(snake[0].x, snake[0].y-1)) game_state = STATE_GAMEOVER; else
      snake[0].y --;
    } break;
    
    case DOWN: { 
      if(isHit(snake[0].x, snake[0].y+1)) game_state = STATE_GAMEOVER; else
      snake[0].y ++;
    } break;
    
    case RIGHT: { 
      if(isHit(snake[0].x+1, snake[0].y)) game_state = STATE_GAMEOVER; else
      snake[0].x ++;
    } break;
    
    case LEFT: { 
      if(isHit(snake[0].x-1, snake[0].y)) game_state = STATE_GAMEOVER; else
      snake[0].x --;
    } break;
    
  }
  
  itemEffect(snake[0].x, snake[0].y);
  
  // snake's body and tail movement logic
  int index;
  for(index=1; index < body_length; index++){
               
    if(game_state != STATE_GAMEOVER){
    
    if(snake[index].dir != NOMOVE){
      snake[index].ldir = snake[index].dir;
      snake[index].dir = snake[index-1].ldir;
    }

    switch(snake[index].dir){
                             
      case UP: {
        if(snake[index].y > snake[index-1].y) snake[index].y--;
      } break;
      
      case DOWN: {
        if(snake[index].y < snake[index-1].y) snake[index].y++;
      } break;
      
      case RIGHT: {
        if(snake[index].x < snake[index-1].x) snake[index].x++;
      } break;
      
      case LEFT: {
        if(snake[index].x > snake[index-1].x) snake[index].x--;
      } break;
      
      case NOMOVE: {
        if(snake[index].x != snake[index-1].x | snake[index].y != snake[index-1].y)
        snake[index].dir = snake[index-1].dir;
      } break;
      
    }
    
    }
    
    if(game_state == STATE_GAMEOVER) snake[0].dir = snake[0].ldir;
    
    // check for corners and correct the tail
    if(snake[index].dir != NOMOVE){
                        
    int a;
    for(a=0; a<8; a++){
      if(index < body_length-1 &&
         snake[index].dir != snake[index-1].dir &&
         snake[index].dir == corners[a][0] &&
         snake[index-1].dir == corners[a][1])
      snake[index].corner = corners[a][2];
      else if(index < body_length-1 && snake[index].dir == snake[index-1].dir)
      snake[index].corner = 0;
      else if(index==body_length-1 && snake[index].dir != snake[index-1].dir)
      snake[index].dir = snake[index-1].dir;
      
    }
    
    }
    
  }
     
}

//------------------------------------------------------------------------------
//
// DETECT IF THE SNAKE HIT ITS OWN BODY AND WALLS
//
//------------------------------------------------------------------------------

bool isHit(int x, int y){
     
  if(map.layer[SNAKE_L].cell[y][x].id==1 | map.layer[BASE_L].cell[y][x].id==WALL)
  return true; else return false;
  
}

//------------------------------------------------------------------------------
//
// GROW THE SNAKE
//
//------------------------------------------------------------------------------

void growSnake(int number){
  int index;
  
  for(index=0; index < number; index++){
               
    body_length++;
    snake[body_length-1].dir = NOMOVE;
        
    snake[body_length-1].x = snake[body_length-2].x;
    snake[body_length-1].y = snake[body_length-2].y;
               
  }
}

//------------------------------------------------------------------------------
//
// ITEM'S EFFECT
//
//------------------------------------------------------------------------------

void itemEffect(int x, int y){

  if(map.layer[OBJECT_L].cell[y][x].id != 0){
    switch(map.layer[OBJECT_L].cell[y][x].id){
    
      case APPLE : { score += 5; growSnake(1); } break;
      case ORANGE : { score += 10; growSnake(1); } break;
      case PIZZA : { score += 15; growSnake(2); } break;
      case STRAWBERRY : { score += 20; growSnake(2); } break;
      case RICE : { score += 25; growSnake(3); } break;
      case MUSHROOM : { score += 30; growSnake(3); } break;
      case SHRINK : { if(body_length>7) body_length -= 5; else body_length = 2; } break;
      case GROW : { growSnake(5); } break;
      case SLOW : { if(speed > S_SLOW) speed -= 0.5; setSpeed(speed); } break;
      case FAST : { if(speed < S_VFAST) speed += 0.5; setSpeed(speed); } break;
    
    }
    
    map.layer[OBJECT_L].cell[y][x].id = 0;
    play_sample((SAMPLE*)data[SOUND2].dat, 255, 0, 1000, 0);
  }
  
}

//------------------------------------------------------------------------------
//
// CLEAR ITEM
//
//------------------------------------------------------------------------------

void clearItem(){

  int i,j;
  
  for(j=0; j < MAP_HEIGHT; j++)
  for(i=0; i < MAP_WIDTH; i++)
    map.layer[OBJECT_L].cell[j][i].id = 0;
  
}

//------------------------------------------------------------------------------
//
// RESTART GAME
//
//------------------------------------------------------------------------------

void restartGame(){
     
  int index;
  for(index=0; index < body_length; index++){
    snake[index].x = 0;
    snake[index].y = 0;
    snake[index].dir = 0;
    snake[index].ldir = 0;
    snake[index].corner = 0;
  }

  clearItem();
  body_length = 2;
  
  speed = S_SLOW;
  setSpeed(speed);
  
  game_state = STATE_LOAD;
  
}

//------------------------------------------------------------------------------
//
// RESET GAME
//
//------------------------------------------------------------------------------

void resetGame(){
     
  int index;
  for(index=0; index < body_length; index++){
    snake[index].x = 0;
    snake[index].y = 0;
    snake[index].dir = 0;
    snake[index].ldir = 0;
    snake[index].corner = 0;
  }

  clearItem();
  body_length = 2;
  
  speed = S_SLOW;
  setSpeed(speed);
  
  game_state = STATE_TITLE;
  score = 0;
  current_level = 1;
  
}

//------------------------------------------------------------------------------
//
// SET SPEED
//
//------------------------------------------------------------------------------

void setSpeed(float number){
  int the_speed = number*10;
  install_int_ex(ticker,BPS_TO_TIMER(the_speed));
}
