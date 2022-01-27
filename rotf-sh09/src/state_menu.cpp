#include "state.h"
#include "state_menu.h"
#include "sys.h"
#include "model.h"
#include "level.h"
#include <alfont.h>

StateMenu::StateMenu()
{
   selection_ = 0;
   scale_ = 0;
   next_ = 0;
}

void StateMenu::HandleEvents()
{
   if (scale_ < 100)
      scale_++;

   if (KeyHit(KEY_ESC))
      SetNextState(STATE_EXIT);

   if (KeyHit(KEY_UP))
   {
      if (selection_ > 0)
         selection_--;

      else
         selection_ = 4;
   }
   else if (KeyHit(KEY_DOWN))
   {
      if (selection_ < 4)
         selection_++;

      else
         selection_ = 0;
   }

   if (KeyHit(KEY_SPACE))
   {
      if (selection_ == 4)
         next_ = STATE_EXIT;
      else if (selection_ == 0)
      {
         next_ = STATE_LEVEL;
         globals::model->SetLevel(globals::levels[0]);
      }
      else
      {
         if (globals::model->isUnlocked_[selection_-1])
         {
            next_ = STATE_LEVEL;
            globals::model->SetLevel(globals::levels[selection_]);
         }
      }
   }
}

void StateMenu::Update()
{
   if (next_ && scale_ < 200)
   {
      scale_++;
   }
   else
   {
      SetNextState(next_);
   }
}

void StateMenu::Render()
{
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 100, makecol(0,0,0), -1, "Level %d.", 1);
   for (int i = 0; i < 3; i++)
   {
      if (globals::model->isUnlocked_[i])
         alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 120 + i*20, makecol(0,0,0), -1, "Level %d.", i+2);

      else
         alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 120 + i*20, makecol(150, 150, 150), -1, "Level %d.", i+2);
   }

   alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "*        *", SCREEN_W/2, 100 + selection_*20, makecol(0,0,0), -1);
   alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "EXIT", SCREEN_W/2, 180, makecol(0,0,0), -1);

   if (selection_ > 0 && selection_ < 4 && !globals::model->isUnlocked_[selection_-1])
   {
      alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 300, makecol(0,0,0), -1, "You need to get %d points on level %d to unlock this level.", globals::levels[selection_-1]->targetScore_, selection_);
   }

   int width = SCREEN_WIDTH*(scale_/100.0);
   int height = SCREEN_HEIGHT*(scale_/100.0);
   stretch_blit(globals::stretchBuffer, globals::buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, width, height);
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));
}
