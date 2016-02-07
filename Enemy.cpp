#include "Enemy.h"
#include "Definitions.h"
#include <cmath>
//#include "SimpleAudioEngine.h"

USING_NS_CC;

Enemy::Enemy(cocos2d::Layer *layer, cocos2d::Point position, std::list<cocos2d::Sprite*> *neighbors) : Character(layer, IMG_ENEMY, ENEMY_COLLISION_BITMASK, "character")
{
	characterSprite->setPosition(position);
	this->neighbors = neighbors;

	stepSize = (characterSprite->getContentSize().width * characterSprite->getScale()) / 2;
	walkAbility[0] = false;
	walkAbility[1] = false;
}

void Enemy::updateWalkAbility()
{
	walkAbility[0] = true;
	walkAbility[1] = true;

	for (Sprite* neighbor : *neighbors)
	{
		//right
		if ((neighbor->getPosition().x - characterSprite->getPosition().x) > 0
			&& (neighbor->getPosition().x - characterSprite->getPosition().x) < 
			   ((stepSize * 2) + ( ( (characterSprite->getContentSize().width*characterSprite->getScale()) + (neighbor->getContentSize().width*neighbor->getScale())) / 2))
		   )
			  walkAbility[1] = false;
		//left
		if ((characterSprite->getPosition().x - neighbor->getPosition().x) > 0
			&& (characterSprite->getPosition().x - neighbor->getPosition().x) <
			   ((stepSize * 2) + (((characterSprite->getContentSize().width*characterSprite->getScale()) + (neighbor->getContentSize().width*neighbor->getScale())) / 2))
			)
			  walkAbility[0] = false;
	}
}

bool Enemy::go(int way)
{
	Character::go(way, stepSize);
	return true;
}
void Enemy::fire(int type, int power)
{
	Character::fire(type, power, -1);
}
void Enemy::update(float dt)
{
	updateWalkAbility();
	if (CCRANDOM_0_1() <= 0.3f)
		this->fire(0, 0);
	else 
		if (walkAbility[0] == true && walkAbility[1] == true)
			if (CCRANDOM_0_1() < 0.5f)
				this->go(-1);
			else
				this->go(1);
	else
	{
		if (walkAbility[0] == true)
			this->go(-1);
		else if (walkAbility[1] == true)
			this->go(1);
	}

}