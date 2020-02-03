#include "CPauseMenu.h"
#include "AppDelegate.h"
#include "CMainMenu.h"
#include "CControlsMenu.h"
#include "GamerCamp/EventManager/CEventManager.h"
#include "CAudioSetting.h"

static GameConfig::EInputs		s_aePlayerActions[] = { GameConfig::EInputs::EI_Up,					GameConfig::EInputs::EI_Down,				GameConfig::EInputs::EI_AxisY,				GameConfig::EInputs::EI_Select,		GameConfig::EInputs::EI_Back,		GameConfig::EInputs::EI_AxisX,				GameConfig::EInputs::EI_Left,				GameConfig::EInputs::EI_Right };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A, cocos2d::Controller::Key::BUTTON_B, cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::BUTTON_DPAD_LEFT, cocos2d::Controller::Key::BUTTON_DPAD_RIGHT };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CPauseMenu::CPauseMenu()
	: IGCGameLayer( GetGCTypeIDOf( CPauseMenu ) )
	, m_bKeyReleased( false )
	, m_iSelected( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CPauseMenu::~CPauseMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CPauseMenu::onEnter()
{
	IGCGameLayer::onEnter();

	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_NONE,			// EPA_NULL,
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_ENTER,			// EPA_Left,
	};

	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CPauseMenu::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Create the Title sprite
	Sprite* pTitleSprite = Sprite::create( "Loose/UI/Titles/Pause_Title.png" );
	// position the sprite at the top of the screen
	pTitleSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f ), origin.y + ( visibleSize.height* 0.8f ) + 50.0f ) );
	// add the sprite as a child to this layer
	this->addChild( pTitleSprite, 1 );

	// Resume game button
	m_pItemResume = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Resume/Menu_Resume_Normal.png",
		"Loose/UI/Buttons/Menu_Resume/Menu_Resume_Selected.png",
		CC_CALLBACK_1( CPauseMenu::CB_OnGameResumeButton, this ) );
	m_pItemResume->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f - 20.0f ),
		origin.y + ( visibleSize.height * 0.7f ) ) );
	m_apcButtonsList[ 0 ] = m_pItemResume;

	// Audio game button
	m_pItemAudio = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Audio/Menu_Audio_Normal.png",
		"Loose/UI/Buttons/Menu_Audio/Menu_Audio_Selected.png",
		CC_CALLBACK_1( CPauseMenu::CB_OnAudioSettingsButton, this ) );
	m_pItemAudio->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f + 20.0f ),
		origin.y + ( visibleSize.height * 0.6f - 50.0f ) ) );
	m_apcButtonsList[ 1 ] = m_pItemAudio;

	// Controls button
	m_pItemControls = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Controls/Menu_Controls_Normal.png",
		"Loose/UI/Buttons/Menu_Controls/Menu_Controls_Selected.png",
		CC_CALLBACK_1( CPauseMenu::CB_OnControlsButton, this ) );
	m_pItemControls->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f - 20.0f ),
		origin.y + ( visibleSize.height * 0.5f - 100.0f ) ) );
	m_apcButtonsList[ 2 ] = m_pItemControls;

	// Quit game button
	m_pItemMainMenu = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Quit/Menu_Quit_Normal.png",
		"Loose/UI/Buttons/Menu_Quit/Menu_Quit_Selected.png",
		CC_CALLBACK_1( CPauseMenu::CB_OnMainMenuButton, this ) );
	m_pItemMainMenu->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f + 20.0f ),
		origin.y + ( visibleSize.height * 0.4f - 150.0f ) ) );
	m_apcButtonsList[ 3 ] = m_pItemMainMenu;

	// create menu, it's an autorelease object
	m_pMenu = Menu::create( m_pItemResume, m_pItemControls, m_pItemAudio, m_pItemMainMenu, NULL );
	m_pMenu->setPosition( Vec2::ZERO );
	this->addChild( m_pMenu, 1 );

	m_apcButtonsList[ 0 ]->selected();

	// create and initialize a label
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Menu, true );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	if( m_pcAudioManager->IsFootstepPlaying() )
	{
		m_pcAudioManager->PauseFootStep();
	}
}

void CPauseMenu::VOnUpdate( f32 fTimeStep )
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

void CPauseMenu::VOnDestroy()
{

	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CPauseMenu::KeyboardControls()
{
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();
	//handles actions when the up arrow key has been pressed 
	if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) && m_bKeyReleased )
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
			m_apcButtonsList[ m_iSelected ]->unselected();

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
			m_apcButtonsList[ m_iSelected ]->selected();
		}

	}
	//handles actions when the down arrow key has been pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) && m_bKeyReleased )
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
			m_apcButtonsList[ m_iSelected ]->unselected();

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
		m_apcButtonsList[ m_iSelected ]->selected();
	}
	//handles actions when the enter key has been pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) && m_bKeyReleased )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		if( m_iSelected != -1 )
		{
			//reset the flag 
			m_bKeyReleased = false;
			//activate the selected button
			m_apcButtonsList[ m_iSelected ]->activate();
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

void CPauseMenu::ControllerControls( TGCController< GameConfig::EInputs > cController )
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
			m_apcButtonsList[ m_iSelected ]->unselected();

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
			m_apcButtonsList[ m_iSelected ]->selected();
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
			m_apcButtonsList[ m_iSelected ]->unselected();

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
			m_apcButtonsList[ m_iSelected ]->selected();
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
			m_apcButtonsList[ m_iSelected ]->activate();
		}

	}

	//Handles the action when the O button is pressed
	if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Back ) && m_bKeyReleased )
	{
		m_apcButtonsList[ 0 ]->activate();
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
void CPauseMenu::CB_OnControlsButton( Ref* pSender )
{
	//Push the Controls screen on top of this screen 
	PushScene( TGCGameLayerSceneCreator< CControlsMenu >::CreateScene() );
}

void CPauseMenu::CB_OnAudioSettingsButton( Ref* pSender )
{
	//push the Audio screen on top of this screen 
	PushScene( TGCGameLayerSceneCreator< CAudioSetting >::CreateScene() );
}

void CPauseMenu::CB_OnGameResumeButton( Ref* pSender )
{
	m_pcAudioManager->StopBackgroundMusic();
	//trigger the enable control event
	CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlEnable, nullptr );
	//Go back to the previous screen
	Director::getInstance()->popScene();
	//Resume the old background music
	m_pcAudioManager->ResumeBackground();
}

void CPauseMenu::CB_OnMainMenuButton( Ref* pSender )
{
	//trigger the Return to menu event
	CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ReturnToMenu, nullptr );
	//go back to the previous screen 
	Director::getInstance()->popScene();
}