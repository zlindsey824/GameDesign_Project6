#ifndef _ENGINE__H
#define _ENGINE__H
#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "sound.h"
#include "menuEngine.h"



//class Player;

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;



  SDL_Renderer * const renderer;
  HudTips& hudTips;
  HudBulletPool& hudPool;
  MenuEngine menu;
  World cloud;
  World rainbow;
  Viewport& viewport;
  SubjectSprite* player;
  std::vector<SmartSprite*> sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  SDLSound sound;
  bool collision;
  bool makeVideo;


  void draw() const;
  void update(Uint32);
  void printScales() const;
  void checkForCollisions();
};
#endif
