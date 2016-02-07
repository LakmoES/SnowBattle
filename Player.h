#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Character.h"

class Player : public Character
{
public:
	Player(cocos2d::Layer *layer);
	void go(int way);
	void fire(int type, int power);
};

#endif // __PLAYER_H__
