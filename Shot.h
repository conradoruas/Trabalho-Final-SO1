#ifndef SHOT_H
#define SHOT_H

#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>

#include <iostream>

#include "Point.h"
#include "Vector.h"
#include "Drawable.h"
#include "Updateable.h"

// Representa um objeto tiro
class Shot : public Drawable, public Updateable {
  public:
   Point centre;
   ALLEGRO_COLOR color;
   Vector speed;
   bool live;
   
   Shot(Point p, ALLEGRO_COLOR c, Vector s) : centre(p), color(c), speed(s)
   {
      live = true;
   }

   virtual ~Shot() { }

   virtual void load_assets() = 0;
   
   void draw() { }
   void update(double dt) { }
   
};

#endif
