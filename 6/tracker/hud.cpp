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


HudGoal& HudGoal::getInstance() {
  static HudGoal instance;
  return instance;
}

HudGoal::HudGoal()
    : width(Gamedata::getInstance().getXmlInt("hudGoal/width")),
      height(Gamedata::getInstance().getXmlInt("hudGoal/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("hudGoal/position/x"),
                   Gamedata::getInstance().getXmlInt("hudGoal/position/y"))),
      visible(true),
      backgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/backgroundColor/r")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/backgroundColor/g")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/backgroundColor/b")),
                       static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/backgroundColor/a"))}),
      outlineColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/outlineColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/outlineColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/outlineColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/outlineColor/a"))}),
      textColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/textColor/r")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/textColor/g")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/textColor/b")),
                 static_cast<Uint8>(Gamedata::getInstance().getXmlInt("hudGoal/textColor/a"))}) {}

void HudGoal::draw(unsigned int balloonsTotal, unsigned int balloonsLeft, unsigned int livesLeft) {
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
    strm_bullet << "Shoot all " << balloonsTotal <<" balloons!";
    IoMod::getInstance().writeText(strm_bullet.str(), r.x + 20, r.y + boundary_y);
    strm_bullet.str("");
    strm_bullet << "Lives left: " << livesLeft;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 40);
    strm_bullet.str("");
    strm_bullet << "Balloons left: " << balloonsLeft;
    IoMod::getInstance().writeText(strm_bullet.str(), textColor, r.x + 20, r.y + 65);
  }
}


HudGameOver& HudGameOver::getInstance() {
  static HudGameOver instance;
  return instance;
}


HudGameOver::HudGameOver()
    : width(Gamedata::getInstance().getXmlInt("gameOver/width")),
      height(Gamedata::getInstance().getXmlInt("gameOver/height")),
      pos(Vector2f(Gamedata::getInstance().getXmlInt("gameOver/position/x"),
                   Gamedata::getInstance().getXmlInt("gameOver/position/y"))),
      visible(false),
      winBackgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/r")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/g")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/b")),
                          static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/backgroundColor/a"))}),
      winTextColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/r")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/g")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/b")),
                    static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/win/textColor/a"))}),
      loseBackgroundColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/r")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/g")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/b")),
                           static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/backgroundColor/a"))}),
      loseTextColor({static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/r")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/g")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/b")),
                     static_cast<Uint8>(Gamedata::getInstance().getXmlInt("gameOver/lose/textColor/a"))}) {}

void HudGameOver::draw(bool won) {
      if (visible) {

      SDL_Rect r;
      r.x = pos[0];
      r.y = pos[1];
      r.w = width;
      r.h = height;

      SDL_Rect inner;
      inner.x = pos[0] + 5;
      inner.y = pos[1] + 5;
      inner.w = width - 10;
      inner.h = height - 10;

      SDL_Renderer* renderer = IoMod::getInstance().getRenderer();
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      std::string textToWrite;

      if (won) {
      SDL_SetRenderDrawColor(renderer, winBackgroundColor.r, winBackgroundColor.g, winBackgroundColor.b,
                            winBackgroundColor.a);
      SDL_RenderFillRect(renderer, &inner);
      std::stringstream strm2;
      strm2.str("");
      strm2 << "YOU WIN!\nPress R to play again!";
      IoMod::getInstance().writeText(strm2.str(), winTextColor, r.x + boundary_x, r.y + (r.h/2));//4*boundary_y);
      } else {
      SDL_SetRenderDrawColor(renderer, loseBackgroundColor.r, loseBackgroundColor.g, loseBackgroundColor.b,
                            loseBackgroundColor.a);
      SDL_RenderFillRect(renderer, &inner);
      std::stringstream strm2;
      strm2.str("");
      strm2 << "YOU LOSE!\nPress R to play again!";
      IoMod::getInstance().writeText(strm2.str(), loseTextColor, r.x + boundary_x, r.y + (r.h/2));//4*boundary_y);
      }
}
}
