#include "subjectSprite.h"
#include "smartSprite.h"
#include "gameData.h"
#include "imageFactory.h"
#include "renderContext.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

void SubjectSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}


SubjectSprite::SubjectSprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  imagesLeft( ImageFactory::getInstance().getImages(name+"Left") ),
  imagesRight( images ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),

  collisionStrategy(new PerPixelCollisionStrategy),
  explosion( nullptr),

  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0),
  observers()
{ }

void SubjectSprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
  bullets.draw();
}

void SubjectSprite::shoot() {
    if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x;
    float y = getY()+getScaledHeight()/4+11;
    if ( vel[0] > 0 ) {
      x = getX()+getScaledWidth()-10;
      vel[0] += bulletSpeed;
    }
    else {
      x = getX();
      vel[0] -= bulletSpeed;
    }
    bullets.shoot( Vector2f(x, y), vel );
    timeSinceLastBullet = 0;
  }

}


bool SubjectSprite::shot(const Drawable* obj ) {
  if ( bullets.collided(obj) ) return true;
  else return false;
}

bool SubjectSprite::collidedWith(const Drawable* obj ) {
  if ( explosion ) return false;
  if ( collisionStrategy->execute(*this, *obj) ) {
    return true;
  }
  return false;
}

void SubjectSprite::explode() {
  if ( !explosion ) {
    Sprite sprite(getName(), getPosition(), getVelocity(), getImage());
    // sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}


void SubjectSprite::stop() {
  setVelocityX( 0.93*getVelocityX() );
  setVelocityY(0);
}

void SubjectSprite::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = imagesRight;
  }
}
void SubjectSprite::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = imagesLeft;
  }
}
void SubjectSprite::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
}
void SubjectSprite::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void SubjectSprite::detach( SmartSprite* o ) {
  o->explode();
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void SubjectSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
  bullets.update(ticks);

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

  timeSinceLastBullet += ticks;
  stop();
}
