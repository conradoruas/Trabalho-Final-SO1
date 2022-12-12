#ifndef VILLAIN_H
#define VILLAIN_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <memory>
#include "Updateable.h"
#include "Enemy.h"


struct Point;
struct Vector;
class Timer;
class Sprite;

class Villain : public Enemy {
  private:
   bool dAnim_complete;
   bool fire;
   int fireSpeed;
   int lives;
   int dAnim;   
   std::shared_ptr<Timer> fireDelay;   
   Vector projSpeed;

  public:

   Villain(Point p, ALLEGRO_COLOR c, Vector s);
   ~Villain();
   
   void update(double dt);
   void load_assets();
   void deathAnim(std::shared_ptr<Sprite>);
   void hit();
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);
   
   void setFire(bool f);	
   ALLEGRO_COLOR getColor();
   Vector getProjSpeed(); 
   int getSize();    
   Point getCentre();
   bool getDead(); 
   bool getFire();    
   bool getdAnim_complete();

   
};
#endif
