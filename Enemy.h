#ifndef ENEMY_H
#define ENEMY_H

#include <allegro5/allegro.h>

#include <memory>

#include "Point.h"
#include "Vector.h"
#include "Updateable.h"

class Sprite;

class Enemy : public Updateable {

  public:
   Vector speed;/**< vetor velocidade do inimigo */
   Point centre;/**< ponto central do inimigo */
   ALLEGRO_COLOR color;/**< cor do inimigo */
   bool dead;

  Enemy(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s), dead(false)
   { }

   virtual ~Enemy() { }

   void update(double dt) { }
   virtual void draw(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> death) = 0;
   
   virtual ALLEGRO_COLOR getColor() = 0;
   virtual Vector getProjSpeed() = 0;
   virtual Point getCentre() = 0; 
   virtual bool getdAnim_complete() = 0; 
   virtual bool getDead() = 0;
   virtual bool getFire() = 0;
   virtual void setFire(bool f) = 0;
   
   virtual void hit() = 0;

};

#endif
