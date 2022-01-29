//------------------------------------------------------------------------------
//
// TIMER
//
//------------------------------------------------------------------------------

volatile int ticks=0;

void ticker(){
        ticks++;
}END_OF_FUNCTION(ticker)

//------------------------------------------------------------------------------
//
// KEY PRESSED
//
//------------------------------------------------------------------------------

static bool MyKeyArray[KEY_MAX];

bool keyPressed(int k)
{
	if(key[k] && !MyKeyArray[k]){ 
		MyKeyArray[k] = true;  
		return true;
	}
	else if(!key[k] && MyKeyArray[k]) { 
		MyKeyArray[k] = false; 
	}
	return false;
}
