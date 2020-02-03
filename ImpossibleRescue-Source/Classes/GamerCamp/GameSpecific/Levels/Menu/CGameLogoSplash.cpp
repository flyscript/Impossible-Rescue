// Includes
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

// Include own header
#include "CGameLogoSplash.h"
#include "AppDelegate.h"
#include "CMainMenu.h"

USING_NS_CC;


static CGameLogo::EInput		s_aePlayerActions[] = { CGameLogo::EInput::EI_Skip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_A };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CGameLogo::CGameLogo()
	: IGCGameLayer( GetGCTypeIDOf( CGameLogo ) )
	, m_fTime( 0.0f )
{
	m_pcAudioManager = new CAudioManager();
}

CGameLogo::~CGameLogo()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CGameLogo::onEnter()
{
	IGCGameLayer::onEnter();
	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_ENTER,
	};
	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CGameLogo::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// Creating the background sprite
	cocos2d::Sprite* pSprite = Sprite::create( "Loose/HelloWorld.png" );
	// position the sprite on the center of the screen
	pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
	// add the sprite as a child to this layer
	this->addChild( pSprite, 0 );

	//Creating the press x to skip text
	m_pcSkipLabel = Label::createWithTTF( "Press X to Continue", "fonts/space_age.ttf", 32 );
	m_pcSkipLabel->setOpacityModifyRGB( true );
	m_pcSkipLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
	m_pcSkipLabel->enableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	this->addChild( m_pcSkipLabel, 1 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );
}

void CGameLogo::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

void CGameLogo::VOnUpdate( f32 fTimeStep )
{
	TGCController< CGameLogo::EInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	if( cController.IsActive() )
	{
		ControllerUpdate( cController );
	}
	else
	{
		KeyboardUpdate();
	}

	// Polish fades label in and out with a sine wave with a y intercept of 1 that then has its magnitude halved - Will
	m_fTime += fTimeStep * 3;
	m_pcSkipLabel->setOpacity( GLubyte( int( ( ( std::sin( m_fTime ) + 1 ) / 2 ) * 255 ) ) );

	IGCGameLayer::VOnUpdate( fTimeStep );
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CGameLogo::ControllerUpdate( TGCController<CGameLogo::EInput> cController )
{
	//Handle the action when the Enter key has been pressed 
	if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
	{
		//Play the menu Ok button
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		//Change to the Main menu screen 
		ReplaceScene( TGCGameLayerSceneCreator< CMainMenu >::CreateScene() );
	}
}

void CGameLogo::KeyboardUpdate()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	//Handle the action when the X button has been pressed 
	if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
	{
		//Play the menu Ok button
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		//Change to the Main menu screen 
		ReplaceScene( TGCGameLayerSceneCreator< CMainMenu >::CreateScene() );
	}
}