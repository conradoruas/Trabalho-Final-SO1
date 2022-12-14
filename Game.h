#ifndef Game_H
#define Game_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <list>
#include <vector>
#include <memory>
#include <string>
#include "Drawable.h"
#include "Root.h"
#include "Vector.h"
#include "Background.h"
#include "Score.h"

struct Point;
class Timer;
class Shot;
class Enemy;
class Player;
class Sprite;
class Font;

extern const int GAME_OVER_WAIT_TIME;
extern const int HIGH_SCORE_DISPLAY_TIME;
extern const int WEAPON_DELAY_LASER;
extern const int WEAPON_DELAY_MISSILE;
extern const Vector PLAYER_SHOT_SPEED;

class Game : public Root {
  public:
   // Game has public access to fps, displayWidth, and displayHeight
   Game(int w, int h, int f, std::string playerName);
   ~Game();
   
   // inherited from Root
   void update(double);
   void draw();
   
   // inherited from Root (pure virtual)
   void init();
   void input(ALLEGRO_KEYBOARD_STATE&);   
   bool is_game_over();
   void updateScore(ALLEGRO_COLOR&);

   // draw functions for components - called by draw
   void drawBossIncomingMessage(); //================
   void drawLives();
   void drawWeaponUpgradeStatus();
   void drawShots();
   void drawEnemies();
   void showGameOverMessage();
   void drawHighScores();

   // update functions for components - called by update
   void updateBoss(); //===========
   void updateBossSpawnCondition();
   void updateBombSpawnCondition();
   void updateShotPosition(double);
   void updateEnemyPosition(double);
   void updateHighscores();
  // colisions
   void collision();
   void checkCollisionOnPlayer();
   void checkCollisionOnEnemies();
   void checkCollidingEnemyWithPlayer();

   void clean();
   void cullPlayer();
   void cullShots();
   void cullEnemies();
   void respawnPlayer();
   
   // add functions for game components
   void setupPlayer();   
   void addLaser(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addMissile(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addBomb(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addVillain(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addBoss(const Point&, const ALLEGRO_COLOR&, const Vector&);
   void addPlayerLaserSingleShot();
   void addPlayerLaserDoubleShot();
   void addPlayerLaserTripleShot();
   void addPlayerMissileSingleShot();
   
   // utility functions
  //  void spawn();
   void spawnBoss();
   void spawnPurpleEnemies();
   void spawnBomb();
   void bossFire(std::shared_ptr<Enemy>);
   void CircleLaser(std::shared_ptr<Enemy>);
   bool doHitboxesIntersect(const Point&, const int&,
			    const Point&, const int&);   
   bool doColorsMatch(const ALLEGRO_COLOR&, const ALLEGRO_COLOR&);
   bool isPointBoxCollision(const Point&, const Point&, const int&);

   
  private:   
   // high score
   std::shared_ptr<Score> _highscores;
   // fonts
   ALLEGRO_FONT* _font24 = NULL;
   std::shared_ptr<Font> gameOverFont;
   std::shared_ptr<Font> gameScoreFont;   
   // timers
   std::shared_ptr<Timer> gameOverTimer;
   std::shared_ptr<Timer> playerWeapon1;
   std::shared_ptr<Timer> playerWeapon2;
   std::shared_ptr<Timer> playerRespawn;
   std::shared_ptr<Timer> upgradeText;
   std::shared_ptr<Timer> bossTimer;
   std::shared_ptr<Timer> bossSpawnCondTimer;
   std::shared_ptr<Timer> bombSpawnCondTimer;
   // base classes
   std::list< std::shared_ptr<Shot> > proj;
   std::list< std::shared_ptr<Enemy> > enem;   
   std::shared_ptr<Player> player;
   // background
   std::shared_ptr<Background> bg;
   // sprites
   std::shared_ptr<Sprite> playerShip;
   std::shared_ptr<Sprite> enemyShip;
   std::shared_ptr<Sprite> enemyDeath;
   std::shared_ptr<Sprite> bossShip;
   std::shared_ptr<Sprite> enemyBomb;
   // player name
   std::string _playerName;
   
   // flags
   bool gameOver;
   bool bossExists = false;
   bool bossIncoming = false;
   bool bossSpawnCond = false;
   bool bombSpawnCond = false;
   bool _Boss = false;
   bool killedBoss = false;
   bool writeComplete = false;
   bool bossFirstShot = false;

   // some game options
   int playerLives;
   int playerScoreTotal;
   int playerScore;
};

#endif
