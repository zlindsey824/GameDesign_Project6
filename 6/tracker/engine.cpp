#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "subjectSprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "twoWayMultiSprite.h"
#include "collisionStrategy.h"
#include "bullets.h"
#include "sound.h"

const SDL_Color yellow = {255, 255, 0, 255};

Engine::~Engine() {


  delete player;
  if (balloonList.size() > 0){
		for ( SmartSprite* balloon : balloonList ) {
		  delete balloon;
		}
  }
  if (freeList.size() > 0){
		for ( SmartSprite* balloon : freeList ) {
		  delete balloon;
		}
  }
  if (strategies.size() > 0){
    for ( CollisionStrategy* strategy : strategies ) {
      delete strategy;
    }
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc.getRenderer() ),
  hudTips(HudTips::getInstance()),
  hudGoal(HudGoal::getInstance()),
  gameEnding(HudGameOver::getInstance()),
  menu(),
  cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor")),
  rainbow("rainbow", Gamedata::getInstance().getXmlInt("rainbow/factor")),
  viewport( Viewport::getInstance() ),
  player(new SubjectSprite("Helicopter")),
  balloonTotal(Gamedata::getInstance().getXmlInt("numberOfBalloons")),
  playerLives(Gamedata::getInstance().getXmlInt("numberOfLives")),
  balloonList(),
  freeList(),
  strategies(),  //SDL_Color loseOutlineColor;
  currentStrategy(0),
  playerDeath(0),
  balloonsExploded(0),
  explosionsFinished(false),
  sound(),
  collision(false),
  godMode(false),
  makeVideo( false )

{
  balloonList.reserve(balloonTotal);
  freeList.reserve(balloonTotal);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (unsigned int i = 0; i < balloonTotal; ++i) {
    balloonList.push_back( new SmartSprite("Balloon", pos, w, h) );
    player->attach( balloonList[i] );
  }

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
  hudTips.setVisible(false);
}


void Engine::draw() const {
  //world.draw();
  cloud.draw();
  rainbow.draw();

	for ( const SmartSprite* balloon : balloonList ) {
    balloon->draw();
  }

	player->draw();

	hudTips.draw();
	hudGoal.draw(balloonTotal, balloonList.size(), playerLives);

 SDL_Rect rect;
 rect.x = 15;
 rect.y = 15;
 rect.w = 250;
 rect.h = 150;
 //this is small hud that display F1 HUD and FPS and Name
 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
 // Now set the color for the hud:
 SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );
 // Render rect
 SDL_RenderFillRect( renderer, &rect );
 // Now set the color for the outline of the hud:
 SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
 SDL_RenderDrawRect( renderer, &rect );

  io.writeText("Press F1 for tips/pool", 30, 90);



  std::stringstream string_fps;
  SDL_Color fpsColor = yellow;
  string_fps << "FPS: " << clock.getFps();
  io.writeText(string_fps.str(), fpsColor, 30, 60);

  //5. Task: add addtional colors
  std::stringstream string_name;
  SDL_Color nameColor = {247, 233, 77, 0};
  string_name << Gamedata::getInstance().getXmlStr("myname");
  int name_loc_x = 20;
  int name_loc_y = 420;
  io.writeText(string_name.str(), nameColor, name_loc_x, name_loc_y);



  std::stringstream strm;
  strm << "God Mode: ";
  if (godMode) strm << "ON";
  else strm << "OFF";
  io.writeText(strm.str(), yellow, 30, 120);
  /*
  std::stringstream strmNum;
  strmNum << balloonsExploded << " balloons shot";
  io.writeText(strmNum.str(), yellow, 30, 150);
  */

  if ( collision ) {
    io.writeText("Oops: Collision", 500, 90);
  }


  if(playerLives == 0) {
    gameEnding.setVisible(true);
    gameEnding.draw(false);
    // io.writeText("Press R to Restart the Game", 250, 200);
    clock.pause();
  }
  if(balloonList.size() == 0) {
    gameEnding.setVisible(true);
    gameEnding.draw(true);
    // io.writeText("Press R to Restart the Game", 250, 200);
    clock.pause();
  }

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto ptr = balloonList.begin();
  while ( ptr != balloonList.end() ) {
    //(*it)->update( ticks );
    if ( player->shot(*ptr) ) {
      player->detach(*ptr);
      SmartSprite* temp = *ptr;
      temp->explode();
      sound[1];
    }
    else if ( player->collidedWith(*ptr) && !godMode){
      player->explode();
      sound[2];
      playerLives--;
    }
    else ++ptr;
  }
}

void Engine::update(Uint32 ticks) {
  cloud.update();
  rainbow.update();


  player->update(ticks);
	checkForCollisions();
	auto ptr = balloonList.begin();
  while ( ptr != balloonList.end() ) {
    (*ptr)->update(ticks);
    if ( (*ptr)->explosionDone() ) {
      freeList.push_back(*ptr);
      //delete (*ptr);
      ptr = balloonList.erase(ptr);
    }
    else ++ptr;
  }

  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_F1]) {
        		hudGoal.setVisible(!hudGoal.isVisible());
            hudTips.setVisible(!hudTips.isVisible());
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
	      if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          clock.pause();
          menu.play();
          clock.unpause();
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          godMode = !godMode;
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the ev  player->draw();ent loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<SubjectSprite*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<SubjectSprite*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<SubjectSprite*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<SubjectSprite*>(player)->down();
      }
      if (keystate[SDL_SCANCODE_SPACE]) {
        static_cast<SubjectSprite*>(player)->shoot();
        sound[0];
      }

      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
return false;
}
