#ifndef STATE_TITLE_H_INCLUDED
#define STATE_TITLE_H_INCLUDED

#include "state.h"
#include "particle.h"
#include <list>

#define MAX_TICKS    100

class StateTitle : public GameState
{
public:
   StateTitle();

   void HandleEvents();
   void Update();
   void Render();

private:
   int ticks_;
   int scale_;
   int next_;
};

#endif // STATE_TITLE_H_INCLUDED
