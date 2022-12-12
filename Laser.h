#ifndef LASER_H
#define LASER_H

#include <allegro5/allegro.h>

#include "Point.h"
#include "Vector.h"
#include "Shot.h"

class Laser : public Shot {
  public:
   Laser (Point p, ALLEGRO_COLOR c, Vector s);   
   ~Laser();
   
   void draw();
   void update(double dt);
   void load_assets() { }
   
  private:
   bool in_bound();
};

#endif
