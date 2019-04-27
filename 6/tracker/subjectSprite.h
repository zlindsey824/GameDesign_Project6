#ifndef SUBJECTSPRITE__H
#define SUBJECTSPRITE__H
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "drawable.h"
#include "bullets.h"

class SmartSprite;
class CollisionStrategy;
class ExplodingSprite;

class SubjectSprite : public Drawable {
public:

  SubjectSprite(const std::string&);
  SubjectSprite(const SubjectSprite&) = default;
  SubjectSprite& operator=(const SubjectSprite&) = default;


  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual void shoot();
  unsigned int bulletCount() const { return bullets.bulletCount(); }
  unsigned int freeCount() const { return bullets.freeCount(); }
  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );
  virtual bool shot(const Drawable*);
  virtual void explode();
  virtual bool explosionDone() const;
  virtual bool collidedWith(const Drawable*);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

  void right();
  void left();
  void up();
  void down();
  void stop();

private:
  std::vector<Image *> images;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> imagesRight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;

  CollisionStrategy* collisionStrategy;
  ExplodingSprite* explosion;

  std::string bulletName;
  Bullets bullets;
  float bulletSpeed;
  int bulletInterval;
  int timeSinceLastBullet;

  void advanceFrame(Uint32 ticks);

protected:
  std::list<SmartSprite*> observers;
};
#endif
