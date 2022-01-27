#ifndef STATE_LEVEL_H_INCLUDED
#define STATE_LEVEL_H_INCLUDED

#include <list>
#include "particle.h"
#include "state.h"

#define END_GAME_DELAY     1000

class StateLevel : public GameState
{
public:
   StateLevel();
   ~StateLevel();

   void HandleEvents();
   void Render();
   void Update();

private:
   std::list<Particle*> particles_;
   ParticleEmitter* emit_[2];
   int ticks_;
   int scale_;
   int next_;
};

#endif // STATE_LEVEL1_H_INCLUDED
