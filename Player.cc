#include "Player.h"

#include "Point.h"
#include "Vector.h"
#include "Sprite.h"
#include "Action.h"
#include "Laser.h"
#include "Missile.h"

Player::Player(Point p, ALLEGRO_COLOR c) : centre(p), color(c), speed(Vector(0, 0)),
					   lives(3), row(0), col(0), dead(false)
{
   
}

Player::~Player() {
   
}

void Player::hit(int damage) {
   lives = lives - damage;
   if (lives < 1)
      dead = true;
}

void Player::draw(std::shared_ptr<Sprite> sprite, int flags) {   
   sprite->draw_region(row, col, 47.0, 40.0, centre, flags);
   drawLifes();
}

void Player::update(double dt) {
   centre = centre + speed * dt;
   shipAnimation();
   speed = Vector(0, 0);
   checkBoundary();
}

act::action Player::input(ALLEGRO_KEYBOARD_STATE& kb) {
  if (al_key_down(&kb, ALLEGRO_KEY_UP)) {
     speed.y -= 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_RIGHT)) {
     speed.x += 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_DOWN)) {
     speed.y += 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_LEFT)) {
     speed.x -= 250;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_SPACE)) {
     return act::action::FIRE_PRIMARY;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_1)) {
     return act::action::FIRE_SECONDARY;
  }
  if (al_key_down(&kb, ALLEGRO_KEY_ESCAPE)) {
     return act::action::QUIT_GAME;
  }

  return act::action::NO_ACTION;
}


void Player::shipAnimation() {
   if (speed.x > 0) {
      col = 1;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   } else {
      col = 0;
      if (speed.y > 0) row = 2;
      else if (speed.y < 0) row = 0;
      else row = 1;
   }
}

void Player::drawLifes() {   
   al_draw_line(centre.x - 16*2, centre.y + 16*2,
		(centre.x - 16*2) + (lives / 3) * (16*4),
		centre.y + 16*2,
		al_map_rgb(255 * (1.0 - lives / 3),
			   200 * (lives / 3),
			   0), 5);
}

void Player::checkBoundary() {   
   // mantem o jogador no eixo x
   if (centre.x > 800 - 16)
      centre.x = 800 - 16;
   else if (centre.x < 16)
      centre.x = 16;   
   // mantem o jogador no eixo y
   if (centre.y > 600 - 16)
      centre.y = 600 - 16;
   else if (centre.y < 16)
      centre.y = 16;
}