#ifndef BACKGROUND_H
#define BACKGROUND_H
#include "Point.h"
#include "Vector.h"
#include <allegro5/allegro.h>
#include <memory>
#include "Drawable.h"
#include "Updateable.h"
class Sprite;

class Background : public Drawable, public Updateable  {
   Point bgMid; /**<point used by the background to draw from */
   Point fgMid;
   Point fg2Mid;
   Vector bgSpeed; /**<background movement speed */
   Vector fgSpeed;
   std::shared_ptr<Sprite> bg; /**<shared pointer to background animation */
   std::shared_ptr<Sprite> fg;
   
  public:

  Background(Vector bg_, Vector fg_) : bgSpeed(bg_), fgSpeed(fg_) {
     load_assets();
   }  
   void draw();
   void update(double dt);
   
  private:
   void load_assets();
   
};

#endif   
