#include "state.h"
#include "state_title.h"
#include "data.h"
#include "sys.h"
#include <allegro.h>
#include <alfont.h>
#include "particle.h"
#include "Vector2D.h"

StateTitle::StateTitle()
{
   ticks_ = 0;
   scale_ = 0;
   next_ = 0;
}

void StateTitle::HandleEvents()
{
   if (scale_ < 100)
      scale_++;

   if (KeyHit(KEY_ESC))
      SetNextState(STATE_EXIT);

   if (KeyHit(KEY_SPACE))
      next_ = STATE_MENU;
}

void StateTitle::Update()
{
   if (ticks_ < MAX_TICKS)
      ticks_++;

   else
      ticks_ = 0;

   if (next_ && scale_ < 200)
   {
      scale_++;
   }
   else
   {
      SetNextState(next_);
   }
}

void StateTitle::Render()
{
   blit((BITMAP*)globals::datafile[TITLE].dat, globals::stretchBuffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   if (ticks_ < MAX_TICKS/2)
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "Press SPACEBAR to continue", SCREEN_W/2-100, 400, makecol(0,0,0), -1);

   int width = SCREEN_WIDTH*(scale_/100.0);
   int height = SCREEN_HEIGHT*(scale_/100.0);
   stretch_blit(globals::stretchBuffer, globals::buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, width, height);
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));
}
