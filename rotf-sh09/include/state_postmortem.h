#ifndef STATE_POSTMORTEM_H_INCLUDED
#define STATE_POSTMORTEM_H_INCLUDED

#include "state.h"
#include <cmath>

class StatePostMortem : public GameState
{
public:
   StatePostMortem();
   ~StatePostMortem();

   void HandleEvents();
   void Render();
   void Update();

private:
   int absoluteError_;
   double relativeError_;
   int bonus_;
   int totalScore_;
   int scale_;
   int next_;
};

#endif // STATE_POSTMORTEM_H_INCLUDED
