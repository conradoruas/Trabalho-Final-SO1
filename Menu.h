#ifndef MENU_H
#define MENU_H

#include <memory>

#include "State.h"

#include <allegro5/allegro.h>
class Sprite;
class Timer;

class Menu {
  public:
   Menu(int fps);
   ~Menu();
   
   void init();
   void draw();
   bool animationInProgress();
   void handleKey(const ALLEGRO_KEYBOARD_STATE&, gs::state&);
   void handleEvent(const ALLEGRO_EVENT&, gs::state&);
  private:
   int _fps;
   int _timerAnim;
   std::shared_ptr<Timer> _timer;
   std::shared_ptr<Sprite> _titleSprite;
   
};

#endif 
