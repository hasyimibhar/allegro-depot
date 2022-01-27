class Pong {

  private:
    int width;
    unsigned char id;
    
  public:
    double x,y;
    double dy;
    Pong ();
    Pong (int);
    void handle_input ();
    void move ();
    void draw ();
    void setCoor (int,int);
    double getY ();
    double getX ();
    int getId ();
    
};

//------------------------------------------------------------------------------

Pong::Pong (){

  x = 0;
  y = 0;
  dy = 0;
  width = 60;
  id = 0;
  
}

//------------------------------------------------------------------------------

Pong::Pong (int a){
  
  id = a;
  width = 60;

}

//------------------------------------------------------------------------------

void Pong::handle_input (){
  
  if(id==0){
    if(key[KEY_UP]) dy -= ACCELERATION;
    if(key[KEY_DOWN]) dy += ACCELERATION;
    //if(keyPressed(KEY_SPACE)) dy *= 2;
  } else
  
  if(id==1){
    if(key[KEY_W]) dy -= ACCELERATION;
    if(key[KEY_S]) dy += ACCELERATION;
  }
  
}

//------------------------------------------------------------------------------

void Pong::move (){
     
  y += dy;
  
  if( y < 0 ){
    y = 0;
    dy *= -1;
  }
  
  if( y > SCREENH - width ){
    y = SCREENH - width;
    dy *= -1;
  }
  
  dy *= FRICTION;
  
}

//------------------------------------------------------------------------------

void Pong::draw (){
  
  draw_sprite(DoubleBuffer, (BITMAP*)data[P1_1+id].dat, x, y);
  
  set_trans_blender(0, 0, 0, 30);
  drawing_mode(DRAW_MODE_TRANS, 0, 0, 0);
  draw_trans_sprite(DoubleBuffer, (BITMAP*)data[P1_2+id].dat, x-1, y-1);
  solid_mode();

}

//------------------------------------------------------------------------------

void Pong::setCoor (int a, int b){

  x = a;
  y = b;
  
}

//------------------------------------------------------------------------------

double Pong::getY (){

  return y;
  
}

//------------------------------------------------------------------------------

double Pong::getX (){

  return x;
  
}

//------------------------------------------------------------------------------

int Pong::getId (){
  
  return id;
  
}

//------------------------------------------------------------------------------

Pong P1;
Pong CPU (1);
