#include "hud.h"
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include "gameData.h"
#include "imageFactory.h"
#include "ioMod.h"
#include "sprite.h"
static const int boundary_x = 10, boundary_y = 5;
static const int interval_x = 100, interval_y = 20;
static const SDL_Color nameColor = {255, 255, 255, 0};

HudTips& HudTips::getInstance() {
  static HudTips instance;
  return instance;
}

HudTips::HudTips()
    : width(Gamedata::getInstance().getXmlInt("hudTips/width")),
      height(Gamedata::getInstance().getXmlInt("hudTips/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudTips/position/x"),
                   Gamedata::getInstance().getXmlInt("hudTips/position/y"))),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudTips/textColor/a"))}) {}

void HudTips::draw() {
  if (isVisible()) {
    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

    SDL_Rect r;
    r.x = pos[0];
    r.y = pos[1];
    r.w = width;
    r.h = height;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderDrawRect(renderer, &r);

    IoMod& io = IoMod::getInstance();
    std::stringstream strm_bullet;
    strm_bullet << "Helpful Guideline";
    IoMod::getInstance().writeText(strm_bullet.str(), r.x + boundary_x, r.y + boundary_y);
    io.writeText("W: Up", textColor, r.x + boundary_x, r.y + boundary_y + interval_y);
    io.writeText("S: Down", textColor, r.x + boundary_x + 1.5*interval_x, r.y + boundary_y + interval_y);
    io.writeText("A: Left", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 2);
    io.writeText("D: Right", textColor, r.x + boundary_x + 1.5*interval_x, r.y + boundary_y + interval_y * 2);
    io.writeText("M: Menu", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 3);
    io.writeText("Space:Shoot", textColor, r.x + boundary_x + 1.5*interval_x, r.y + boundary_y + interval_y * 3);
    io.writeText("R: Restart", textColor, r.x + boundary_x, r.y + boundary_y + interval_y * 4);
    io.writeText("P: Pause", textColor, r.x + boundary_x + 1.5*interval_x, r.y + boundary_y + interval_y * 4);
  }
}


HudBulletPool& HudBulletPool::getInstance() {
  static HudBulletPool instance;
  return instance;
}

HudBulletPool::HudBulletPool()
    : width(Gamedata::getInstance().getXmlInt("hudBulletPool/width")),
      height(Gamedata::getInstance().getXmlInt("hudBulletPool/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudBulletPool/position/x"),
                   Gamedata::getInstance().getXmlInt("hudBulletPool/position/y"))),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudBulletPool/textColor/a"))}) {}

void HudBulletPool::draw(unsigned int activebuffernum, unsigned int freebuffernum) {
  if (isVisible()) {
    SDL_Renderer* renderer = IoMod::getInstance().getRenderer();

    SDL_Rect r;
    r.x = pos[0];
    r.y = pos[1];
    r.w = width;
    r.h = height;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderDrawRect(renderer, &r);

    std::stringstream strm_bullet;
    strm_bullet << "OBJECT BULLET POOL";
    IoMod::getInstance().writeText(strm_bullet.str(), r.x + 20, r.y + boundary_y);
    strm_bullet.str("");
    strm_bullet << "bulletslist: " << activebuffernum;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 40);
    strm_bullet.str("");
    strm_bullet << "freelist: " << freebuffernum;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 65);
  }
}
