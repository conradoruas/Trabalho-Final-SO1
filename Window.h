#ifndef WINDOW_H
#define WINDOW_H

#include <allegro5/allegro.h>
#include <memory>
#include <string>

#include "State.h"

class Menu;
class Root;

class Window {
      
  public:
   Window(int w, int h, int fps);
   ~Window();
   
   void init();
   void run();
   void draw();
   void update(double dt);
   void gameState(float& prevTime, gs::state currentState);
   void menuLoop();
   void gameLoop(float& prevTime);

   inline void setName(std::string playerName) {
      _playerName = playerName;
   }   
   inline int getWidth() const {
      return _displayWidth;
   }
   inline int getHeight() const {
      return _displayHeight;
   }
   inline int getFps() const {
      return _fps;
   }

  private:
   // variaveis gerais da classe
   int _displayWidth;
   int _displayHeight;
   int _fps;
   int _gameScore;
   std::string _playerName;   
   gs::state _state;
   // objetos allegro
   ALLEGRO_TIMER *_timer;
   ALLEGRO_EVENT_QUEUE *_eventQueue;
   ALLEGRO_DISPLAY *_display;
   // Tela de Menu
   std::shared_ptr<Menu> _menu;
   // Tela Inicial
   std::shared_ptr<Root> _root; 

   // Funcoes que adicionam componentesS
   void addGame();
   void addMenu();
};





#endif
