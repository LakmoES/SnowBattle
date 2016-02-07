#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Character.h"
#include <list>

class Enemy: public Character
{
private:
	std::list<cocos2d::Sprite*> *neighbors;
	int stepSize;
	bool walkAbility[2];
	void updateWalkAbility();
	bool go(int way);
public:
	Enemy(cocos2d::Layer *layer, cocos2d::Point position, std::list<cocos2d::Sprite*> *neighbors);
	void fire(int type, int power);
	void update(float dt);
};

#endif // __ENEMY_H__
