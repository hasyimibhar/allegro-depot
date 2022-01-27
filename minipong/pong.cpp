class Pong {
  
  private:
    int y;
    int width, height;
    double dy;
    
  public:
    Pong ();
    void drawPong ();
};

Pong::Pong (){
  y = SCREENH/2;
  dy = 0;
  width = 10;
  height = 100;
}

void Pong::drawPong (){
  rect(DoubleBuffer, 30, y-height/2, 30+width, y+height/2, BLACK);
}
