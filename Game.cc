#include "Game.h"

#include <iostream>
#include <fstream>

#include "Point.h"
#include "Vector.h"
#include "Shot.h"
#include "Drawable.h"
#include "Player.h"
#include "Laser.h"
#include "Missile.h"
#include "Sprite.h"

#include "Action.h"
#include "Timer.h"
#include "Font.h"

#include "Enemy.h"
#include "Villain.h"
#include "Bomb.h"
#include "Boss.h"

Game::Game(int w, int h, int f, std::string playerName) :
   Root(w, h, f),
   _playerName(playerName),
   gameOver(false),
   playerLives(3),
   playerScoreTotal(0),
   playerScore(0)
{
   
}
Game::~Game() {
   player.reset();
   bg.reset();
   proj.clear();
   enem.clear();

   gameOverTimer.reset();
   playerWeapon1.reset();
   playerWeapon2.reset();
   playerRespawn.reset();
   upgradeText.reset();

   al_destroy_font(_font24);
}

void Game::init() {
   // timers
   gameOverTimer = std::make_shared<Timer> (framesPerSec); gameOverTimer->create();
   playerWeapon1 = std::make_shared<Timer> (framesPerSec); playerWeapon1->create();   
   playerWeapon2 = std::make_shared<Timer> (framesPerSec); playerWeapon2->create();
   playerRespawn = std::make_shared<Timer> (framesPerSec); playerRespawn->create();
   bossTimer     = std::make_shared<Timer> (framesPerSec); bossTimer->create();
   upgradeText   = std::make_shared<Timer> (framesPerSec); upgradeText->create();
   bossSpawnCondTimer = std::make_shared<Timer> (framesPerSec); bossSpawnCondTimer->create();
   bombSpawnCondTimer = std::make_shared<Timer> (framesPerSec); bombSpawnCondTimer->create();

   playerWeapon1->startTimer();
   playerWeapon2->startTimer();
   bossSpawnCondTimer->startTimer();
   bombSpawnCondTimer->startTimer();
   // cria o objeto Player
   setupPlayer();
   // adiciona o path dos recursos
   ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
   al_append_path_component(path, "resources");
   al_change_directory(al_path_cstr(path, '/'));   
   // fonts
   _font24 = al_load_font("ipag.ttf", 24, 0);
   gameScoreFont = std::make_shared<Font> ("ipag.ttf", 18); gameScoreFont->load();   
   gameOverFont = std::make_shared<Font> ("DavidCLM-BoldItalic.ttf", 64); gameOverFont->load();
   // instancia o background
   bg = std::make_shared<Background> (Vector(50, 0), Vector(-90, 0));
   // instancia os sprites dos objetos
   enemyDeath = std::make_shared<Sprite> ("explode.png");
   enemyBomb  = std::make_shared<Sprite> ("spikebomb.png");
   bossShip   = std::make_shared<Sprite> ("bossv2.png");
   enemyShip = std::make_shared<Sprite> ("EnemyBasic.png");
   playerShip = std::make_shared<Sprite> ("Sprite2.png");
   // deleta o path da memoria
   al_destroy_path(path);
}

void Game::update(double dt) {
   // atualiza  background
   bg->update(dt);
   // atualiza player
   if (player) {
      player->update(dt);
   }
   // gameover condition
   else if (!player && playerLives <= 0) {
      gameOver = true;
   }
   // respawna o player
   else {
      respawnPlayer();
   }

   // atualiza o Boss
   updateBoss();
   
   // atualiza os Tiros
   updateShotPosition(dt);
   // atualiza a posicao dos inimigos
   updateEnemyPosition(dt);
   // lida com todos os tipos de colisoes
   collision();
   // remove os objetos fora da tela ou mortos
   clean();      
}

void Game::draw() {
   // desenha background
   bg->draw();
   // desenha indicador de vidas
   drawLives();
   // desenha os tiros na tela
   drawShots();
   // desenha os inimigos na tela
   drawEnemies(); 
   // desenha a mensagem do boss
   drawBossIncomingMessage();
   
   if (gameOver) {
      showGameOverMessage();
   }
   else if (player) {
      player->draw(playerShip, 0);
   }
   gameScoreFont->drawTextF(al_map_rgb(255, 255, 255), 100, 50,"Score: %i", playerScoreTotal);
}

void Game::input(ALLEGRO_KEYBOARD_STATE& kb) {
   if (player) {
      switch (player->input(kb)) { // movimentos ocorrem no player
	 case act::action::QUIT_GAME: 
	    player.reset();
	    return;
	 case act::action::FIRE_PRIMARY: // atira os lasers  
	    if (playerWeapon1->getCount() > 6) {
	       addPlayerLaserSingleShot();
	    }	    
	    break;
	 case act::action::FIRE_SECONDARY: // atira os misseis
	    if (playerWeapon2->getCount() > 20) {
	       addPlayerMissileSingleShot();
	    }
	    break;
	    
	 default:
	    break;
      }
   }
}


bool Game::is_game_over() {
   if (gameOver && gameOverTimer->getCount() > 100 + 200) {
      bg->draw();
      return true;
   }
   return false;
}

void Game::spawnPurpleEnemies() { // spawna inimigos no formato de <
    addVillain(Point(800, 300), al_map_rgb(246, 64, 234),Vector(-180, 0));
	 addVillain(Point(900, 350), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addVillain(Point(900, 250), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addVillain(Point(1000, 400), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addVillain(Point(1000, 200), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addVillain(Point(1100, 100), al_map_rgb(246, 64, 234), Vector(-180, 0));
	 addVillain(Point(1100, 500), al_map_rgb(246, 64, 234), Vector(-180, 0));
}

void Game::spawnBomb() { // spawna as bombas
   Point pt(800, 200);
   pt.y = pt.y + (rand()%200);
   addBomb(pt, al_map_rgb(204,3,3), Vector(-60, 0));
}

void Game::CircleLaser(std::shared_ptr<Enemy> E) { // explosao de lasers das bombas
   for(int i = -500; i <= 500; i += 325) {
      for(int j = -500; j <= 500; j += 325) {
	 addLaser(E->getCentre(), E->getColor(), Vector(i, j));
      }
   }
   E->setFire(false);

}

void Game::bossFire(std::shared_ptr<Enemy> e) {
   int n = rand() % 7 + 1;
   Point playerloc;
if (player) {
      playerloc = player->centre;
   }
   Vector aim(0, 0);
   
   switch(n) {
      case 1:
	 for(int i=-100; i<=100; i+=20)
	    addLaser(e->centre+Point(0, i), e->color, e->getProjSpeed());
	 break;
      case 2:
	 aim.Angle(playerloc, e->centre, 0.9);
	 for(int i = -70; i <= 70; i += 20)
	    addLaser(e->centre+Point(50,0), e->color, aim+ Vector(-30,i));
	break;
      case 3:
	 addBomb(e->centre+Point(50,0), al_map_rgb(204,3,3), Vector(-100, 0));
	 break;
      default:
	 aim.Angle(playerloc, e->centre+Point(0,50), 0.9);
	 addMissile(e->centre+Point(0,50), al_map_rgb(204, 3, 3), aim);
	 aim.Angle(playerloc, e->centre+Point(0,-50), 0.9);
	 addMissile(e->centre+Point(0,-50), al_map_rgb(204, 3, 3), aim);
	 break;
   }
}
   
void Game::updateBoss() {
   // spawn condition
   updateBossSpawnCondition();
   if (bossSpawnCond && !bossExists) {
      // start timer
      bossSpawnCond = false;
      if (bossTimer->isRunning() == false) {
         bossTimer->startTimer();
         bossIncoming = true;
      }
      if (bossTimer->getCount() > 250) {
         spawnBoss();
         bossTimer->stopTimer();
         bossTimer->resetCount();
         bossIncoming = false;
      }    
   }
}

void Game::addBoss(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<Boss> (cen, col, spd));
}

void Game::spawnBoss() {
   addBoss(Point(850, 300), al_map_rgb(155, 0, 0), Vector(-50, 0));
   bossExists = true;

}

void Game::drawBossIncomingMessage() {
   if (bossTimer->isRunning() == true && bossIncoming == true) {
      gameOverFont->drawTextCentered(al_map_rgb(204, 204, 0), "BOSS INCOMING");
      if (bossTimer->getCount() > 200) {
	 bossIncoming = false;
      }      
   }
}

void Game::updateHighscores() {
   if (writeComplete == false) {
      _highscores = std::make_shared<Score> ("leaderboard.txt");
      _highscores->init();
      _highscores->readPrev();
      if (_highscores->getMin() < playerScoreTotal) {
	 _highscores->swapWithMin(_playerName, playerScoreTotal);
	 _highscores->sortRecords();
      }
      _highscores->update();
      writeComplete = true;
   }
}

void Game::updateScore(ALLEGRO_COLOR& c) {
   if (player && doColorsMatch(player->color, c)) {
      playerScore += 1;
      playerScoreTotal += 1;
   }
}

void Game::updateBossSpawnCondition() {
   // controla que o boss apareca em 60 segundos
   if (!bossExists && !bossSpawnCond && 
   bossSpawnCondTimer->getCount() % (60 * framesPerSec) == 0) {
      bossSpawnCond = true;
   } else if (bossExists && bossSpawnCond) {
      bossSpawnCond = false;
   } else if (bossExists && !bossSpawnCond) {
      bossSpawnCondTimer->resetCount();
   }
}

void Game::updateBombSpawnCondition() {
   // gera as bombas a cada 30 segundos
   if (!bombSpawnCond && bombSpawnCondTimer->getCount() % (30 * framesPerSec) == 0) {
      bombSpawnCond = true;
   } else if (bombSpawnCond) {
      spawnBomb();
      bombSpawnCond = false;
      bombSpawnCondTimer->resetCount();
   }
}

void Game::updateShotPosition(double dt) {
   if (!proj.empty()) {
      for (auto it = proj.begin(); it != proj.end(); ++it) {
	 (*it)->update(dt);
      }
   }
}

void Game::updateEnemyPosition(double dt) {
   if (!enem.empty()) {
      for (auto enemy = enem.begin(); enemy != enem.end(); ++enemy) {
	 (*enemy)->update(dt);
	 // tiros dos inimigos	 
	 if ((*enemy)->getFire()) {
	    // explosao lasers da bomba	    
	    if (doColorsMatch((*enemy)->color, al_map_rgb(204,3,3))) {
	       CircleLaser((*enemy));
	    }
	    // tiros do boss
	    if (doColorsMatch((*enemy)->color, al_map_rgb(155, 0, 0))) {
	       bossFire((*enemy));
	    }	    
	    // inimigos roxos atirando dobrado
	    else if (doColorsMatch((*enemy)->color, al_map_rgb(246, 64, 234))) {
	       addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->getProjSpeed() + Vector(0, 40));
	       addLaser((*enemy)->centre, (*enemy)->color, (*enemy)->getProjSpeed() + Vector(0, -40));
	    }	    
	    else { // inimigos roxos atirando normalmente (horizontal)
	       addLaser((*enemy)->centre + Vector(20, 0), (*enemy)->color, (*enemy)->getProjSpeed());
	    }
	    (*enemy)->setFire(false);
	 }
      }
   }
   updateBombSpawnCondition();

   if (enem.size() <= 3 && bossExists == false && !bossIncoming) {
      spawnPurpleEnemies();
   }
}

void Game::collision() {
   checkCollisionOnPlayer();
   checkCollisionOnEnemies();
   checkCollidingEnemyWithPlayer();
}

void Game::checkCollisionOnEnemies() {
   if (!proj.empty() && !enem.empty() && player) {
      for (std::list< std::shared_ptr<Shot> >::iterator it_proj = 
	      proj.begin(); it_proj != proj.end(); ++it_proj) {
	    
	 if (doColorsMatch(player->color, (*it_proj)->color)) {
	    for (std::list< std::shared_ptr<Enemy> >::iterator it_enem = 
		    enem.begin(); it_enem != enem.end(); ++it_enem) {
		  
	       Point shot = (*it_proj)->centre;
	       Point enemy = (*it_enem)->getCentre();

	       if ((shot.x > enemy.x - 20) &&
		   (shot.x < enemy.x + 20) &&
		   (shot.y > enemy.y - 20) &&
		   (shot.y < enemy.y + 20)) {
		     
		  (*it_proj)->live = false;
		  (*it_enem)->hit();
		     
		  if ((*it_enem)->getDead()) {
		     updateScore(player->color);
		  }
	       }
	    }
	 }
      }
   }
}

void Game::checkCollidingEnemyWithPlayer() {
   if (!enem.empty() && player) {      
      for (auto it = enem.begin(); it != enem.end(); ++it) {	 
	 if ((*it)->dead == false) {	    
	    if (doHitboxesIntersect(player->centre, 16, (*it)->centre, 20)) {
	       // ocorreu a colisao, dando dano ao inimigo e ao jogador
	       (*it)->hit();
	       player->hit(1);
	    }
	 }	       
      }
   }  
}

void Game::checkCollisionOnPlayer() {
   if (!proj.empty() && player) {
      for (auto shot = proj.begin(); shot != proj.end(); ++shot) {	    
	 // checa se a cor do tiro eh diferente 
	 if (!doColorsMatch((*shot)->color, player->color)) {
	    if (isPointBoxCollision((*shot)->centre, player->centre, 16)) {	  
	       // ocorreu a colisao do tiro com o jogador, deletar o tiro e dar dano ao jogador 
	       (*shot)->live = false;
	       player->hit(1);
	    }
	 }
      }
   }  
}

void Game::clean() {
   cullPlayer();
   cullShots();
   cullEnemies();
}

void Game::cullPlayer() {
   if (player && player->dead) {
      playerLives -= 1;
      playerScore = 0;
      player.reset();
   }
}

void Game::cullShots() {
   std::list<std::shared_ptr<Shot>> newProj;
   if (!proj.empty()) {
      for (std::list< std::shared_ptr<Shot> >::iterator it = proj.begin(); 
	   it != proj.end(); ++it) {
	 if ((*it)->live)
	    newProj.push_back(*it);
      }
      proj.clear();
      proj.assign(newProj.begin(), newProj.end());      
   }
}

void Game::cullEnemies() {
   std::list< std::shared_ptr<Enemy> > newEnem;
   if (!enem.empty()) {
      for (auto it = enem.begin(); it != enem.end(); ++it) {
	 if (doColorsMatch((*it)->getColor(), al_map_rgb(155, 0, 0))) {
	    if ((*it)->getdAnim_complete()) {
	       if(playerLives<3)
		  playerLives++;
	       bossExists = false;
	       bossFirstShot = false;
	    }
	 }
	 if (!(*it)->getdAnim_complete()) {
	    newEnem.push_back(*it);
	 }
      }
      enem.clear();
      enem.assign(newEnem.begin(), newEnem.end());
   }
}

void Game::respawnPlayer() {
   if (!playerRespawn->isRunning()) {
      playerRespawn->startTimer();
   }
   if (playerRespawn->getCount() > 80) {
      setupPlayer();
      playerRespawn->stopTimer();
      playerRespawn->resetCount();
   }
}



void Game::drawEnemies() {
   if (!enem.empty()) {
      for (auto enemy = enem.begin(); enemy != enem.end(); ++enemy) {
	 if (doColorsMatch((*enemy)->getColor(), al_map_rgb(204, 3, 3))) {
	    // bomba
	    (*enemy)->draw(enemyBomb, enemyDeath);
	 }
	 else if (doColorsMatch((*enemy)->getColor(), al_map_rgb(155, 0, 0))) {
	    // boss
	    (*enemy)->draw(bossShip, enemyDeath);
	 }				
	 else {
	    // inimigo padrao
	    (*enemy)->draw(enemyShip, enemyDeath);
	 }
      }
   }
}

void Game::drawShots() {
   if (!proj.empty()) {
      for (auto shot = proj.begin(); shot != proj.end(); ++shot) { 
	 (*shot)->draw();
      }
   }
}

void Game::drawLives() {
   Point centre(displayWidth-70, displayWidth-50);
   if (playerLives > 0) {
	  al_draw_rectangle(displayWidth - 70, 50, displayWidth - 50, 70,
			    al_map_rgb(0, 255, 0), 5);
   }
   if (playerLives > 1) {
      al_draw_rectangle(displayWidth - 110, 50, displayWidth - 90, 70,
			al_map_rgb(0, 255, 0), 5);
   }
   if (playerLives > 2) {
      al_draw_rectangle(displayWidth - 150, 50, displayWidth - 130, 70,
			al_map_rgb(0, 255, 0) , 5);
   }   
   if (!player && playerLives > 0) {
      gameOverFont->drawTextCenteredF(al_map_rgb(255, 0, 0), "%i LIVES REMAINING", playerLives);
   }
}

void Game::showGameOverMessage() {
   if (!gameOverTimer->isRunning()) {
      gameOverTimer->startTimer();
      updateHighscores();
   }
   if (gameOverTimer->getCount() < 100) {
      // game over message
      gameOverFont->drawTextCentered(al_map_rgb(255, 0, 0), "GAME OVER");
   }   
   else {
      drawHighScores();
   }
}

void Game::drawHighScores() {
   int yPosition = 150;
   for (int i = 0; i < 3; i++) {
      
      al_draw_textf(_font24,           
		    al_map_rgb(255, 255, 255),  
		    300,                        
		    yPosition,                  
		    0,                          
		    "%i. %s %i", i + 1,             
		    _highscores->getName(i).c_str(),
		    _highscores->getScore(i));
      yPosition += 150;
   }
}


// Componentes do Jogo


// adiciona objeto Laser na lista de projeteis
void Game::addLaser(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   proj.push_back(std::make_shared<Laser> (cen, col, spd));
}

// cria objeto jogador
void Game::setupPlayer() {
   player = std::make_shared<Player> (Point(215, 245), al_map_rgb(0, 200, 0));
}

// adiciona inimigo
void Game::addVillain(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<Villain> (cen, col, spd));
}

// adiciona missel
void Game::addMissile(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   std::shared_ptr<Shot> missileObject = std::make_shared<Missile> (cen, col, spd);
   missileObject->load_assets();
   proj.push_back(missileObject);
}

// adiciona bomba
void Game::addBomb(const Point& cen, const ALLEGRO_COLOR& col, const Vector& spd) {
   enem.push_back(std::make_shared<Bomb>(cen, col, spd));
}

// adiciona arma 1 do player
void Game::addPlayerLaserSingleShot() {
   addLaser(player->centre + Point(-20, 0), player->color, Vector(500, 0));
   playerWeapon1->srsTimer();
}

// adiciona arma 2 do player
void Game::addPlayerMissileSingleShot() {
   addMissile(player->centre, player->color, Vector(500, 0));
   playerWeapon2->srsTimer();
}

// funcoes auxiliares

bool Game::isPointBoxCollision(const Point& p1, const Point& p2, 
				 const int& s2) {
   return ((p1.x > p2.x - s2) &&
	   (p1.x < p2.x + s2) &&
	   (p1.y > p2.y - s2) &&
	   (p1.y < p2.y + s2));
}


bool Game::doColorsMatch(const ALLEGRO_COLOR& a, const ALLEGRO_COLOR& b) {
   return (a.r == b.r && a.g == b.g && a.b == b.b);
}


bool Game::doHitboxesIntersect(const Point& centre1, const int& size1,
				 const Point& centre2, const int& size2) {
   return (abs(centre1.x - centre2.x) < (size1 + size2) &&
	   abs(centre1.y - centre2.y) < (size1 + size2));
}