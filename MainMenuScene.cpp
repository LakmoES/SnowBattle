#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions.h"
#include "Localized.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create(BACKGROUND_SRC);
    backgroundSprite->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
    this->addChild( backgroundSprite );

	titleLabel = Label::createWithTTF(TITLE_NAME, FONT1, visibleSize.height * SCORE_FONT_SIZE);
	titleLabel->setColor(Color3B::BLACK);
	titleLabel->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.90 + origin.y));
	this->addChild(titleLabel, 10000);

	//ѕолучаем указатель на €зыковый модуль
	auto pLocalized = Localized::getInstance();

	auto startLabel = Label::createWithTTF(pLocalized->getString("start").c_str(), FONT2, visibleSize.height * MENU_FONT_SIZE);
	auto startGameItem = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainMenuScene::startGame, this));
    
	auto menu = Menu::create(startGameItem, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2.0 + origin.x, visibleSize.height / 2.0 + origin.y));
	menu->setColor(Color3B::GRAY);
	menu->alignItemsVerticallyWithPadding(1.0f);
	this->addChild(menu, 3);
    
	/*auto playItem = MenuItemImage::create( "play_button.png", "play_button_clicked.png", CC_CALLBACK_1( MainMenuScene::GoToGameScene, this ) );
	playItem->setPosition( Point( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );

	auto menu = Menu::create( playItem, NULL );
	menu->setPosition( Point::ZERO );

	this->addChild( menu );*/
    
    return true;
}
void MainMenuScene::startGame(Ref* pSender)
{
	CCLOG("Start button tapped");
	auto scene = GameScene::createScene();
	    
	Director::getInstance( )->replaceScene( TransitionFade::create( TRANSITION_TIME, scene ) );
}
