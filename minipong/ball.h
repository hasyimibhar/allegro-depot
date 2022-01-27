class Ball {
      
  private:
    double dx, dy;
    int size;
    bool inv;
    double x,y;
    double lx[num_trail];
    double ly[num_trail];
    unsigned char dir;
    
  public:
    Ball ();
    void move ();
    void draw ();
    void setCoor (int,int);
    void setVel (int,int);
    void collide (Pong* p);
    void AI (Pong* p, Pong* p2);
    
};

//------------------------------------------------------------------------------

Ball::Ball (){

  x = 0;
  y = 0;
  
  int index;
  for(index=0; index<num_trail; index++){ 
    lx[index] = 0;
    ly[index] = 0;
  }
  
  dx = 0;
  dy = 0;
  size = 10;
  dir = 0;
  inv = false;
  
}

//------------------------------------------------------------------------------

void Ball::move (){
  
  int index;
  for(index=1; index<num_trail; index++){ 
    lx[num_trail-index] = lx[num_trail-1-index];
    ly[num_trail-index] = ly[num_trail-1-index];
  }
  
  lx[0] = x;
  ly[0] = y; 
  
  x += dx;
  y += dy;
  
  if(x > 20 && x < (SCREENW-20-size))
    inv = false;
  else 
    inv = true;
  
  if(dx>0)
    dir = 0;
  else if(dx<0)
    dir = 1;

  if(y < 0){
  
    y = 0;
    dy *= -1;
    
  }
  
  if(y > SCREENH - size){
       
    y = SCREENH - size;
    dy *= -1;
    
  }
  
  if(x < 0){
  
    dy = 0;
    dx = -3;
    setCoor (SCREENW/2, SCREENH/2);
    score[1] += 1;
    game_state = S_BETWEEN;
    
  }
  
  if(x > SCREENW - size){
       
    dy = 0;
    dx = 3;
    setCoor (SCREENW/2, SCREENH/2);
    score[0] += 1;
    game_state = S_BETWEEN;
    
  }
  
  dy *= B_FRICTION;
  
}

//------------------------------------------------------------------------------

void Ball::draw (){
           
  if(!inv){
       
    draw_sprite(DoubleBuffer, (BITMAP*)data[BALL1+dir].dat, x, y);
    
    int index;
    for(index=0; index<num_trail; index++){
      set_trans_blender(0, 0, 0, index*(140/num_trail));
      drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
      draw_trans_sprite(DoubleBuffer, (BITMAP*)data[BALL1+dir].dat, lx[num_trail-index], ly[num_trail-index]);
      solid_mode(); 
    }
    
  } else {
   
    set_trans_blender(0, 0, 0, 40);
    drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
    draw_trans_sprite(DoubleBuffer, (BITMAP*)data[BALL1+dir].dat, x, y);
    solid_mode(); 
    
  }
  
}
//------------------------------------------------------------------------------

void Ball::setCoor (int a, int b){

  x = a;
  y = b;
  
  int index;
  for(index=0; index<num_trail; index++){ 
    lx[index] = x;
    ly[index] = y;
  }
  
}

//------------------------------------------------------------------------------

void Ball::setVel (int a, int b){

  dx = a;
  dy = b;
  
}

//------------------------------------------------------------------------------

void Ball::collide (Pong* p){

  if(!inv){
    if(p->getId() == 0){
           
      if(x > p->getX() && x < (p->getX() + 17) && (y+size) > p->getY() && y < (p->getY() + 60)){
        dy += p->dy;
        dx *= -1;
        dx += DX_INC;
        if(thesound) play_sample((SAMPLE*)data[WAV1].dat, 255, 0, 1000, 0);
      }
      
    } else {
         
      if((x+size) < (p->getX()+15) && (x+size) > (p->getX() - 1) && (y+size) > p->getY() && y < (p->getY() + 60)){
        dy += p->dy;
        dx += DX_INC;
        dx *= -1;
        if(thesound) play_sample((SAMPLE*)data[WAV1].dat, 255, 0, 1000, 0);
      }
      
    }
  }
     
}

//------------------------------------------------------------------------------

void Ball::AI (Pong* p, Pong* p2){
     
  switch(p->getId()){
                     
    case 0 : {
         
      if(dir == 1){
        
        if((y+5) < (p->y+20) || (y+5) > (p->y+40)){
          if((p->y+30) < (y+5)) p->dy += CPU_ACCELERATION; else
          if((p->y+30) > (y+5)) p->dy -= CPU_ACCELERATION;
        }
        
        if(x > p->getX() && x < (p->getX() + 20) && (y+size) > p->getY() && y < (p->getY() + 60)){
                    
          int randc = rand()%2 + 1;
          
          if(p2->y > SCREENH/2) p->dy -= 3/randc;
          else p->dy += 3/randc;
          
        }
        
      } else if(dir == 0){
             
        //if((p->y+30) < (SCREENH/2 - 20) || (p->y+30) > (SCREENH/2 + 20)){
        //  if((p->y+30) < SCREENH/2) p->dy += CPU_ACCELERATION; else
        //  if((p->y+30) > SCREENH/2) p->dy -= CPU_ACCELERATION;
        //}
        
      }
         
    } break;
    
    case 1 : {
         
      if(dir == 0){
        
        if((y+5) < (p->y+20) || (y+5) > (p->y+40)){
          if((p->y+30) < (y+5)) p->dy += CPU_ACCELERATION; else
          if((p->y+30) > (y+5)) p->dy -= CPU_ACCELERATION;
        }
        
        if((x+size) < (p->getX()+15) && (x+size) > (p->getX() - 5) && (y+size) > p->getY() && y < (p->getY() + 60)){
                    
          int randc = rand()%2 + 1;
          
          if(p2->y > SCREENH/2) p->dy -= 3/randc;
          else p->dy += 3/randc;
          
        }
        
      } else if(dir == 1){
             
        //if((p->y+30) < (SCREENH/2 - 20) || (p->y+30) > (SCREENH/2 + 20)){
        //  if((p->y+30) < SCREENH/2) p->dy += CPU_ACCELERATION; else
        //  if((p->y+30) > SCREENH/2) p->dy -= CPU_ACCELERATION;
        //}
        
      }
         
    } break;
    
  }
     
}

//------------------------------------------------------------------------------


Ball ball;
