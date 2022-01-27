#include "level.h"
#include <list>
#include "Vector2D.h"

Level* globals::levels[4];

Level::Level(const int& minFlies, const int& maxFlies, const int& dir, const int& targetScore, const int& id)
{
   minFlies_ = minFlies;
   maxFlies_ = maxFlies;
   dir_ = dir;
   targetScore_ = targetScore;
   id_ = id;
}

void InitializeLevels()
{
   globals::levels[0] = new Level(10, 40, LEVEL_EMIT_LEFT, 3000, 1);
   globals::levels[1] = new Level(50, 100, LEVEL_EMIT_RIGHT, 4000, 2);
   globals::levels[2] = new Level(30, 60, LEVEL_EMIT_BOTH, 4000, 3);
   globals::levels[3] = new Level(60, 100, LEVEL_EMIT_BOTH, 6000, 4);
}
