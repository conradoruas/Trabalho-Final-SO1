#ifndef BOMB_H
#define BOMB_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <memory>
#include <iostream>
#include <stdexcept>

#include "Enemy.h"
#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
//#include "Timer.h"
#include "Root.h"
#include "Updateable.h"


struct Point;
struct Vector;
class Timer;

class Bomb : public Enemy {
   
  private:
   std::shared_ptr<Timer> fireDelay;
   Vector projSpeed;
   int size;// tamanho da bomba
   int row;
   int col;
   int lives; // vidas da bomba
   int dAnim; // frames da animacao
   bool dAnim_complete; // flag de fim da animacao   
   bool fire, fired, fire2; // flags auxiliares
	
  public:
   Bomb(Point cen, ALLEGRO_COLOR col, Vector spd);
   ~Bomb();
  
   void setFire(bool f);	
   ALLEGRO_COLOR getColor();
   Vector getProjSpeed(); 
   int getSize();    
   Point getCentre();
   bool getDead(); 
   bool getFire();    
   void update(double dt);
   void load_assets();
   
   void deathAnim(std::shared_ptr<Sprite> d);
   void hit();
   
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);
   bool getdAnim_complete();
   
};

#endif
