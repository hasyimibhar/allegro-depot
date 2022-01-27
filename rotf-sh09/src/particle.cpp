#include "particle.h"
#include "Vector2D.h"
#include "sys.h"
#include <cmath>
#include "data.h"

Particle::Particle(const Vector2D& pos, const Vector2D& vel, const int& graphicID, MovingBehavior* mov)
{
   pos_ = pos;
   vel_ = vel;
   graphicID_ = graphicID;
   mov_ = mov;
}

Particle::~Particle()
{
   delete mov_;
}

void Particle::Update()
{
   mov_->Update(this);
}

void Particle::Render()
{
   //draw_sprite(globals::buffer, (BITMAP*)globals::datafile[graphicID_].dat, (int)pos_.x, (int)pos_.y);
   circlefill(globals::stretchBuffer, (int)pos_.x, (int)pos_.y, 2, makecol(0,0,0));
}

ParticleEmitter::ParticleEmitter(const int& particleNum, const Vector2D& start, const Vector2D& end, const int& emitInterval, const int& dir)
{
   particleNum_ = particleNum;
   start_ = start;
   end_ = end;
   emitInterval_ = emitInterval;
   ticks_ = 0;
   dir_ = dir;
}

void ParticleEmitter::Update(std::list<Particle*>& particles)
{
   if (ticks_ < emitInterval_ - rand()%20)
   {
      ticks_++;
   }
   else if (particleNum_ != 0)
   {
      ticks_ = 0;

      Vector2D randPos;
      randPos.x = start_.x + rand()%((int)end_.x - (int)start_.x);
      randPos.y = start_.y + rand()%((int)end_.y - (int)start_.y);

      particles.push_back(new Particle(randPos, Vector2D(), FLY, new FlyBehavior(FLY_RADIUS, FLY_PROJ_DIST, FLY_UPDATE_INTERVAL, dir_)));
      particleNum_--;
   }
}

int ParticleEmitter::GetParticleNum()
{
   return particleNum_;
}

FlyBehavior::FlyBehavior(const int& randRadius, const int& projectionDist, const int& updateInterval, const int& dir)
{
   randRadius_ = randRadius;
   projectionDist_ = projectionDist;
   updateInterval_ = updateInterval;
   ticks_ = 0;
   dir_ = dir;
}

void FlyBehavior::Update(Particle* p)
{
   if (ticks_ < updateInterval_)
   {
      ticks_++;
   }
   else
   {
      ticks_ = 0;

      int rAngle = rand()%360;
      double rad = rAngle*MATH_PI/180;
      Vector2D temp = p->pos_;

      if (dir_ == FLY_LEFT)
         temp.x -= projectionDist_;

      else
         temp.x += projectionDist_;

      temp.x += randRadius_*cosf(rad);
      temp.y += randRadius_*sinf(rad);
      //target_ = temp;

      p->vel_ = temp - p->pos_;
      p->vel_ = p->vel_/(80.0 - (double)(rand()%20));
   }

   p->pos_ = p->pos_ + p->vel_;
}
