// Dependency Include
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

#include "CCreditScene.h"
#include "AppDelegate.h"
#include "GamerCamp/GameSpecific/Levels/Menu/CMainMenu.h"


static CCreditScene::EInput		s_aePlayerActions[] = { CCreditScene::EInput::EI_Skip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_A };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CCreditScene::CCreditScene()
	: IGCGameLayer( GetGCTypeIDOf( CCreditScene ) )
	, m_pcSkipLabel( nullptr )
	, m_pCreditContainerScrollView( nullptr )
	, m_fTimeLeftToMenu( 10.0f )
	, m_fCurrentTime( 0.0f )
	, m_bIsTimerEnded( false )
	, m_fPressToContinueAnimationTime( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CCreditScene::~CCreditScene()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CCreditScene::onEnter()
{
	IGCGameLayer::onEnter();

	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_ENTER,
	};
	u32 uSizeOfActionArray = (sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ));

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray , aeKeyCodesForActions );
	

}

void CCreditScene::VOnCreate()
{
	IGCGameLayer::VOnCreate();
	
	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* pBackgroundSprite = Sprite::create( "Loose/Backgrounds/Splash/Credits.png" );

	// position the sprite on the center of the screen
	pBackgroundSprite->setPosition( Vec2( origin.x + (visibleSize.width * 0.5f) , origin.y + (visibleSize.height* 0.5f) ) );

	// add the sprite as a child to this layer
	this->addChild( pBackgroundSprite , 0 );

	// Init Label
	m_pcSkipLabel = Label::createWithTTF( "Press X to Skip" , "fonts/space_age.ttf" , 24 );
	m_pcSkipLabel->setOpacityModifyRGB( true );
	m_pcSkipLabel->setPosition( Vec2( visibleSize.width / 2 , visibleSize.height* 0.1 - 50.0f) );
	m_pcSkipLabel->enableShadow( cocos2d::Color4B::BLACK , cocos2d::Size( -2 , -2 ) , 5.f );
	m_pcSkipLabel->setVisible( false );
	this->addChild( m_pcSkipLabel , 1 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions , s_aeKeys );

	
}

void CCreditScene::VOnReset()
{
	IGCGameLayer::VOnReset();
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Credits , true );
}

void CCreditScene::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	m_pcAudioManager->StopBackgroundMusic();
	
	IGCGameLayer::VOnDestroy();
}

void CCreditScene::VOnUpdate( f32 fTimeStep )
{
	if( !m_bIsTimerEnded && m_fCurrentTime >= m_fTimeLeftToMenu )
	{
		m_bIsTimerEnded = true;
	}
	else
	{
		m_fCurrentTime += fTimeStep;
	}

	if( m_bIsTimerEnded )
	{
		m_pcSkipLabel->setVisible( true );
		
		TGCController< CCreditScene::EInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne , (*m_pcControllerActionToKeyMap) );
		if ( cController.IsActive() )
		{
			ControllerUpdate( cController );
		}
		else
		{
			KeyboardUpdate();
		}
	}

	// Polish fades label in and out with a sine wave with a y intercept of 1 that then has its magnitude halved - Will
	m_fPressToContinueAnimationTime += fTimeStep * 3;
	m_pcSkipLabel->setOpacity( GLubyte( int( ((std::sin( m_fPressToContinueAnimationTime ) + 1) / 2) * 255 ) ) );

	IGCGameLayer::VOnUpdate( fTimeStep );
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CCreditScene::ControllerUpdate(TGCController<CCreditScene::EInput> cController)
{
	if ( cController.ButtonHasJustBeenPressed( EI_Skip ) )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK , false );
		BackToMenu();
	}
}

void CCreditScene::KeyboardUpdate()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();
	
	if ( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK , false );
		BackToMenu();
	}
}

//////////////////////////////////////////////////////////////////////////
// Callback to menu
//////////////////////////////////////////////////////////////////////////
void CCreditScene::BackToMenu()
{
	ReplaceScene( TGCGameLayerSceneCreator< CMainMenu >::CreateScene() );
}
