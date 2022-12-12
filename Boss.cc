#include "Boss.h"

#include "Point.h"
#include "Vector.h"
#include "Timer.h"
#include "Sprite.h"
#include <iostream>

Boss::Boss(Point cen, ALLEGRO_COLOR c, Vector spd) : Enemy(cen, c, spd),
						     projSpeed(Vector(-400, 0)),
						     fireSpeed(400),
						     lives(30), dAnim(0),
						     dAnim_complete(false), fire(true)
{
   hitbox = 1;
   targetable = false;
   col = 0;
   row = 0; 
   load_assets();
}

void Boss::load_assets() {
   fireDelay = std::make_shared<Timer> (60);   
   fireDelay->create();
   fireDelay->startTimer();
}


Boss::~Boss() {
   fireDelay.reset();
}

// getters
ALLEGRO_COLOR Boss::getColor() { return color; }
Vector Boss::getProjSpeed() { return projSpeed; }
int Boss::getSize() { return hitbox; }
Point Boss::getCentre() { return centre; }
bool Boss::getDead() { return dead; }   
bool Boss::getFire() { return fire; }
bool Boss::getdAnim_complete() { return dAnim_complete; }
bool Boss::getAlive() { return aliveBoss; }

// setteres
void Boss::setFire(bool f) { fire = f; }

void Boss::hit() {
   lives -= 1;
   if(lives < 1) {
      dead = true;
   }
}

void Boss::draw(std::shared_ptr<Sprite> bossShip, std::shared_ptr<Sprite> bossDeath) {
   chooseFrame();
   if (!dead) {
      bossShip -> draw_boss(row, col, 200, 200, centre, 0);
   }
   else {
      if (dAnim < 5) 
	 deathAnim(bossDeath);
      else
	 dAnim_complete = true;
   }
}

void Boss::deathAnim(std::shared_ptr<Sprite> bossDeath) {
   bossDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}

void Boss::update(double dt) {
   centre = centre + speed * dt;
   
   if(centre.x < 700 && !targetable) {
      targetable = true;
      hitbox = 80;      
      speed.x = 0;            
      speed.y = 100;    
   }
   if (centre.y > 450 && speed.y > 0) {
      speed.reflectY();
   }
   if (centre.y < 150 && speed.y < 0) { 
      speed.reflectY();
   }
   if (fireDelay->getCount() > fireSpeed) {
      fire = true;
      fireDelay->srsTimer();
   }	 
}

void Boss::chooseFrame() {
   if (lives > 60){
      spriteSheetIndex = 0;
   }
   row = spriteSheetIndex / 3;
   col = spriteSheetIndex % 3;

}
