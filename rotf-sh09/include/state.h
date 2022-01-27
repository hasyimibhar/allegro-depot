#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

/*
 * HEADER FILES
 */

#include "sys.h"

/*
 * CONSTANTS
 */

enum State
{
   STATE_NULL,
   STATE_TITLE,
   STATE_MENU,
   STATE_LEVEL,
   STATE_POST_MORTEM,
   STATE_UNLOCK,
   STATE_EXIT
};

/*
 * VARIABLES
 */

/*
 * CLASS INTERFACE
 */

class GameState
{
public:
   virtual void HandleEvents() = 0;
   virtual void Update() = 0;
   virtual void Render() = 0;
   virtual ~GameState() {}
};

namespace globals
{
   extern GameState* currentState;
   extern int stateID;
   extern int nextState;
};

/*
 * FUNCTION PROTOTYPES
 */

void SetNextState(const int&);
void ChangeState();

#endif // STATE_H_INCLUDED
