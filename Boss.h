#ifndef BOSS_H
#define BOSS_H

#include "Enemy.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Updateable.h"

struct Point;
struct Vector;
class Timer;
class Sprite;

class Boss : public Enemy {

  private:
   std::shared_ptr<Timer> fireDelay;

   Vector projSpeed;
   int fireSpeed; 
   int lives;
   int dAnim;
   int hitbox;

   bool aliveBoss;
   bool fire;
   bool dAnim_complete;
   bool targetable;

   int spriteSheetIndex;
   int row;
   int col;
   
  public:
   Boss (Point p, ALLEGRO_COLOR c, Vector s);
   ~Boss();


   ALLEGRO_COLOR getColor();
   Vector getProjSpeed();
   Point getCentre();
   bool getFire();
   bool getAlive();
   int getLives();
   bool getdAnim_complete();
   bool getDead();
   int getSize();

   void setFire(bool f);
   
   void update(double dt);
   void load_assets();
   void deathAnim(std::shared_ptr<Sprite>);
   void hit();
   void chooseFrame();
   void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death);
};
#endif
