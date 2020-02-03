#include "CMainMenu.h"
#include "AppDelegate.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CCutscene.h"
#include "CSettingsMenu.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel1.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel2.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel3.h"
#include "GamerCamp/GameSpecific/Levels/Act_1/CLevel4.h"
#include "GamerCamp/GameSpecific/Levels/Menu/CGameLogoSplash.h"


static GameConfig::EInputs		s_aePlayerActions[] = { GameConfig::EInputs::EI_Up,					GameConfig::EInputs::EI_Down,				GameConfig::EInputs::EI_AxisY,				GameConfig::EInputs::EI_Select,		GameConfig::EInputs::EI_Back,		GameConfig::EInputs::EI_AxisX,				GameConfig::EInputs::EI_Left,				GameConfig::EInputs::EI_Right };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A, cocos2d::Controller::Key::BUTTON_B, cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::BUTTON_DPAD_LEFT, cocos2d::Controller::Key::BUTTON_DPAD_RIGHT };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CMainMenu::CMainMenu()
	: IGCGameLayer( GetGCTypeIDOf( CMainMenu ) )
	, m_iSelected( 0 )
	, m_bKeyReleased( false )
{
	m_pcAudioManager = new CAudioManager();
}

CMainMenu::~CMainMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CMainMenu::onEnter()
{
	IGCGameLayer::onEnter();

	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_NONE,			// EPA_NULL,
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_ENTER
	};

	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CMainMenu::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Start game button
	MenuItemImage* pItemStartGame = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Start/Menu_Start_Normal.png",
		"Loose/UI/Buttons/Menu_Start/Menu_Start_Selected.png",
		CC_CALLBACK_1( CMainMenu::CB_OnGameStartButton, this ) );

	pItemStartGame->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f - 20.0f ),
		origin.y + ( visibleSize.height * 0.9f - 70 ) ) );
	m_pcButtonsList[ 0 ] = pItemStartGame;

	// Load game button
	MenuItemImage* pItemLoadGame = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Load/Menu_Load_Normal.png",
		"Loose/UI/Buttons/Menu_Load/Menu_Load_Selected.png",
		CC_CALLBACK_1( CMainMenu::CB_OnGameLoadButton, this ) );
	pItemLoadGame->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f + 20.0f ),
		origin.y + ( visibleSize.height * 0.8f - 120.0f ) ) );
	m_pcButtonsList[ 1 ] = pItemLoadGame;

	// Settings button
	MenuItemImage* pItemOptionGame = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Option/Menu_Option_Normal.png",
		"Loose/UI/Buttons/Menu_Option/Menu_Option_Selected.png",
		CC_CALLBACK_1( CMainMenu::CB_OnGameOptionButton, this ) );
	pItemOptionGame->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f - 20.0f ),
		origin.y + ( visibleSize.height * 0.7f - 170.0f ) ) );
	m_pcButtonsList[ 2 ] = pItemOptionGame;

	// Quit game button
	MenuItemImage* pItemQuitGame = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Quit/Menu_Quit_Normal.png",
		"Loose/UI/Buttons/Menu_Quit/Menu_Quit_Selected.png",
		CC_CALLBACK_1( CMainMenu::CB_OnGameQuitButton, this ) );
	pItemQuitGame->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f + 20.0f ),
		origin.y + ( visibleSize.height * 0.6f - 220.0f ) ) );
	m_pcButtonsList[ 3 ] = pItemQuitGame;

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pItemStartGame, pItemLoadGame, pItemQuitGame, pItemOptionGame, NULL );
	pMenu->setPosition( Vec2::ZERO );
	m_pcButtonsList[ 0 ]->selected();
	this->addChild( pMenu, 1 );

	// Create Background sprite"
	Sprite* pSprite = Sprite::create( "Loose/HelloWorld.png" );
	// position the sprite on the center of the screen
	pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
	// add the sprite as a child to this layer
	this->addChild( pSprite, 0 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );
}

void CMainMenu::VOnUpdate( f32 fTimeStep )
{
	TGCController< GameConfig::EInputs > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	if( cController.IsActive() )
	{
		ControllerControls( cController );
	}
	else
	{
		KeyboardControls();
	}

	IGCGameLayer::VOnUpdate( fTimeStep );
}

void CMainMenu::VOnDestroy()
{
	m_pcAudioManager->StopBackgroundMusic();

	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;
	IGCGameLayer::VOnDestroy();
}

void CMainMenu::VOnReset()
{
	//Check if background music is already playing, if its not then play the music
	if( !m_pcAudioManager->MenuMusicIsPlaying() )
	{
		m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Menu, true );
	}
	IGCGameLayer::VOnReset();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CMainMenu::KeyboardControls()
{
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();
	//handle actions when the up arrow key is pressed 
	if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) && m_bKeyReleased )
	{
		//play the menu up sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp, false );
		//reset the flag 
		m_bKeyReleased = false;
		// Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != -1 )
			{
				m_iSelected--;
				//Logic to loop around the selections 
				if( m_iSelected == -1 )
				{
					m_iSelected = 3;
				}
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_pcButtonsList[ m_iSelected ]->selected();
		}

	}
	//handle actions when the down arrow key is pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) && m_bKeyReleased )
	{
		//play menu down sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
		//reset the flag 
		m_bKeyReleased = false;
		// Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != 4 )
			{
				m_iSelected++;
				//Logic to loop around the selections 
				if( m_iSelected == 4 )
				{
					m_iSelected = 0;
				}
			}
		}
		//Show that the button is selected 
		m_pcButtonsList[ m_iSelected ]->selected();
	}
	//Handle actions when the Enter Key is selected 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) && m_bKeyReleased )
	{
		//play the menu ok sound effect
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		if( m_iSelected != -1 )
		{
			//reset the flag 
			m_bKeyReleased = false;
			//activate the selected button 
			m_pcButtonsList[ m_iSelected ]->activate();
		}
	}

	//all these set the key released flag 
	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Up ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Down ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Left ) ) )
	{
		m_bKeyReleased = true;
	}
}

void CMainMenu::ControllerControls( TGCController< GameConfig::EInputs > cController )
{
	//handle the actions when either the joystick is moved up or the up key on the d-pad is pressed
	if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) >= 0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Up ) ) && m_bKeyReleased )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp, false );
		//reset the flag 
		m_bKeyReleased = false;
		// Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != -1 )
			{
				m_iSelected--;
				//Logic to loop around the selections 
				if( m_iSelected == -1 )
				{
					m_iSelected = 3;
				}
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_pcButtonsList[ m_iSelected ]->selected();
		}
	}
	//handle the actions when either the joystick is moved down or the down key on the d-pad is pressed
	else if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) <= -0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Down ) ) && m_bKeyReleased )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
		//reset the flag 
		m_bKeyReleased = false;
		// Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != 4 )
			{
				m_iSelected++;
				//Logic to loop around the selections 
				if( m_iSelected == 4 )
				{
					m_iSelected = 0;
				}
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_pcButtonsList[ m_iSelected ]->selected();
		}
	}
	//Handles the action when the X button is pressed 
	else if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Select ) && m_bKeyReleased )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		if( m_iSelected != -1 )
		{
			//reset the flag 
			m_bKeyReleased = false;
			//activate the selected button
			m_pcButtonsList[ m_iSelected ]->activate();
		}

	}

	// Handles the action when the O button is pressed 
	if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Back ) && m_bKeyReleased )
	{
		ReplaceScene( TGCGameLayerSceneCreator< CGameLogo >::CreateScene() );
	}
	
	//all these set the key released flag 
	if( cController.ButtonHasJustBeenReleased( GameConfig::EInputs::EI_Down ) || cController.ButtonHasJustBeenReleased( GameConfig::EInputs::EI_Up ) )
	{
		m_bKeyReleased = true;
	}
	if( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) >= -0.1 &&   cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) <= 0.1 )
	{
		m_bKeyReleased = true;
	}
	if( cController.ButtonHasJustBeenReleased( GameConfig::EInputs::EI_Select ) )
	{
		m_bKeyReleased = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// UI Callbacks
//////////////////////////////////////////////////////////////////////////
void CMainMenu::CB_OnGameQuitButton( Ref* pSender )
{
	//End the Game 
	Director::getInstance()->end();
}

void CMainMenu::CB_OnGameStartButton( Ref* pSender )
{
	//Change screen to the Cutscene 1 screen
	ReplaceScene( TGCGameLayerSceneCreator< CCutscene1 >::CreateScene() );
}

void CMainMenu::CB_OnGameLoadButton( Ref* pSender )
{
	CDataPersistence* pcData = new CDataPersistence();
	//Get the level stored from data persistence 
	int iLevel = pcData->GetLevel();

	//Depending on which level was stored change the screen to that level 
	switch( iLevel )
	{

	case 1:
		ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
		break;
	case 2:
		ReplaceScene( TGCGameLayerSceneCreator< CLevel2 >::CreateScene() );
		break;
	case 3:
		ReplaceScene( TGCGameLayerSceneCreator< CLevel3 >::CreateScene() );
		break;
	case 4:
		ReplaceScene( TGCGameLayerSceneCreator< CLevel4 >::CreateScene() );
		break;
	default:
		ReplaceScene( TGCGameLayerSceneCreator< CCutscene1 >::CreateScene() );
		break;
	}
}

void CMainMenu::CB_OnGameOptionButton( Ref* pSender )
{
	//Push the Settings screen on top of this screen
	PushScene( TGCGameLayerSceneCreator< CSettingsMenu >::CreateScene() );
}