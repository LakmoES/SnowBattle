#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

class Ball
{
public:
    Ball();
    
	//доделать метод спавна шарика
	void spawnBall(cocos2d::Layer *layer, cocos2d::Point throwerPosition, int throwerGrowth,/* int type, float speed,*/ int way);
    
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
};

#endif // __BALL_H__
