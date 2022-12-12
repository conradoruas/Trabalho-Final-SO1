#include "Window.h"

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdexcept>
#include <iostream>

#include "Menu.h"
#include "State.h"
#include "Action.h"
#include "Root.h"
#include "Game.h"
#include "Timer.h"

Window::Window(int w, int h, int fps) : _displayWidth(w), _displayHeight(h), 
					_fps(fps),
					_gameScore(-1),
					_state(gs::state::MENU),
					_timer(NULL),
					_eventQueue(NULL)
{
   
}

Window::~Window() {
   if (_timer != NULL) al_destroy_timer(_timer);
   if (_eventQueue != NULL) al_destroy_event_queue(_eventQueue);
   if (_display != NULL) al_destroy_display(_display);
   _root.reset();
   _menu.reset();
}


// inicializa o Allegro, a tela _display...
void Window::init() {
   // initialize allegro
   al_init();
   // create the display
   if ((_display = al_create_display(_displayWidth, _displayHeight)) == NULL) {
      std::cout << "Cannot initialize the display\n";
      exit(1); 
   }   
   // inicializa os addons
   al_init_primitives_addon();
   al_init_font_addon();
   al_init_ttf_addon();
   al_init_image_addon();
   // inicializa os timers
   if ((_timer = al_create_timer(1.0 / _fps)) == NULL) {
      std::cout << "error, could not create timer\n";
      exit(1);
   }
   if ((_eventQueue = al_create_event_queue()) == NULL) {
      std::cout << "error, could not create event queue\n";
      exit(1);
   }
   // cria uma _eventQueue para o Allegro
   al_register_event_source(_eventQueue, al_get_display_event_source(_display)); 
   al_register_event_source(_eventQueue, al_get_timer_event_source(_timer));
   al_start_timer(_timer);
   // install keyboard
   if (!al_install_keyboard()) {
      std::cerr << "Could not install keyboard\n";
   }
   
   // registra o teclado
   al_register_event_source(_eventQueue, al_get_keyboard_event_source());
}

void Window::run() {
   float prevTime = 0;
   
   while (_state != gs::state::EXIT) {
      gameState(prevTime, _state);
   }
}


void Window::gameState(float& prevTime, gs::state currentState) {
   switch (currentState) {
      case gs::state::PLAY:
	 if (_root && _menu) {
	    _menu.reset();
	 }
	 gameLoop(prevTime);
	 break;

      case gs::state::MENU:
	 if (!_menu) {
	    addMenu();
	 }
	 menuLoop();
	 break;

      case gs::state::LOAD:
	 menuLoop();
	 break;

      case gs::state::EXIT:
	 return;
   }
}

void Window::menuLoop() {
   ALLEGRO_EVENT event;
   ALLEGRO_KEYBOARD_STATE kb;
   al_wait_for_event(_eventQueue, &event);
   al_get_keyboard_state(&kb);

   _menu->handleEvent(event, _state);
   _menu->handleKey(kb, _state);
   if (_state == gs::state::LOAD && !_root) {
      addGame();
   }
}


void Window::gameLoop(float& prevTime) {
   ALLEGRO_EVENT event;
   ALLEGRO_KEYBOARD_STATE kb;
   bool redraw = true;
   float crtTime;
   
   // input do teclado
   al_get_keyboard_state(&kb);      
   _root->input(kb);
   
   // get event
   al_wait_for_event(_eventQueue, &event);
   
   // fecha a janela
   if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      _state = gs::state::EXIT;
      return;
   }
   
   if (event.type == ALLEGRO_EVENT_TIMER) {
      crtTime = al_current_time();
      update(crtTime - prevTime);
      prevTime = crtTime;
      redraw = true;
   }
   
   // renderiza
   if (redraw && al_is_event_queue_empty(_eventQueue)) {
      redraw = false;      
      draw(); 
      al_flip_display();
   }
   
   // checa se o jogo terminou
   if (_root->is_game_over()) {
      _state = gs::state::MENU;
      _root.reset();
   }   
}

// atualiza o modo de jogo
void Window::update(double dt) {
   if (_root) {
      _root->update(dt);
   }
}

// desenha na tela
void Window::draw() {
   if (_root) {
      _root->draw();
   }
}

void Window::addGame() {
   _root = std::make_shared<Game> (_displayWidth, _displayHeight, _fps, _playerName);
   _root->init();
}

void Window::addMenu() {
   _menu = std::make_shared<Menu> (_fps);
   _menu->init();
}



   
