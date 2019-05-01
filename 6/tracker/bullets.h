#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "vector2f.h"
#include "drawable.h"
#include "bullet.h"

class CollisionStrategy;
class Bullets {
public:
  Bullets(const std::string&);
  Bullets(const Bullets&);
  Bullets& operator=(const Bullets&) = default;
  ~Bullets();
  void draw() const;
  void update(int ticks);
  void shoot(const Vector2f& pos, const Vector2f& objVel);
  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount() const { return freeList.size(); }

  bool shooting() const { return !bulletList.empty(); }
  bool collided(const Drawable*);
private:
  std::string name;
  Vector2f myVelocity;
  unsigned int numOfBullets;
  std::list<Bullet> bulletList;
  std::list<Bullet> freeList;
  CollisionStrategy* strategy;
};
#endif
