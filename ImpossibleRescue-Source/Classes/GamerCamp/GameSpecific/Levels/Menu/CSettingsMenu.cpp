#include "CSettingsMenu.h"
#include "AppDelegate.h"
#include "CControlsMenu.h"
#include "CAudioSetting.h"

static GameConfig::EInputs		s_aePlayerActions[] = { GameConfig::EInputs::EI_Up,					GameConfig::EInputs::EI_Down,				GameConfig::EInputs::EI_AxisY,				GameConfig::EInputs::EI_Select,		GameConfig::EInputs::EI_Back,		GameConfig::EInputs::EI_AxisX,				GameConfig::EInputs::EI_Left,				GameConfig::EInputs::EI_Right };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A, cocos2d::Controller::Key::BUTTON_B, cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::BUTTON_DPAD_LEFT, cocos2d::Controller::Key::BUTTON_DPAD_RIGHT };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CSettingsMenu::CSettingsMenu()
	: IGCGameLayer( GetGCTypeIDOf( CSettingsMenu ) )
	, m_bKeyReleased( false )
	, m_iSelected( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CSettingsMenu::~CSettingsMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CSettingsMenu::onEnter()
{
	IGCGameLayer::onEnter();

	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_NONE,			// EPA_NULL,
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_ENTER			// EPA_Left,
	};


	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CSettingsMenu::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// Create Title Sprite
	Sprite* pTitleSprite = Sprite::create( "Loose/UI/Titles/Settings_Title.png" );
	// position the sprite at the top of the screen
	pTitleSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f ), origin.y + ( visibleSize.height* 0.9f ) ) );
	// add the sprite as a child to this layer
	this->addChild( pTitleSprite, 1 );


	// Controls Button
	MenuItemImage* pItemControls = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Controls/Menu_Controls_Normal.png",
		"Loose/UI/Buttons/Menu_Controls/Menu_Controls_Selected.png",
		CC_CALLBACK_1( CSettingsMenu::CB_OnControlButton, this ) );

	pItemControls->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f - 20.0f ),
		origin.y + ( visibleSize.height * 0.7f + 50.0f ) ) );
	m_pcButtonsList[ 0 ] = pItemControls;

	// Load game button
	MenuItemImage* pItemAudioSetting = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Audio/Menu_Audio_Normal.png",
		"Loose/UI/Buttons/Menu_Audio/Menu_Audio_Selected.png",
		CC_CALLBACK_1( CSettingsMenu::CB_OnAudioSettingButton, this ) );

	pItemAudioSetting->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f + 20.0f ),
		origin.y + ( visibleSize.height * 0.6f ) ) );
	m_pcButtonsList[ 1 ] = pItemAudioSetting;

	// Quit game button
	MenuItemImage* pItemBack = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Normal.png",
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Selected.png",
		CC_CALLBACK_1( CSettingsMenu::CB_OnBackButton, this ) );

	pItemBack->setPosition( Vec2( origin.x + ( visibleSize.width * 0.8f ),
		origin.y + ( visibleSize.height * 0.4f ) ) );
	m_pcButtonsList[ 2 ] = pItemBack;

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pItemControls, pItemAudioSetting, pItemBack, NULL );
	//Menu* pMenu = Menu::create( pItemStartGame , pItemQuitGame , NULL );
	pMenu->setPosition( Vec2::ZERO );
	m_pcButtonsList[ 0 ]->selected();
	this->addChild( pMenu, 1 );


	// Create background sprite
	Sprite* pSprite = Sprite::create( "Loose/HelloWorld.png" );
	// position the sprite on the center of the screen
	pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );
	// add the sprite as a child to this layer
	this->addChild( pSprite, 0 );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

}

void CSettingsMenu::VOnUpdate( f32 fTimeStep )
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

void CSettingsMenu::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// UI Callbacks
//////////////////////////////////////////////////////////////////////////
void CSettingsMenu::CB_OnControlButton( Ref* pSender )
{
	//play the menu ok sound effect 
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
	// push the Controls screen on top of this screen  
	PushScene( TGCGameLayerSceneCreator< CControlsMenu >::CreateScene() );
}

void CSettingsMenu::CB_OnAudioSettingButton( Ref* pSender )
{
	//play the menu ok sound effect
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
	//push the audio settings screen on top of this screen
	PushScene( TGCGameLayerSceneCreator< CAudioSetting >::CreateScene() );
}

void CSettingsMenu::CB_OnBackButton( Ref* pSender )
{
	//play the menu back sound effect
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuBack, false );
	//go back to the previous screen 
	Director::getInstance()->popScene();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CSettingsMenu::KeyboardControls()
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
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != -1 )
			{
				m_iSelected--;
				//Logic to loop around the selections 
				if( m_iSelected == -1 )
				{
					m_iSelected = 2;
				}
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_pcButtonsList[ m_iSelected ]->selected();
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
			m_pcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != 3 )
			{
				m_iSelected++;
				//Logic to loop around the selections 
				if( m_iSelected == 3 )
				{
					m_iSelected = 0;
				}
			}
		}
		//Show that the button is selected 
		m_pcButtonsList[ m_iSelected ]->selected();
	}
	//handles actions when the enter key has been pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) && m_bKeyReleased )
	{
		if( m_iSelected != -1 )
		{
			//reset the flag 
			m_bKeyReleased = false;
			//activate the selected button
			m_pcButtonsList[ m_iSelected ]->activate();
		}
	}
	
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

void CSettingsMenu::ControllerControls( TGCController<GameConfig::EInputs> cController )
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
					m_iSelected = 2;
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
			if( m_iSelected != 3 )
			{
				m_iSelected++;
				//Logic to loop around the selections 
				if( m_iSelected == 3 )
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
		if( m_iSelected != -1 )
		{
			//reset the flag 
			m_bKeyReleased = false;
			//activate the selected button
			m_pcButtonsList[ m_iSelected ]->activate();
		}

	}

	//Handles the action when the O button is pressed
	if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Back ) && m_bKeyReleased )
	{
		m_pcButtonsList[ 2 ]->activate();
	}

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


