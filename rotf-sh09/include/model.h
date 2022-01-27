#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "level.h"

#define MAX_WAVES    3

#define WAVE_1_INTERVAL    100
#define WAVE_2_INTERVAL    80
#define WAVE_3_INTERVAL    60

namespace globals
{
   extern int wavesInterval[];
}

class Model
{
public:
   Model();

   void GenerateRandomFliesCount();
   void SetLevel(Level*);
   void Reset();
   void LoadUnlocked();
   void UpdateUnlocked();

   int actualFliesCount_;
   int userFliesCount_;
   int score_;
   int fliesEmitInterval_;
   Level* currentLevel_;
   int currentWave_;

   bool isUnlocked_[3];
};

#endif // MODEL_H_INCLUDED
