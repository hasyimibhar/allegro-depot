volatile int ticks=0;

void ticker(){
        ticks++;
}END_OF_FUNCTION(ticker)

//------------------------------------------------------------------------------

volatile int keyticks=0;

void keyticker(){
        keyticks++;
}END_OF_FUNCTION(keyticker)

//------------------------------------------------------------------------------

volatile int close_button_pressed = FALSE;

void close_button_handler(void){
  close_button_pressed = TRUE;
}END_OF_FUNCTION(close_button_handler)
