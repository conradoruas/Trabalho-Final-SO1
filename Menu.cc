#include <stdexcept>
#include <iostream>

#include "Menu.h"
#include "Sprite.h"
#include "Timer.h"
#include "State.h"

Menu::Menu(int fps) : _fps(fps), _timerAnim(0)
{
   
}


Menu::~Menu() {

}

void Menu::init() {
   // especifica o path dos recursos
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));
   // pega a imagem do menu
   _titleSprite = std::make_shared<Sprite> ("Titlescreen2.png");
   // remove da memoria o path
   al_destroy_path(path);
   
   _timer = std::make_shared<Timer> (_fps); _timer->create();   
}

bool Menu::animationInProgress() {
   if (!_timer->isRunning()) {
      _timer->startTimer();
   }
   if (_timer->getCount() > 2) {
      _titleSprite->draw_menu_anim(_timerAnim++, 0);
      if (_timerAnim > 6) {
         _timerAnim = 0;
         _timer->stopTimer();
         _timer->resetCount();
         return false;
      }
      _timer->resetCount();
   }
   else 
      _titleSprite->draw_menu_anim(_timerAnim, 0);
   
   return true;
}


void Menu::draw() {
   _titleSprite->draw_menu_anim(5, 0);
}

void Menu::handleKey(const ALLEGRO_KEYBOARD_STATE& kb, gs::state& st) {
   if (st == gs::state::EXIT) {
      return;
   }
   // aperta 1 pra jogar
   else if (al_key_down(&kb, ALLEGRO_KEY_1)) {
      st = gs::state::LOAD;
   }
   // aperta esc para sair
   else if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
      st = gs::state::EXIT;
   }
   
}

void Menu::handleEvent(const ALLEGRO_EVENT& event, gs::state& st) {
   if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      st = gs::state::EXIT;
      return;
   }
   if (event.type == ALLEGRO_EVENT_TIMER) {
      // carrega o game quando a animacao acabar
      if (st == gs::state::LOAD) {
         if (!animationInProgress()) {
         st = gs::state::PLAY;
         return;
      }
      al_flip_display();
   }
      else {
         draw();
         al_flip_display();
      }
   }
}

