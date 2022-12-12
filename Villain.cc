#include "Villain.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"

Villain::Villain(Point cen, ALLEGRO_COLOR col, Vector spd) : Enemy(cen, col, spd),
							 projSpeed(Vector(-400, 0)),
							 fireSpeed(rand() % 50 + 30),
							 lives(1), dAnim(0),
							 dAnim_complete(false), fire(true)
{
   load_assets();
}
						
void Villain::load_assets() {
   fireDelay = std::make_shared<Timer> (60);   
   fireDelay->create();
   fireDelay->startTimer();
}
	
Villain::~Villain() {
   fireDelay.reset();
}

// getters e setters
void Villain::setFire(bool f) { fire = f; }
ALLEGRO_COLOR Villain::getColor() { return color; }
Vector Villain::getProjSpeed() { return projSpeed; }
int Villain::getSize() { return 20; }
Point Villain::getCentre() { return centre; }
bool Villain::getDead() { return dead; }   
bool Villain::getFire() { return fire; }
bool Villain::getdAnim_complete() { return dAnim_complete; }


// dano nos inimigos
void Villain::hit() {
   lives = lives - 1;
   if (lives < 1)
      dead = true;
}
	
// desenha a nave do inimigo na tela
void Villain::draw(std::shared_ptr<Sprite> enemyShip, std::shared_ptr<Sprite> enemyDeath) {
   if (!dead) {
      enemyShip->draw_tinted(centre, color, 0);
   }
   else {
      if (dAnim < 5) deathAnim(enemyDeath);
      else dAnim_complete = true;
   }      
}

void Villain::deathAnim(std::shared_ptr<Sprite> enemyDeath) {
   enemyDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}

// atualiza posicao dos inimigos
void Villain::update(double dt) {
   centre = centre + speed * dt;
				
   if (centre.x < 0) {
      dead = true;
      return;
   }
   
   // checa se o inimigo esta na tela pelo eixo y
   if (centre.y > 600 - 20 &&speed.y > 0)  
      speed.reflectY();
   if (centre.y < 0 - 20 && speed.y < 0)
      speed.reflectY();


   if (fireDelay->getCount() > fireSpeed) {
      fire = true;
      fireDelay->stopTimer();
      fireDelay->resetCount();
      fireDelay->startTimer();
   }
}
