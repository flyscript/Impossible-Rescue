// Includes
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel1.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

// Include own header

#include "MenuScene.h"
#include "CCutsceneAct2.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CCutsceneAct2::CCutsceneAct2()
	: IGCGameLayer( GetGCTypeIDOf( CCutsceneAct2 ) )
{
	m_pcAucioManager = new CAudioManager();
}

CCutsceneAct2::~CCutsceneAct2()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CCutsceneAct2::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// add "CGCGameLayerPlatformer" splash screen"
	const char* pszPlist_background = "TexturePacker/Backgrounds/CutScene/Act1CutScene.plist";
	{
		m_pcBackground = new CGCObjSprite();
		m_pcBackground->CreateSprite( pszPlist_background );
		m_pcBackground->SetResetPosition( cocos2d::Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
		m_pcBackground->SetParent( IGCGameLayer::ActiveInstance() );
	}

	MenuItemImage* pContinue = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Start/Menu_Start_Normal.png",
		"Loose/UI/Buttons/Menu_Start/Menu_Start_Selected.png",
		CC_CALLBACK_1( CCutsceneAct2::Callback_ContinueButton, this ) );

	pContinue->setPosition( cocos2d::Vec2( origin.x + ( visibleSize.width * 0.5f ), origin.y + ( visibleSize.height * 0.2f ) ) );

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pContinue, NULL );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 1 );

	m_pcAnimatedText = UIAnimatedText::create( "Placeholder for act2", "fonts/deadman.ttf", 32, EUILabelIntroAnimationType::EUIAnimType_None );
	m_pcAnimatedText->SetTextAreaSize( Size( 800, 500 ) );
	m_pcAnimatedText->EnableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	m_pcAnimatedText->SetPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
	m_pcAnimatedText->SetTextColor( Color4B::WHITE );
	this->addChild( m_pcAnimatedText, 1 );


}

void CCutsceneAct2::VOnDestroy()
{
	delete m_pcBackground;
	m_pcBackground = NULL;
	IGCGameLayer::VOnDestroy();
}

void CCutsceneAct2::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );
}

//////////////////////////////////////////////////////////////////////////
// UI Callbacks
//////////////////////////////////////////////////////////////////////////
void CCutsceneAct2::Callback_ContinueButton( Ref* pSender )
{
	Director::getInstance()->replaceScene( CMenuLayer::scene() );
}
