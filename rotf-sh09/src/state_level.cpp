#include <list>
#include "sys.h"
#include "state_level.h"
#include "particle.h"
#include "data.h"
#include "level.h"

StateLevel::StateLevel()
{
   globals::model->GenerateRandomFliesCount();
   globals::model->fliesEmitInterval_ = globals::wavesInterval[globals::model->currentWave_-1];

   if (globals::model->currentLevel_->dir_ == LEVEL_EMIT_LEFT)
   {
      emit_[0] = new ParticleEmitter(globals::model->actualFliesCount_, Vector2D(-100.0, 100.0), Vector2D(-10.0, 300.0), globals::model->fliesEmitInterval_, FLY_RIGHT);
      emit_[1] = new ParticleEmitter(0, Vector2D(650.0, 100.0), Vector2D(740.0, 300.0), globals::model->fliesEmitInterval_, FLY_LEFT);
   }
   else if (globals::model->currentLevel_->dir_ == LEVEL_EMIT_RIGHT)
   {
      emit_[0] = new ParticleEmitter(0, Vector2D(-100.0, 100.0), Vector2D(-10.0, 300.0), globals::model->fliesEmitInterval_, FLY_RIGHT);
      emit_[1] = new ParticleEmitter(globals::model->actualFliesCount_, Vector2D(650.0, 100.0), Vector2D(740.0, 300.0), globals::model->fliesEmitInterval_, FLY_LEFT);
   }
   else if (globals::model->currentLevel_->dir_ == LEVEL_EMIT_BOTH)
   {
      emit_[0] = new ParticleEmitter(globals::model->actualFliesCount_/2, Vector2D(-100.0, 100.0), Vector2D(-10.0, 300.0), globals::model->fliesEmitInterval_, FLY_RIGHT);
      emit_[1] = new ParticleEmitter(globals::model->actualFliesCount_/2, Vector2D(650.0, 100.0), Vector2D(740.0, 300.0), globals::model->fliesEmitInterval_, FLY_LEFT);
   }

   ticks_ = 0;
   scale_ = 0;
   next_ = 0;
}

StateLevel::~StateLevel()
{
}

void StateLevel::HandleEvents()
{
   if (KeyHit(KEY_ESC))
      SetNextState(STATE_EXIT);

   if (KeyHit(KEY_SPACE))
   {
      globals::model->userFliesCount_++;
      ticks_ = 0;
   }
}

void StateLevel::Update()
{
   if (scale_ < 100)
      scale_++;

   emit_[0]->Update(particles_);
   emit_[1]->Update(particles_);

   for (std::list<Particle*>::iterator i = particles_.begin(); i != particles_.end(); i++)
   {
      (*i)->Update();
   }

   if (ticks_ < END_GAME_DELAY)
   {
      ticks_++;
   }
   else if (!emit_[0]->GetParticleNum() && !emit_[1]->GetParticleNum())
   {
      next_ = STATE_POST_MORTEM;
   }

   if (next_ && scale_ < 200)
   {
      scale_++;
   }
   else
   {
      SetNextState(next_);
   }
}

void StateLevel::Render()
{
   blit((BITMAP*)globals::datafile[BG_1].dat, globals::stretchBuffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   for (std::list<Particle*>::iterator i = particles_.begin(); i != particles_.end(); i++)
   {
      (*i)->Render();
   }

   alfont_textprintf_aa_ex(globals::stretchBuffer, globals::userFont, 560, 20, makecol(100,100,100), -1, "%04d", globals::model->score_);
   alfont_textprintf_aa_ex(globals::stretchBuffer, globals::userFont, SCREEN_W/2, 380, makecol(0,0,0), -1, "%d flies", globals::model->userFliesCount_);
   //alfont_textprintf_aa_ex(globals::buffer, globals::userFont, SCREEN_W/2-100, 420, makecol(0,0,0), -1, "%d flies", globals::model->actualFliesCount_);
   int width = SCREEN_WIDTH*(scale_/100.0);
   int height = SCREEN_HEIGHT*(scale_/100.0);
   stretch_blit(globals::stretchBuffer, globals::buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH/2 - width/2, SCREEN_HEIGHT/2 - height/2, width, height);
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));

}
