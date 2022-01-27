#include "sys.h"
#include "state.h"
#include "state_title.h"
#include "state_postmortem.h"
#include "particle.h"
#include "data.h"
#include <cstdlib>
#include <allegro.h>
#include <string>
#include <loadpng.h>
#include "model.h"
#include "state_menu.h"

BITMAP* globals::buffer = NULL;
BITMAP* globals::stretchBuffer = NULL;
DATAFILE* globals::datafile = NULL;
ALFONT_FONT* globals::userFont = NULL;
Model*       globals::model    = NULL;

volatile int counter = 0;

void sysInit()
{
   if (allegro_init() == -1)
      exit(1);

   install_keyboard();
   install_mouse();
   install_timer();
   show_mouse(NULL);
   srand(time(NULL));

   LOCK_VARIABLE(counter);
   LOCK_FUNCTION(Counter);
   install_int_ex(Counter, BPS_TO_TIMER(200));

   register_png_datafile_object(DAT_PNG);

   set_color_depth(SCREEN_DEPTH);
   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) == -1)
   {
      sysError("set_gfx_mode: Failed to set graphics mode to %dx%dx%d", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
   }

   globals::buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
   if (globals::buffer == NULL)
   {
      sysError("create_bitmap: Failed to create buffer bitmap");
   }
   clear_to_color(globals::buffer, makecol(255, 255, 255));

   globals::stretchBuffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
   if (globals::stretchBuffer == NULL)
   {
      sysError("create_bitmap: Failed to create sctretch buffer bitmap");
   }
   clear_to_color(globals::stretchBuffer, makecol(255, 255, 255));

   globals::datafile = load_datafile("media/data.dat");
   if (globals::datafile == NULL)
   {
      sysError("load_datafile: Failed to load datafile");
   }

   if (alfont_init() != ALFONT_OK)
   {
      sysError("alfont_init: Failed to initialize AlFont");
   }

   globals::userFont = alfont_load_font("media/font.TTF");
   if (globals::userFont == NULL)
   {
      sysError("alfont_load_font: Failed to load font");
   }

   alfont_set_font_size(globals::userFont, 24);

   globals::stateID = STATE_TITLE;
   globals::currentState = new StateTitle();

   globals::model = new Model();
   InitializeLevels();
   globals::model->LoadUnlocked();
}

void sysRun()
{
   while (globals::stateID != STATE_EXIT)
   {
      while (counter > 0)
      {
         globals::currentState->HandleEvents();
         ChangeState();

         globals::currentState->Update();

         counter--;
      }

      globals::currentState->Render();
      //draw_sprite(globals::buffer, mouse_sprite, mouse_x, mouse_y);
      blit(globals::buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      clear_to_color(globals::buffer, makecol(0, 0, 0));
   }
}

void sysExit()
{
   unload_datafile(globals::datafile);
   destroy_bitmap(globals::buffer);
   destroy_bitmap(globals::stretchBuffer);
   allegro_exit();
}

void sysError(const std::string& str, ...)
{
   static char textBuffer[1024];
	va_list args;
	va_start(args, str.c_str());
	vsprintf(textBuffer, str.c_str(), args);
	va_end(args);
   allegro_message(textBuffer);
   sysExit();
	exit(1);
}

bool KeyHit(int k) {
  static bool kh[256];
  if (key[k]) {
    if (kh[k]) {
      return false;
    } else {
      kh[k] = true;
      return true;
    }
  } else {
    kh[k] = false;
    return false;
  }
}

void Counter()
{
  counter++;
}
END_OF_FUNCTION(Counter);
