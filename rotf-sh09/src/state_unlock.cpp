#include "state.h"
#include "state_unlock.h"
#include "model.h"
#include "sys.h"
#include <alfont.h>

StateUnlock::StateUnlock()
{
   if (!globals::model->isUnlocked_[globals::model->currentLevel_->id_-1] && globals::model->score_ >= globals::model->currentLevel_->targetScore_)
   {
      unlocked_ = true;
      if (globals::model->currentLevel_->id_ < 4)
      {
         globals::model->isUnlocked_[globals::model->currentLevel_->id_-1] = true;
         globals::model->UpdateUnlocked();
      }
   }
   else
   {
      unlocked_ = false;
   }

   scale_ = 0;
   next_ = 0;
}

StateUnlock::~StateUnlock()
{
   globals::model->Reset();
}

void StateUnlock::HandleEvents()
{
   if (scale_ < 100)
      scale_++;

   if (KeyHit(KEY_ESC))
      SetNextState(STATE_EXIT);

   if (KeyHit(KEY_SPACE))
      next_ = STATE_MENU;
}

void StateUnlock::Update()
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

void StateUnlock::Render()
{
   if (!unlocked_ && !globals::model->isUnlocked_[globals::model->currentLevel_->id_-1])
   {
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "You didn't unlock anything. Better luck next time!", SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1);
   }
   else if (!unlocked_ && globals::model->isUnlocked_[globals::model->currentLevel_->id_-1])
   {
      alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1, "Looks like you have already unlocked level %d.", globals::model->currentLevel_->id_+1);
   }
   else if (unlocked_ && globals::model->currentLevel_->id_ < 4)
   {
      alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1, "Congratulations! You have unlocked level %d.", globals::model->currentLevel_->id_+1);
   }
   else if (unlocked_ && globals::model->currentLevel_->id_ == 4)
   {
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "You have now mastered the art of counting flies.", SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1);
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "Now go impress your maths teacher.", SCREEN_W/2, SCREEN_H/2 + 20, makecol(0,0,0), -1);
   }
   else
   {
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "You need more practice to beat this level.", SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1);
      alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "Go to your nearest dumpster and start practicing.", SCREEN_W/2, SCREEN_H/2 + 20, makecol(0,0,0), -1);
   }

   int width = SCREEN_WIDTH*(scale_/100.0);
   int height = SCREEN_HEIGHT*(scale_/100.0);
   stretch_blit(globals::stretchBuffer, globals::buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, width, height);
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));
}
