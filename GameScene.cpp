#include "GameScene.h"
#include "Definitions.h"
//#include "GameOverScene.h"
//#include "SimpleAudioEngine.h"
#include <string.h>
#include <math.h>
#include "Utils.h"

USING_NS_CC;
Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics( );
    scene->getPhysicsWorld( )->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->SetPhysicsWorld( scene->getPhysicsWorld( ) );

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	// Init gesture recognizer
	initGestureRecognizer();
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
	enemyList = new std::list<Enemy*>();

    auto backgroundSprite = Sprite::create( BACKGROUND_SRC );
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    
    this->addChild( backgroundSprite );
	scheduleOnce(schedule_selector(GameScene::worldBorders), 1.0f); //костыль для бага, когда нет физ.границ экрана
    
    //this->schedule( schedule_selector( GameScene::SpawnPipe ), PIPE_SPAWN_FREQUENCY * visibleSize.width );
    
	scheduleOnce(schedule_selector(GameScene::playerSpawn), 1.0f); //костыль для спавна игрока
	scheduleOnce(schedule_selector(GameScene::enemiesSpawn), 1.0f); //костыль для спавна врага

	//schedule(schedule_selector(GameScene::updateEnemies), 1.0f/ENEMY_UPDATE_SPEED); //все боты двигаются одновременно

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	score = 0;
	__String *tempScore = __String::createWithFormat("%i", score);

	scoreLabel = Label::createWithTTF(tempScore->getCString(), FONT1, visibleSize.height * SCORE_FONT_SIZE);
	scoreLabel->setColor(Color3B::BLACK);
	scoreLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.75 + origin.y));

	this->addChild(scoreLabel, 10000);

	this->scheduleUpdate();

    return true;
}

void GameScene::worldBorders(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->setCollisionBitmask(OBSTACLE_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(true);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);
}

void GameScene::crashBall(PhysicsBody *a, PhysicsBody *b)
{
	auto nodeA = a->getNode(), nodeB = b->getNode();
	//remove the ball
	if (strcmp(nodeA->getName().c_str(), "ball") == 0)
		this->removeChild(nodeA, true);
	else if (strcmp(nodeB->getName().c_str(), "ball") == 0)
		this->removeChild(nodeB, true);

	CCLOG("BALL CRASHED");
}
void GameScene::shotCharacter(PhysicsBody *a, PhysicsBody *b)
{
	auto nodeA = a->getNode(), nodeB = b->getNode();
	if (nodeA == NULL)
	{
		CCLOG("A IS NULL, b:%s", nodeB->getName().c_str());
		return;
	}
	if (nodeB == NULL)
	{
		CCLOG("B IS NULL, a:%s", nodeA->getName().c_str());
		return;
	}
	//remove the ball
	if (strcmp(nodeA->getName().c_str(), "character") == 0)
		((Character*)nodeA)->takeDamage(1);
	else if (strcmp(nodeB->getName().c_str(), "character") == 0)
		((Character*)nodeB)->takeDamage(1);

	CCLOG("CHARACTER SHOT");
}
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if ((PLAYER_COLLISION_BITMASK == a->getCollisionBitmask() && BALL_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(PLAYER_COLLISION_BITMASK == b->getCollisionBitmask() && BALL_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		shotCharacter(a, b);
		crashBall(a, b);
		CCLOG("PLAYER SHOT");
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/Hit.mp3");
		//auto scene = GameOverScene::createScene(score);
		//Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}
	else if ((ENEMY_COLLISION_BITMASK == a->getCollisionBitmask() && BALL_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(ENEMY_COLLISION_BITMASK == b->getCollisionBitmask() && BALL_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("Sounds/Point.mp3");
		shotCharacter(a, b);
		crashBall(a, b);
		CCLOG("ENEMY SHOT");
		++score;

		__String *tempScore = __String::createWithFormat("%i", score);
		scoreLabel->setString(tempScore->getCString());
	}
	else if ((BALL_COLLISION_BITMASK == a->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(BALL_COLLISION_BITMASK == b->getCollisionBitmask() && OBSTACLE_COLLISION_BITMASK == a->getCollisionBitmask()))
	{
		crashBall(a, b);
	}
	return true;
}

/*
bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	initialTouchPos[0] = touch->getLocation().x;
	initialTouchPos[1] = touch->getLocation().y;
	currentTouchPos[0] = touch->getLocation().x;
	currentTouchPos[1] = touch->getLocation().y;

	isTouchDown = true;

	return true;
}
void GameScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	currentTouchPos[0] = touch->getLocation().x;
	currentTouchPos[1] = touch->getLocation().y;
}
void GameScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	isTouchDown = false;
}
void GameScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
	onTouchEnded(touch, event);
}
*/
void GameScene::updateEnemies(float dt)
{
	for (Enemy* enemy : *enemyList) 
	{
		enemy->update(dt);
	}
}

void GameScene::update(float dt)
{
	if (player == NULL)
		return;
}

void GameScene::playerSpawn(float dt)
{
	player = new Player(this);
}
void GameScene::enemiesSpawn(float dt)
{
	std::list<Sprite*> *neighbors = new std::list<Sprite*>();

	enemyList->push_back(new Enemy(this, Point(visibleSize.width / 5 + origin.x, visibleSize.height * 0.8 + origin.y), neighbors));
	enemyList->push_back(new Enemy(this, Point(visibleSize.width / 4 + origin.x, visibleSize.height * 0.8 + origin.y), neighbors));
	enemyList->push_back(new Enemy(this, Point(visibleSize.width / 3 + origin.x, visibleSize.height * 0.8 + origin.y), neighbors));
	enemyList->push_back(new Enemy(this, Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.9 + origin.y), neighbors));
	enemyList->push_back(new Enemy(this, Point(visibleSize.width / 1.5f + origin.x, visibleSize.height * 0.9 + origin.y), neighbors));

	for (Enemy* enemy : *enemyList)
		neighbors->push_back(enemy->getSprite());

	schedule(schedule_selector(GameScene::updateEnemies), 1.0f / ENEMY_UPDATE_SPEED);
}
void GameScene::playerThrow()
{
	if (player == NULL)
		return;
	player->fire(0, 0);
	player->canFire = false;
	scheduleOnce(schedule_selector(GameScene::playerCanThrowAgain), PLAYER_THROW_COOLDOWN); //таймер до следующего броска
}
void GameScene::playerCanThrowAgain(float dt)
{
	player->canFire = true;
}
//gestures
void GameScene::initGestureRecognizer()
{
	// Init gesture recognizer
	gestureRecognizer = new TSimpleGestureRecognizer();
	gestureRecognizer->init();
	gestureRecognizer->setGestureHandler(this);

	// Enable all gesture kinds
	gestureRecognizer->setTapEnabled(true);
	gestureRecognizer->setDoubleTapEnabled(true);
	gestureRecognizer->setLongPressEnabled(true);
	gestureRecognizer->setPanEnabled(true);
	gestureRecognizer->setPinchEnabled(true);
	gestureRecognizer->setRotationEnabled(true);
	gestureRecognizer->setSwipeEnabled(true);

	// Taps will be fired immediately without waiting for double tap
	gestureRecognizer->setTapRequiresDoubleTapRecognitionToFail(false);

	// Other config
	// gestureRecognizer->setTapThreshold(1.0f);
	// gestureRecognizer->setLongPressThreshold(1.0f);
	// gestureRecognizer->setDoubleTapInterval(0.3f);
	// gestureRecognizer->setPinchFingersDistanceChangeTolerance(0.1f);
	// gestureRecognizer->setRotationFingersDistanceChangeTolerance(0.5f);
	// gestureRecognizer->setSwipeThreshold(0.3f);

	//
	// IMPORTANT:
	// For multiple touch gestures on iOS (pinch, rotation), always remember tu put
	// the below line of code right after creating the CCEAGLView in AppController.mm
	// [eaglView setMultipleTouchEnabled:YES];
	// For Android, there no need to do this.
	//

	// Create touch listener and register it with cocos2d to receive touch events
	EventListenerTouchOneByOne* touchDelegate = EventListenerTouchOneByOne::create();
	touchDelegate->onTouchBegan = std::bind(&GameScene::TouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touchDelegate->onTouchMoved = std::bind(&GameScene::TouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touchDelegate->onTouchCancelled = std::bind(&GameScene::TouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
	touchDelegate->onTouchEnded = std::bind(&GameScene::TouchEnded, this, std::placeholders::_1, std::placeholders::_2);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(touchDelegate, 100);
}

bool GameScene::onGestureTap(TGestureTap* gesture)
{
	if (NodeContainsPoint(player->getSprite(), gesture->getLocation()))
	{
		if (gesture->getNumClicks() == 1)
			CCLOG("Tap on player");
		else
			CCLOG("Double Tap on player");
	}

	return false;
}

bool GameScene::onGestureLongPress(TGestureLongPress* gesture)
{
	if (NodeContainsPoint(player->getSprite(), gesture->getLocation()))
	{
		CCLOG("Long Tap on player");
	}

	return false;
}

bool GameScene::onGesturePan(TGesturePan* gesture)
{
	/*static int lastPanId = -1;
	static bool panInsideNode = false;

	// A new pan
	if (gesture->getID() != lastPanId)
	{
		lastPanId = gesture->getID();
		panInsideNode = NodeContainsPoint(_sprite, gesture->getLocation());
	}

	if (panInsideNode)
	{
		_sprite->setPosition(_sprite->getPosition() + gesture->getTranslation());
	}*/

	return false;
}

bool GameScene::onGesturePinch(TGesturePinch* gesture)
{
	/*static int lastPinchId = -1;
	static bool pinchInsideNode = false;
	static float originalScale;

	// A new pinch
	if (gesture->getID() != lastPinchId)
	{
		lastPinchId = gesture->getID();
		pinchInsideNode = NodeContainsPoint(_sprite, gesture->getLocation());
		originalScale = _sprite->getScale();
	}

	if (pinchInsideNode)
	{
		_sprite->setScale(originalScale * gesture->getScale());
	}*/

	return false;
}

bool GameScene::onGestureRotation(TGestureRotation* gesture)
{
	/*static int lastRotationId = -1;
	static bool rotationInsideNode = false;
	static float originalRotation;

	// A new rotation
	if (gesture->getID() != lastRotationId)
	{
		lastRotationId = gesture->getID();
		rotationInsideNode = NodeContainsPoint(_sprite, gesture->getLocation());
		originalRotation = _sprite->getRotation();
	}

	if (rotationInsideNode)
	{
		_sprite->setRotation(originalRotation + gesture->getRotation());
	}*/

	return false;
}

bool GameScene::onGestureSwipe(TGestureSwipe* gesture)
{
	SwipeDirection dir = gesture->getDirection();
	if (dir == SwipeDirectionNorth)
	{
		CCLOG("Swipe detected: NORTH");
		//ball.spawnBall(this, player->getPosition(), player->getContentSize().x, 1);
		playerThrow();
	}
	else if (dir == SwipeDirectionNorthEast)
	{
		CCLOG("Swipe detected: NORTH EAST");
		//ball.spawnBall(this, player->getPosition(), player->getContentSize().x, 1);
		playerThrow();
		player->go(1);
	}
	else if (dir == SwipeDirectionEast)
	{
		CCLOG("Swipe detected: EAST");
		player->go(1);
	}
	else if (dir == SwipeDirectionSouthEast)
	{
		CCLOG("Swipe detected: SOUTH EAST");
		player->go(1);
	}
	else if (dir == SwipeDirectionSouth)
	{
		CCLOG("Swipe detected: SOUTH");
	}
	else if (dir == SwipeDirectionSouthWest)
	{
		CCLOG("Swipe detected: SOUTH WEST");
		player->go(-1);
	}
	else if (dir == SwipeDirectionWest)
	{
		CCLOG("Swipe detected: WEST");
		player->go(-1);
	}
	else if (dir == SwipeDirectionNorthWest)
	{
		CCLOG("Swipe detected: NORTH WEST");
		//ball.spawnBall(this, player->getPosition(), player->getContentSize().x, 1);
		playerThrow();
		player->go(-1);
	}
	/*
	bool panEnabled = true; // If pan is not enabled, use false
	bool swipeFromNode = NodeContainsPoint(_sprite, panEnabled ? gesture->getEndLocation() : gesture->getLocation());

	if (swipeFromNode)
	{
		float swipeLength = gesture->getLength();
		float moveDistance = swipeLength * 2;

		// Move the sprite along the swipe direction
		Vec2 targetPos = _sprite->getPosition() + (gesture->getDirectionVec().getNormalized() * moveDistance);

		MoveTo* move = MoveTo::create(1.5f, targetPos);
		EaseOut* easeout = EaseOut::create(move, 5);

		_sprite->runAction(easeout);
	}*/

	return false;
}



bool GameScene::TouchBegan(Touch* touch, Event* event)
{
	// Let the gesture recognizer to do its work
	gestureRecognizer->onTouchBegan(touch, event);
	return true;
}
void GameScene::TouchMoved(Touch* touch, Event* event)
{
	// Let the gesture recognizer to do its work
	gestureRecognizer->onTouchMoved(touch, event);
}
void GameScene::TouchEnded(Touch* touch, Event* event)
{
	// Let the gesture recognizer to do its work
	gestureRecognizer->onTouchEnded(touch, event);
}
void GameScene::TouchCancelled(Touch* touch, Event* event)
{
	// Let the gesture recognizer to do its work
	gestureRecognizer->onTouchCancelled(touch, event);
}
