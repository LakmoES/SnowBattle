#include "Ball.h"
#include "Definitions.h"

USING_NS_CC;

Ball::Ball()
{
    visibleSize = Director::getInstance( )->getVisibleSize( );
    origin = Director::getInstance( )->getVisibleOrigin( );
}

void Ball::spawnBall(cocos2d::Layer *layer, Point throwerPosition, int throwerGrowth,/* int type, float speed,*/ int way)
{
	//Point throwerPosition = thrower->getPosition();
    CCLOG( "SPAWN BALL" );
    
    auto ball = Sprite::create( IMG_BALL );
    
    auto ballPosition = throwerPosition;
	ballPosition.y += way*throwerGrowth;

	ball->setPosition(ballPosition);
	ball->setScale(1.3f);
    
	auto ballBody = PhysicsBody::createCircle(ball->getContentSize().width/2);
	ballBody->setCollisionBitmask(BALL_COLLISION_BITMASK);
	ballBody->setContactTestBitmask(true);
	ball->setPhysicsBody(ballBody);
	ball->setName("ball");
    
	layer->addChild(ball, 100);
    
    auto ballAction = MoveBy::create( (1.0f/BALL_MOVEMENT_SPEED) * visibleSize.height * ball->getScale(), Point( 0, way*visibleSize.height * 1.5 ) );
    
	ball->runAction(ballAction);
}







