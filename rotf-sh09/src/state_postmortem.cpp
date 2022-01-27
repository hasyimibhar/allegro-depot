#include "state.h"
#include "state_postmortem.h"
#include <cmath>
#include "sys.h"
#include <alfont.h>
#include "model.h"

StatePostMortem::StatePostMortem()
{
   globals::model->currentWave_++;

   absoluteError_ = fabs(globals::model->userFliesCount_-globals::model->actualFliesCount_);
   relativeError_ = ((double)absoluteError_/globals::model->actualFliesCount_)*100.0;

   if (!absoluteError_)
      bonus_ = 1000;
   else
      bonus_ = 0;

   totalScore_ = (int)((100.0 - relativeError_)*10.0) + bonus_;

   globals::model->score_ += totalScore_;

   scale_ = 0;
   next_ = 0;
}

StatePostMortem::~StatePostMortem()
{
}

void StatePostMortem::HandleEvents()
{
   if (scale_ < 100)
      scale_++;

   if (KeyHit(KEY_ESC))
      SetNextState(STATE_EXIT);

   if (KeyHit(KEY_SPACE))
   {
      if (globals::model->currentWave_ <= MAX_WAVES)
      {
         globals::model->userFliesCount_ = 0;
         next_ = STATE_LEVEL;
      }
      else
      {
         next_ = STATE_UNLOCK;
      }
   }
}

void StatePostMortem::Update()
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

void StatePostMortem::Render()
{
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 100, makecol(0,0,0), -1, "Your count    %d", globals::model->userFliesCount_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 120, makecol(0,0,0), -1, "Actual count    %d", globals::model->actualFliesCount_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 140, makecol(0,0,0), -1, "Absolute error   %d", absoluteError_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 160, makecol(0,0,0), -1, "Relative error     %.2f", relativeError_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 180, makecol(0,0,0), -1, "Accuracy        %.2f", 100.0-relativeError_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 200, makecol(0,0,0), -1, "Bonus        %d", bonus_);
   alfont_textout_centre_aa_ex(globals::stretchBuffer, globals::userFont, "*********************", SCREEN_W/2, 220, makecol(0,0,0), -1);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 240, makecol(0,0,0), -1, "Level score        %04d", totalScore_);
   alfont_textprintf_centre_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 260, makecol(0,0,0), -1, "Total score        %04d", globals::model->score_);

   int width = SCREEN_WIDTH*(scale_/100.0);
   int height = SCREEN_HEIGHT*(scale_/100.0);
   stretch_blit(globals::stretchBuffer, globals::buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, width, height);
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));
}
