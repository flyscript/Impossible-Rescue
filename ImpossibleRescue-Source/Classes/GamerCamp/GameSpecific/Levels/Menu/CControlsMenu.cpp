#include "CControlsMenu.h"
#include "AppDelegate.h"

static GameConfig::EInputs		s_aePlayerActions[] = { GameConfig::EInputs::EI_Up,					GameConfig::EInputs::EI_Down,				GameConfig::EInputs::EI_AxisY,				GameConfig::EInputs::EI_Select,		GameConfig::EInputs::EI_Back,		GameConfig::EInputs::EI_AxisX,				GameConfig::EInputs::EI_Left,				GameConfig::EInputs::EI_Right };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A, cocos2d::Controller::Key::BUTTON_B, cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::BUTTON_DPAD_LEFT, cocos2d::Controller::Key::BUTTON_DPAD_RIGHT };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CControlsMenu::CControlsMenu()
	: IGCGameLayer( GetGCTypeIDOf( CControlsMenu ) )
	, m_bKeyReleased( false )
	, m_iSelected( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CControlsMenu::~CControlsMenu()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CControlsMenu::onEnter()
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

void CControlsMenu::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	// Creating the Title Sprite
	//////////////////////////////////////////////////////////////////////////
	
	Sprite* pTitleSprite = Sprite::create( "Loose/UI/Titles/Controls_Title.png" );
	// position the sprite to the top of the screen
	pTitleSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f ), origin.y + ( visibleSize.height* 0.9f ) ) );
	// add the sprite as a child to this layer
	this->addChild( pTitleSprite, 1 );

	//////////////////////////////////////////////////////////////////////////
	// Creating the back button
	//////////////////////////////////////////////////////////////////////////
	
	// add a "close" icon to exit the progress. it's an autorelease object
	// Quit game button
	MenuItemImage* pItemBack = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Normal.png",
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Selected.png",
		CC_CALLBACK_1( CControlsMenu::CB_OnBackButton, this ) );
	pItemBack->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f ),
		origin.y + ( visibleSize.height * 0.1f ) - 40.0f ) );
	m_apcButtonsList[ 0 ] = pItemBack;
	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pItemBack , NULL );
	pMenu->setPosition( Vec2::ZERO );
	m_apcButtonsList[ 0 ]->selected();
	this->addChild( pMenu , 1 );

	//////////////////////////////////////////////////////////////////////////
	// Creating the background sprite
	//////////////////////////////////////////////////////////////////////////
	
	cocos2d::Sprite* pSprite = Sprite::create( "Loose/Backgrounds/Splash/Controls_Plain.png" );
	// add the sprite as a child to this layer
	this->addChild( pSprite, 0 );
	// position the sprite on the center of the screen
	pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y ) );


	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );
}

void CControlsMenu::VOnUpdate( f32 fTimeStep )
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

void CControlsMenu::VOnDestroy()
{
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// UI Callbacks
//////////////////////////////////////////////////////////////////////////
void CControlsMenu::CB_OnBackButton( Ref* pSender )
{
	//Play the back Sound Effect
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuBack, false );
	//Go back to the previous Screen 
	Director::getInstance()->popScene();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CControlsMenu::KeyboardControls()
{
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();
	//Handle the actions when the Up arrow is pressed 
	if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) && m_bKeyReleased )
	{
		//Play the menu up sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp, false );
		//reset the flag
		m_bKeyReleased = false;
		//Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_apcButtonsList[ m_iSelected ]->unselected();

			//Selection Logic 
			if( m_iSelected != 0 )
			{
				m_iSelected--;
			}
		}
		//Show which Button is selected
		if( m_iSelected != -1 )
		{
			m_apcButtonsList[ m_iSelected ]->selected();
		}

	}

	//Handle action when the down Key is pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) && m_bKeyReleased )
	{
		//Play the menu down sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
		//reset the flag 
		m_bKeyReleased = false;
		//Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected buttons
			m_apcButtonsList[ m_iSelected ]->unselected();

			//Selection Logic
			if( m_iSelected != 0 )
			{
				m_iSelected++;
			}
		}
		//Show that the button is selected 
		m_apcButtonsList[ m_iSelected ]->selected();
	}

	//Handle actions when the Enter key is pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) && m_bKeyReleased )
	{
		if( m_iSelected != -1 )
		{
			//Reset the flag
			m_bKeyReleased = false;
			//Activate the selected button 
			m_apcButtonsList[ m_iSelected ]->activate();
		}
	}

	// Press O to quit
// 	if ( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Hack ) ) )
// 	{
// 		m_apcButtonsList[2]->activate();
// 	}

	// All these set the KeyReleased flag
	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Up ) ) )
	{
		m_bKeyReleased = true;
		//return;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Down ) ) )
	{
		m_bKeyReleased = true;
		//return;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Left ) ) )
	{
		m_bKeyReleased = true;
		//return;
	}
}

void CControlsMenu::ControllerControls( TGCController<GameConfig::EInputs> cController )
{
	//Handle actions when either the joystick is moved up or the up key on the d-pad is pressed  
	if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) >= 0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Up ) ) && m_bKeyReleased )
	{
		//Play the menu up sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp, false );
		//reset the flag 
		m_bKeyReleased = false;
		//Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-Select the selected Button 
			m_apcButtonsList[ m_iSelected ]->unselected();

			//Selection Logic 
			if( m_iSelected != 0 )
			{
				m_iSelected--;
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_apcButtonsList[ m_iSelected ]->selected();
		}
	}
	//Handle the actions when either  the joystick is moved down or the down key on the d-pad is pressed 
	else if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) <= -0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Down ) ) && m_bKeyReleased )
	{
		//play the menu down music 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
		//reset the flag 
		m_bKeyReleased = false;
		//Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			//De-select the selected button 
			m_apcButtonsList[ m_iSelected ]->unselected();

			//Selection logic
			if( m_iSelected != 0 )
			{
				m_iSelected++;
			}
		}
		//Show that the button is selected 
		if( m_iSelected != -1 )
		{
			m_apcButtonsList[ m_iSelected ]->selected();
		}
	}

	//handle actions when the X button has been pressed  
	else if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Select ) && m_bKeyReleased )
	{
		if( m_iSelected != -1 )
		{
			//Reset the flag
			m_bKeyReleased = false;
			//Activate the selected button 
			m_apcButtonsList[ m_iSelected ]->activate();
		}

	}
	// Handle actions when the circle/O button has been pressed 
	if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Back ) && m_bKeyReleased )
	{
		//activate the back button 
		m_apcButtonsList[ 0 ]->activate();
	}
	//These all set the KeyReleased flag
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




