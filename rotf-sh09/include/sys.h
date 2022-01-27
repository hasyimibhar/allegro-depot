#ifndef SYS_H_INCLUDED
#define SYS_H_INCLUDED

#include <allegro.h>
#include <string>
#include <alfont.h>
#include "model.h"

#define SCREEN_WIDTH       640
#define SCREEN_HEIGHT      480
#define SCREEN_DEPTH       32

#define DAT_PNG  DAT_ID('P','N','G',' ')

namespace globals
{
   extern BITMAP*   buffer;
   extern BITMAP*   stretchBuffer;
   extern DATAFILE* datafile;
   extern ALFONT_FONT* userFont;
   extern Model*       model;
};

void sysInit();
void sysRun();
void sysExit();
void sysError(const std::string&, ...);
bool KeyHit(int);

void Counter();


#endif // SYS_H_INCLUDED
