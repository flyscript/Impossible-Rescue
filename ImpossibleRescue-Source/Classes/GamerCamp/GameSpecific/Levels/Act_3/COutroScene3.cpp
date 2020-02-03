// Includes
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"
#include "GamerCamp/GameSpecific/Levels/Act_3/CCreditScene.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

// Include own header
#include "COutroScene3.h"
#include "AppDelegate.h"

USING_NS_CC;

static COutroScene3::EInput		s_aePlayerActions[] = { COutroScene3::EInput::EI_Skip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_A };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
COutroScene3::COutroScene3()
	: IGCGameLayer( GetGCTypeIDOf( COutroScene3 ) )
	, m_bTextSkipped( false )
	, m_fPressToContinueAnimationTime( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

COutroScene3::~COutroScene3()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void COutroScene3::onEnter()
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

void COutroScene3::VOnCreate()
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

	m_pcAnimatedText = UIAnimatedText::create( "SHADOW and I were able to advance deeper into the facility to find where Ellie was being held. A security alarm was activated but we were able to escape the lockdown before the time ran out. It was clear that they knew of our presence in the facility, so we had to move on quickly because it was our mission to rescue her!", "fonts/deadman.ttf", 32, EUILabelIntroAnimationType::EUIAnimType_Typing, GameConfig::k_fCutSceneTextSpeed );
	m_pcAnimatedText->SetTextAreaSize( Size( 800, 500 ) );
	m_pcAnimatedText->SetPosition( Vec2( visibleSize.width / 2 + 400.0f, visibleSize.height *0.3 ) + origin );
	m_pcAnimatedText->EnableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	m_pcAnimatedText->SetTextColor( Color4B::WHITE );
	this->addChild( m_pcAnimatedText, 1 );



	m_pcSkipLabel = Label::createWithTTF( "Press X to Skip", "fonts/space_age.ttf", 24 );
	m_pcSkipLabel->setOpacityModifyRGB( true );
	m_pcSkipLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height* 0.1 ) );
	m_pcSkipLabel->enableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	this->addChild( m_pcSkipLabel, 1 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_Geiger, true );

}

void COutroScene3::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;
	delete m_pcBackground;
	m_pcBackground = NULL;
	IGCGameLayer::VOnDestroy();
}

void COutroScene3::VOnUpdate( f32 fTimeStep )
{
	if( m_bTextSkipped || m_pcAnimatedText->HasAnimationEnded() )
	{
		m_pcSkipLabel->setString( "Press X to Continue" );
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_Geiger );
	}
	TGCController< COutroScene3::EInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	if( cController.IsActive() )
	{
		ControllerUpdate( cController );
	}
	else
	{
		KeyboardUpdate();
	}

	// Polish fades label in and out with a sine wave with a y intercept of 1 that then has its magnitude halved - Will
	m_fPressToContinueAnimationTime += fTimeStep * 3;
	m_pcSkipLabel->setOpacity( GLubyte( int( ( ( std::sin( m_fPressToContinueAnimationTime ) + 1 ) / 2 ) * 255 ) ) );

	IGCGameLayer::VOnUpdate( fTimeStep );
}

//////////////////////////////////////////////////////////////////////////
// Control Update Methods
//////////////////////////////////////////////////////////////////////////
void COutroScene3::ControllerUpdate( TGCController<COutroScene3::EInput> cController )
{
	if( !m_pcAnimatedText->HasAnimationEnded() )
	{
		if( m_bTextSkipped )
		{
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				ReplaceScene( TGCGameLayerSceneCreator< CCreditScene >::CreateScene() );
			}
		}
		else
		{
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				m_pcAnimatedText->SkipAnimation();
				m_bTextSkipped = true;
			}
		}
	}
	else
	{
		if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
		{
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			ReplaceScene( TGCGameLayerSceneCreator< CCreditScene >::CreateScene() );
		}
	}
}

void COutroScene3::KeyboardUpdate()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();

	if( !m_pcAnimatedText->HasAnimationEnded() )
	{
		if( m_bTextSkipped )
		{
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				ReplaceScene( TGCGameLayerSceneCreator< CCreditScene >::CreateScene() );
			}
		}
		else
		{
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				m_pcAnimatedText->SkipAnimation();
				m_bTextSkipped = true;
			}
		}
	}
	else
	{
		if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
		{
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			ReplaceScene( TGCGameLayerSceneCreator< CCreditScene >::CreateScene() );
		}
	}

}
