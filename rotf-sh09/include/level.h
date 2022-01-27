#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <list>

#define LEVEL_EMIT_LEFT    0
#define LEVEL_EMIT_RIGHT   1
#define LEVEL_EMIT_BOTH    2

class Level
{
public:
   Level(const int&, const int&, const int&, const int&, const int&);

   int minFlies_, maxFlies_;
   int dir_;
   int targetScore_;
   int id_;
};

namespace globals
{
   extern Level* levels[4];
};

void InitializeLevels();

#endif // LEVEL_H_INCLUDED
