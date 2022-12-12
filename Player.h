#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include <memory>

#include "Shot.h"
#include "Action.h"
#include "Updateable.h"

struct Point;
struct Vector;
class Sprite;

class Player : public Updateable {
  public:
   Point centre;        /**< posicao da nave */
   ALLEGRO_COLOR color; /**< cor da nave */
   
   Vector speed;        /**< vetor velocidade */
   float lives;         /**< vidas do jogador */
   int row;             /**< animacao vertical da nave */
   int col;             /**< animacao horizontal da nave */
   bool dead;           /**< identificador de morte */

   Player(Point p, ALLEGRO_COLOR c);
   ~Player();

   void hit(int);
   void draw(std::shared_ptr<Sprite> sprite, int flags);
   void update(double);

   act::action input(ALLEGRO_KEYBOARD_STATE&);

  private:
   // funcoes auxiliares
   void shipAnimation();
   void checkBoundary();
   void drawLifes();
   
};

#endif

