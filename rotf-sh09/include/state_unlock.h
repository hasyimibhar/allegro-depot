#ifndef STATE_UNLOCK_H_INCLUDED
#define STATE_UNLOCK_H_INCLUDED

#include "state.h"

class StateUnlock : public GameState
{
public:
   StateUnlock();
   ~StateUnlock();

   void HandleEvents();
   void Update();
   void Render();

private:
   bool unlocked_;
   int scale_;
   int next_;
};

#endif // STATE_UNLOCK_H_INCLUDED
