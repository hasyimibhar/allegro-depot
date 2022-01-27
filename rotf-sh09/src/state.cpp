/*
 * HEADER FILES
 */

#include "state.h"
#include "state_title.h"
#include "state_menu.h"
#include "state_level.h"
#include "state_postmortem.h"
#include "state_unlock.h"
#include <cstdlib>

GameState* globals::currentState = NULL;
int globals::stateID = STATE_NULL;
int globals::nextState = STATE_NULL;

/*
 * FUNCTION IMPLEMENTATIONS
 */

void SetNextState(const int& newState)
{
   if (globals::nextState != STATE_EXIT)
   {
      globals::nextState = newState;
   }
}

void ChangeState()
{
   if (globals::nextState != STATE_NULL)
   {
      if (globals::nextState != STATE_EXIT)
      {
         delete globals::currentState;
      }

      switch (globals::nextState)
      {
         case STATE_TITLE: globals::currentState = new StateTitle(); break;
         case STATE_MENU: globals::currentState = new StateMenu(); break;
         case STATE_LEVEL: globals::currentState = new StateLevel(); break;
         case STATE_POST_MORTEM: globals::currentState = new StatePostMortem(); break;
         case STATE_UNLOCK : globals::currentState = new StateUnlock(); break;
         default: break;
      }

      globals::stateID = globals::nextState;
      globals::nextState = STATE_NULL;
   }
}
