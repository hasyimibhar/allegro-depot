#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "Vector2D.h"
#include <list>

#define MATH_PI      3.14159265

#define FLY_RADIUS    50
#define FLY_PROJ_DIST 100
#define FLY_UPDATE_INTERVAL   100

class MovingBehavior;

class Particle
{
public:
   Particle(const Vector2D&, const Vector2D&, const int&, MovingBehavior*);
   ~Particle();

   void Update();
   void Render();

   Vector2D pos_, vel_;
   int graphicID_;
   MovingBehavior* mov_;
};

class ParticleEmitter
{
public:
   ParticleEmitter(const int&, const Vector2D&, const Vector2D&, const int&, const int&);

   void Update(std::list<Particle*>&);

   int GetParticleNum();

private:
   Vector2D start_, end_;
   int ticks_;
   int particleNum_;
   int emitInterval_;
   int dir_;
};

int RandomInteger(int, int);

class MovingBehavior
{
public:
   virtual void Update(Particle*)=0;
};

#define FLY_LEFT     0
#define FLY_RIGHT    1

class FlyBehavior : public MovingBehavior
{
public:
   FlyBehavior(const int&, const int&, const int&, const int&);

   void Update(Particle*);

private:
   //Vector2D target_;
   int randRadius_;
   int projectionDist_;
   int updateInterval_;
   int ticks_;
   int dir_;
};

#endif // PARTICLE_H_INCLUDED
