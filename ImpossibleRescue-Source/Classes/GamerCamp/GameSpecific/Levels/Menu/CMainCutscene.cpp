// Includes
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

// Include own header
#include "CMainCutscene.h"
#include "AppDelegate.h"
#include "CGameLogoSplash.h"

USING_NS_CC;

static CCutsceneMain::EInput		s_aePlayerActions[] = { CCutsceneMain::EInput::EI_Skip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_A };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CCutsceneMain::CCutsceneMain()
	: IGCGameLayer( GetGCTypeIDOf( CCutsceneMain ) )
	, m_bTextSkipped( false )
	, m_fTime( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CCutsceneMain::~CCutsceneMain()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CCutsceneMain::onEnter()
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

void CCutsceneMain::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Creating the texet to be displayed in the cutscene 
	m_pcAnimatedText = UIAnimatedText::create( 
		"Year: 2199\n\nAssigned Operative : Freelance mercenary \'Danielle\' , with her Remote Pilot - droid \'SHADOW\'\n( Specialist Hard - linked Android for Duty in Offshore Wars )\n\nLocation : Hidden Martian security installation belonging to the Imperial Empire of Mars\n\nMission : Rescue the President of Earth\'s daughter.",
		"fonts/deadman.ttf", 32, EUILabelIntroAnimationType::EUIAnimType_Typing, GameConfig::k_fIntroTextSpeed );
	m_pcAnimatedText->SetTextAreaSize( Size( visibleSize.width*0.8, visibleSize.height*0.8 ) );
	m_pcAnimatedText->EnableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	m_pcAnimatedText->SetPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2 + 20.0f ) + origin );
	m_pcAnimatedText->SetTextColor( Color4B::WHITE );
	this->addChild( m_pcAnimatedText, 1 );

	//Creating the background sprite
	cocos2d::Sprite* pSprite = Sprite::create( "Loose/Backgrounds/Splash/Splash_Plain.png" );
	// position the sprite on the center of the screen
	pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
	// add the sprite as a child to this layer
	this->addChild( pSprite, 0 );

	//Creating the press x to skip text
	m_pcSkipLabel = Label::createWithTTF( "Press X to Skip", "fonts/space_age.ttf", 24 );
	m_pcSkipLabel->setOpacityModifyRGB( true );
	m_pcSkipLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height* 0.1 ) );
	m_pcSkipLabel->enableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	this->addChild( m_pcSkipLabel, 1 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_Geiger, true );
}

void CCutsceneMain::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

void CCutsceneMain::VOnUpdate( f32 fTimeStep )
{
	//Change the text from press x to skip, if x has been pressed or all the cutscene text has been loaded
	if( m_bTextSkipped || m_pcAnimatedText->HasAnimationEnded() )
	{
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_Geiger );
		m_pcSkipLabel->setString( "Press X to Continue" );
	}
	TGCController< CCutsceneMain::EInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
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
void CCutsceneMain::ControllerUpdate( TGCController<CCutsceneMain::EInput> cController )
{
	//Checks if the ticker text animation has not yet ended 
	if( !m_pcAnimatedText->HasAnimationEnded() )
	{
		//Checks if the text skipped flag has been set 
		if( m_bTextSkipped )
		{
			//Handles the actions when the X button is pressed 
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				//play the Menu ok sound effect 
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				//Change the screen to the game logo screen 
				ReplaceScene( TGCGameLayerSceneCreator< CGameLogo >::CreateScene() );
			}
		}
		//evaluates when the text skipped flag has not been set 
		else
		{
			//Handle the action when the X button is pressed 
			if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
			{
				//Play the Menu ok sound effect 
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				//Skip the ticker text animation 
				m_pcAnimatedText->SkipAnimation();
				//Set the Text skipped flag 
				m_bTextSkipped = true;
			}
		}
	}
	//evaluates when the ticker text animations have ended 
	else
	{
		//handle the action when the X button is pressed 
		if( cController.ButtonHasJustBeenPressed( EI_Skip ) )
		{
			//play the menu ok sound effect 
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			//Change the screen to the Game log screen 
			ReplaceScene( TGCGameLayerSceneCreator< CGameLogo >::CreateScene() );
		}
	}
}

void CCutsceneMain::KeyboardUpdate()
{
	const CGCKeyboardManager* pKeyManager = AppDelegate::GetKeyboardManager();

	//Checks if the ticker text animation has not yet ended 
	if( !m_pcAnimatedText->HasAnimationEnded() )
	{
		//Checks if the text skipped flag has been set 
		if( m_bTextSkipped )
		{
			//Handles the actions when the Enter key is pressed 
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				//Play the Menu ok sound effect 
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				//Change the screen to the Game log screen 
				ReplaceScene( TGCGameLayerSceneCreator< CGameLogo >::CreateScene() );
			}
		}
		else
		{
			//Handles the actions when the Enter key is pressed 
			if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
			{
				//Play the Menu ok sound effect 
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
				//Skip the ticker text animation 
				m_pcAnimatedText->SkipAnimation();
				//Set the Text skipped flag 
				m_bTextSkipped = true;
			}
		}
	}
	else
	{
		//Handles the actions when the Enter key is pressed 
		if( pKeyManager->ActionHasJustBeenPressed( u32( EInput::EI_Skip ) ) )
		{
			//Play the Menu ok sound effect 
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
			//Change the screen to the Game log screen 
			ReplaceScene( TGCGameLayerSceneCreator< CGameLogo >::CreateScene() );
		}
	}
}