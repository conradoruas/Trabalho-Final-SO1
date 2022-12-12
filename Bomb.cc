#include "Bomb.h"
#include "Timer.h"


Bomb::Bomb(Point cen, ALLEGRO_COLOR col, Vector spd) : Enemy(cen, col, spd) {
   projSpeed = Vector(-500, 0);
   lives = 3;
   size = 20;   
   dAnim = 0;      
   dAnim_complete = false;
   fire = false;
   
   load_assets();
}
	
Bomb::~Bomb() {

}
// setters e getters
void Bomb::setFire(bool f) { fire = f; }
ALLEGRO_COLOR Bomb::getColor() { return color; }
Vector Bomb::getProjSpeed() { return speed; }
int Bomb::getSize() { return size; }
Point Bomb::getCentre() { return centre; }
bool Bomb::getDead() { return dead; }   
bool Bomb::getFire() { return fire; }
bool Bomb::getdAnim_complete() { return dAnim_complete; }


void Bomb::load_assets() {
   row =0; col=0;
   
   fireDelay = std::make_shared<Timer> (60);
   fireDelay->create();
   fireDelay->startTimer();
   
   projSpeed = Vector(-500, 0);
   lives = 3;
   size = 20;   
   dAnim = 0;      
   dAnim_complete = false;
   fire = false; fired=false; fire2=false;
}

// diminui a vida da bomba em uma unidade por hit
void Bomb::hit() {
   lives = lives - 1;
   col++;
   //change anim column
   if (lives < 1)
      dead = true;
}
	
// desenha bomba na tela
void Bomb::draw(std::shared_ptr<Sprite> enemyShip, std::shared_ptr<Sprite> enemyDeath) {
   if (!dead) {
      enemyShip->draw_region(row, col, 40, 41, centre, 0);
   }
   else {
      // bomba morreu, inicia a animacao
      if (dAnim < 5) deathAnim(enemyDeath);
      else{ dAnim_complete = true;}
   }      
}

void Bomb::deathAnim(std::shared_ptr<Sprite> enemyDeath) {
   enemyDeath->draw_death_anim(dAnim, centre, 0);
   dAnim++;
}

void Bomb::update(double dt) {
   centre = centre + speed * dt;
				
   if (centre.x < 0)
      dead = true;
   
   if(centre.x<670&&row==0){
      row++;  
   }
   if(centre.x<540&& row==1){
      row++;
   }
   if(centre.x<400 && !fired){
      fire = true;
      fired = true;
      dead = true;
      fireDelay->stopTimer();
      fireDelay->resetCount();
   }
}
