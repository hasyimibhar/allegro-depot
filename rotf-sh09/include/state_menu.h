#ifndef STATE_MENU_H_INCLUDED
#define STATE_MENU_H_INCLUDED

#include "state.h"

class StateMenu : public GameState
{
public:
   StateMenu();

   void HandleEvents();
   void Render();
   void Update();

private:
   int selection_;
   int scale_;
   int next_;
};

#endif // STATE_MENU_H_INCLUDED
