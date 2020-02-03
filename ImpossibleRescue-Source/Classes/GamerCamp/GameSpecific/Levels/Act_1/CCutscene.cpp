// Includes
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel1.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

// Include own header
#include "CCutscene.h"
#include "CLevel1.h"
#include "AppDelegate.h"

USING_NS_CC;


static CCutscene1::EInput		s_aePlayerActions[] = { CCutscene1::EInput::EI_Skip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_A };

CCutscene1::CCutscene1()
	: IGCGameLayer( GetGCTypeIDOf( CCutscene1 ) )
	, m_bTextSkipped( false )
	, m_fPressToContinueAnimationTime( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CCutscene1::~CCutscene1()
{
}

void CCutscene1::onEnter()
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

void CCutscene1::VOnCreate()
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

	m_pAnimatedText = UIAnimatedText::create( "I was tasked to retrieve the president of earth\'s daughter Ellie from the Imperial Empire of Mars. I landed near the auxiliary entrance of section A. For a hidden Martian security installation I was surprised to find their ventilation system poorly maintained, therefore gained access to facility through the unguarded vent.\n\nWe expected to encounter some resistance inside, needing to remain undetected I utilised SHADOW\'s disabling weaponry. We confronted some lower tier automated robotic defenses making our way through the first level of the base. We had to move quickly to find Ellie due to the enemy issuing a \"surrender or suffer\" demand to be met in 60 minutes.", "fonts/deadman.ttf", 24, EUILabelIntroAnimationType::EUIAnimType_Typing, GameConfig::k_fCutSceneTextSpeed );
	m_pAnimatedText->SetTextAreaSize( Size( 800, 500 ) );
	m_pAnimatedText->EnableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	m_pAnimatedText->SetPosition( Vec2( visibleSize.width / 2 + 400.0f, visibleSize.height *0.3 ) + origin );
	m_pAnimatedText->SetTextColor( Color4B::WHITE );
	this->addChild( m_pAnimatedText, 1 );

	m_pcSkipLabel = Label::createWithTTF( "Press X to Skip", "fonts/space_age.ttf", 24 );
	m_pcSkipLabel->setOpacityModifyRGB( true );
	m_pcSkipLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height* 0.1 ) );
	m_pcSkipLabel->enableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	this->addChild( m_pcSkipLabel, 1 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_Geiger, true );

}

void CCutscene1::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;
	delete m_pcBackground;
	m_pcBackground = NULL;
	IGCGameLayer::VOnDestroy();
}

void CCutscene1::VOnUpdate( f32 fTimeStep )
{
	if( m_bTextSkipped || m_pAnimatedText->HasAnimationEnded() )
	{
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_Geiger );
		m_pcSkipLabel->setString( "Press X to Continue" );
	}
	TGCController< CCutscene1::EInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
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

void CCutscene1::ControllerUpdate( TGCController<CCutscene1::EInput> cController )
{
	if( !m_pAnimatedText->HasAnimationEnded() )
	{
		if( m_bTextSkipped )
		{
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
			}
		}
		else
		{
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				m_pAnimatedText->SkipAnimation();
				m_bTextSkipped = true;
			}
		}
	}
	else
	{
		if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
		{
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
		}
	}
}

void CCutscene1::KeyboardUpdate()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();

	if( !m_pAnimatedText->HasAnimationEnded() )
	{
		if( m_bTextSkipped )
		{
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
			}
		}
		else
		{
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				m_pAnimatedText->SkipAnimation();
				m_bTextSkipped = true;
			}
		}
	}
	else
	{
		if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
		{
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
		}
	}

}
