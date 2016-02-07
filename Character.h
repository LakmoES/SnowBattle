#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"
#include "Ball.h"

class Character
{
public:
	Character(cocos2d::Layer *layer, const char* img, int bitmask, const char* bodyName);
	cocos2d::Point getPosition();
	cocos2d::Vec2 getContentSize();
	cocos2d::Sprite* getSprite();
	bool canFire;

	virtual void go(int way, int n);
	virtual void fire(int type, int power, int direction);
	void fireReduce();
	void fireIncrease();
	int getFireCount();
	virtual void kill();
	void takeDamage(int n);
protected: 
	int fireCount;
	int health;
	cocos2d::Sprite *characterSprite;
	cocos2d::PhysicsBody *characterBody;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Layer* parentalLayer;
private:
	Ball ball;
	
	bool isAlive;
	bool canGo;
};

#endif // __CHARACTER_H__
