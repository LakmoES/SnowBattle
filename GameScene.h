#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "Ball.h"
#include "TGestureRecognizer.h"
#include <list>

class GameScene : public cocos2d::Layer, public TGestureHandler
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	void playerSpawn(float dt);
	void enemiesSpawn(float dt);

	void worldBorders(float dt);

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	std::list<Enemy*> *enemyList;
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
private:
    void SetPhysicsWorld( cocos2d::PhysicsWorld *world ) { sceneWorld = world; };
    
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	void crashBall(cocos2d::PhysicsBody *a, cocos2d::PhysicsBody *b);
	void shotCharacter(PhysicsBody *a, PhysicsBody *b);

	void playerThrow();
	void playerCanThrowAgain(float dt);

	void updateEnemies(float dt);
	void update(float dt);

    cocos2d::PhysicsWorld *sceneWorld;
    
	Ball ball;
	Player *player = NULL;
	unsigned int score;
	cocos2d::Label *scoreLabel;
	TGestureRecognizer* gestureRecognizer;
	//bool NodeContainsPoint(Node* node, const Vec2& pt);
public:
	// TGestureHandler's stuff
	virtual bool onGestureTap(TGestureTap* gesture);
	virtual bool onGestureLongPress(TGestureLongPress* gesture);
	virtual bool onGesturePan(TGesturePan* gesture);
	virtual bool onGesturePinch(TGesturePinch* gesture);
	virtual bool onGestureRotation(TGestureRotation* gesture);
	virtual bool onGestureSwipe(TGestureSwipe* gesture);

	// Touch handler (pass touches to the Gesture recognizer to process)
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);
	virtual void TouchCancelled(Touch* touch, Event* event);

private:
	void initGestureRecognizer();
};

#endif // __GAME_SCENE_H__
