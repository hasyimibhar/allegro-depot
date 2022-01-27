#include "model.h"
#include <cstdlib>
#include "level.h"
#include <cassert>
#include <allegro.h>
#include "sys.h"

int globals::wavesInterval[MAX_WAVES] = { WAVE_1_INTERVAL, WAVE_2_INTERVAL, WAVE_3_INTERVAL };

Model::Model()
{
   actualFliesCount_ = 0;
   userFliesCount_ = 0;
   score_ = 0;
   currentLevel_ = NULL;
   currentWave_ = 1;
   fliesEmitInterval_ = globals::wavesInterval[currentWave_-1];

   isUnlocked_[0] = false;
   isUnlocked_[1] = false;
   isUnlocked_[2] = false;
}

void Model::GenerateRandomFliesCount()
{
   assert(currentLevel_ != NULL);
   actualFliesCount_ = rand()%(currentLevel_->maxFlies_ - currentLevel_->minFlies_) + currentLevel_->minFlies_;
}

void Model::SetLevel(Level* level)
{
   currentLevel_ = level;
}

void Model::Reset()
{
   actualFliesCount_ = 0;
   userFliesCount_ = 0;
   score_ = 0;
   fliesEmitInterval_ = 100;
   currentLevel_ = NULL;
   currentWave_ = 1;
}

void Model::LoadUnlocked()
{
   PACKFILE* file;

   if (file_exists("media/unlocks.fly", 0, NULL))
   {
      file = pack_fopen("media/unlocks.fly", "rb");

      int i = 0;

      while (!pack_feof(file))
      {
         if (i == 3)
         {
            sysError("File is corrupted");
         }

         isUnlocked_[i] = pack_getc(file);
         i++;
      }
   }
   else
   {
      file = pack_fopen("media/unlocks.fly", "wb");
      for (int i = 0; i < 3; i++)
      {
         pack_putc(0, file);
      }
   }

   pack_fclose(file);
}

void Model::UpdateUnlocked()
{
   PACKFILE* file;

   file = pack_fopen("media/unlocks.fly", "wb");
   for (int i = 0; i < 3; i++)
   {
      pack_putc(isUnlocked_[i], file);
   }

   pack_fclose(file);
}
