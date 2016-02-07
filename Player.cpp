#include "Player.h"
#include "Definitions.h"
//#include "SimpleAudioEngine.h"

USING_NS_CC;

Player::Player(cocos2d::Layer *layer) :Character(layer, IMG_PLAYER, PLAYER_COLLISION_BITMASK, "character")
{
	characterSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.1 + origin.y));
}

void Player::go(int way)
{
	Character::go(way, characterSprite->getContentSize().width);
}
void Player::fire(int type, int power)
{
	Character::fire(type, power, 1);
}