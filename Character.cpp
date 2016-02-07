#include "Character.h"
#include "Definitions.h"

USING_NS_CC;

Character::Character(cocos2d::Layer *layer, const char* img, int bitmask, const char* bodyName)
{
	this->parentalLayer = layer;
	characterSprite = Sprite::create(img);

	characterBody = PhysicsBody::createBox(Size(characterSprite->getContentSize().width *0.95f, characterSprite->getContentSize().height *0.9f));
	characterBody->setCollisionBitmask(bitmask);
	characterBody->setContactTestBitmask(true);

	characterSprite->setPhysicsBody(characterBody);
	characterSprite->setName(bodyName);
	characterSprite->setScale(1.5f);

	layer->addChild(characterSprite, 100);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	fireCount = 0;
	health = 0;
	isAlive = true;
	canGo = true;
	canFire = true;
}

void Character::go(int way, int n)
{
	switch (way)
	{
	case 1:
		//проверки на выход за границы экрана
		if ((visibleSize.width - characterSprite->getPositionX()) >= characterSprite->getContentSize().width)
			characterSprite->setPositionX(characterSprite->getPositionX() + n);
		break;
	case -1:
		if (characterSprite->getPositionX() >= characterSprite->getContentSize().width)
			characterSprite->setPositionX(characterSprite->getPositionX() - n);
		break;
	default:
		break;
	}
}
void Character::fire(int type, int power, int direction)
{
	if (canFire)
		ball.spawnBall(parentalLayer, characterSprite->getPosition(), characterSprite->getContentSize().height * characterSprite->getScale(), direction);
}
void Character::fireReduce()
{
	--this->fireCount;
}
void Character::fireIncrease()
{
	++this->fireCount;
}
int Character::getFireCount()
{
	return this->fireCount;
}
void Character::kill()
{
	this->isAlive = false;
}
//вылетает ошибка EPS #0 ...
void Character::takeDamage(int n)
{
	/*this->health -= n;
	if (health <= 0)
		this->kill();*/
}
Point Character::getPosition()
{
	return characterSprite->getPosition();
}
Vec2 Character::getContentSize()
{
	return characterSprite->getContentSize();
}
Sprite* Character::getSprite()
{
	return characterSprite;
}