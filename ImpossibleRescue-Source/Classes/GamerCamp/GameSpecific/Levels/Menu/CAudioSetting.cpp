#include "CAudioSetting.h"
#include "AppDelegate.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"


static GameConfig::EInputs		s_aePlayerActions[] = { GameConfig::EInputs::EI_Up,					GameConfig::EInputs::EI_Down,				GameConfig::EInputs::EI_AxisY,				GameConfig::EInputs::EI_Select,		GameConfig::EInputs::EI_Back,		GameConfig::EInputs::EI_AxisX,				GameConfig::EInputs::EI_Left,				GameConfig::EInputs::EI_Right };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,	cocos2d::Controller::Key::JOYSTICK_LEFT_Y,	cocos2d::Controller::Key::BUTTON_A, cocos2d::Controller::Key::BUTTON_B, cocos2d::Controller::Key::JOYSTICK_LEFT_X,	cocos2d::Controller::Key::BUTTON_DPAD_LEFT, cocos2d::Controller::Key::BUTTON_DPAD_RIGHT };

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CAudioSetting::CAudioSetting()
	: IGCGameLayer( GetGCTypeIDOf( CAudioSetting ) )
	, m_bKeyReleased( false )
	, m_bKeyReleasedX( true )
	, m_iSelected( 0 )
{
	m_pcAudioManager = new CAudioManager();
}

CAudioSetting::~CAudioSetting()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CAudioSetting::onEnter()
{
	IGCGameLayer::onEnter();
	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_NONE,			// EPA_NULL,
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_LEFT_ARROW,		// EPA_Left,
		EventKeyboard::KeyCode::KEY_RIGHT_ARROW,	// EPA_Right,
		EventKeyboard::KeyCode::KEY_ENTER,			// EPA_Jump,
		EventKeyboard::KeyCode::KEY_ESCAPE          // EPA_Fire
	};
	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CAudioSetting::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////
	// Creating the Title Sprite
	//////////////////////////////////////////////////////////////////////////
	
	Sprite* pTitleSprite = Sprite::create( "Loose/UI/Titles/Audio_Settings_title.png" );
	// position the sprite to the top of screen
	pTitleSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f ), origin.y + ( visibleSize.height* 0.8f ) ) );
	// add the sprite as a child to this layer
	this->addChild( pTitleSprite, 1 );


	//////////////////////////////////////////////////////////////////////////
	// Creating the Sound Effects Slider and label
	//////////////////////////////////////////////////////////////////////////

	// Creating the label 
	Sprite* pEffectsSprite = Sprite::create( "Loose/UI/Titles/sound_effects_subtitle.png" );
	// position the sprite 
	pEffectsSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.3f ), origin.y + ( visibleSize.height* 0.6f ) ) );
	// add the sprite as a child to this layer
	this->addChild( pEffectsSprite, 1 );

	//Creating the Slider
	m_pcAudioEffectsSlider = ui::Slider::create();
	m_pcAudioEffectsSlider->loadBarTexture( "Loose/Track.png" );
	m_pcAudioEffectsSlider->loadSlidBallTextureNormal( "Loose/BallSelected.png" );
	m_pcAudioEffectsSlider->loadSlidBallTextureDisabled( "Loose/Ball.png" );
	m_pcAudioEffectsSlider->loadSlidBallTexturePressed( "Loose/BallSelected.png" );
	m_pcAudioEffectsSlider->loadProgressBarTexture( "Loose/Progress.png" );
	m_pcAudioEffectsSlider->setPosition( Vec2( origin.x + ( visibleSize.width * 0.7f ),
		origin.y + ( visibleSize.height * 0.6f ) ) );
	m_pcAudioEffectsSlider->addEventListener( CC_CALLBACK_2( CAudioSetting::CB_OnEffectSliderChange, this ) );
	m_pcAudioEffectsSlider->setEnabled( true );
	this->addChild( m_pcAudioEffectsSlider, 2 );
	
	//Adding in custom sprite for the start of the slider
	Sprite* pEffectBarStartSprite = Sprite::create( "Loose/Bar_Start.png" );
	// position the sprite at the start of the slider
	pEffectBarStartSprite->setPosition( 
		Vec2( m_pcAudioEffectsSlider->getPosition().x - m_pcAudioEffectsSlider->getContentSize().width / 2 
					- pEffectBarStartSprite->getContentSize().width / 2 + 1.0f, 
				 m_pcAudioEffectsSlider->getPosition().y + pEffectBarStartSprite->getContentSize().height / 2 - 3.0f ) );
	// add the sprite as a child to this layer
	this->addChild( pEffectBarStartSprite, 1 );

	//Adding in custom sprite for the end of the slider
	Sprite* pEffectBarEndSprite = Sprite::create( "Loose/Bar_End.png" );
	// position the sprite at the end of the slider
	pEffectBarEndSprite->setPosition( 
		Vec2( m_pcAudioEffectsSlider->getPosition().x + m_pcAudioEffectsSlider->getContentSize().width / 2 
					+ pEffectBarEndSprite->getContentSize().width / 2 + 0.5f, 
				 m_pcAudioEffectsSlider->getPosition().y - pEffectBarEndSprite->getContentSize().height / 2 + 3.0f ) );
	// add the sprite as a child to this layer
	this->addChild( pEffectBarEndSprite, 1 );


	//////////////////////////////////////////////////////////////////////////
	// Creating the Background Music Slider and label
	//////////////////////////////////////////////////////////////////////////

	// Creating the label 
	Sprite* pBackgroundSprite = Sprite::create( "Loose/UI/Titles/background_music_subtitle.png" );
	// position the sprite
	pBackgroundSprite->setPosition( Vec2( origin.x + ( visibleSize.width * 0.3f ), origin.y + ( visibleSize.height* 0.5f ) ) );
	// add the sprite as a child to this layer
	this->addChild( pBackgroundSprite, 1 );

	//Creating the Slider
	m_pcAudioBackgroundSlider = ui::Slider::create();
	m_pcAudioBackgroundSlider->loadBarTexture( "Loose/Track.png" );
	m_pcAudioBackgroundSlider->loadSlidBallTextureNormal( "Loose/BallSelected.png" );
	m_pcAudioBackgroundSlider->loadSlidBallTextureDisabled( "Loose/Ball.png" );
	m_pcAudioBackgroundSlider->loadSlidBallTexturePressed( "Loose/BallSelected.png" );
	m_pcAudioBackgroundSlider->loadProgressBarTexture( "Loose/Progress.png" );
	m_pcAudioBackgroundSlider->setPosition( Vec2( origin.x + ( visibleSize.width * 0.7f ),
		origin.y + ( visibleSize.height * 0.5f ) ) );
	m_pcAudioBackgroundSlider->addEventListener( CC_CALLBACK_2( CAudioSetting::CB_OnBackgroundSliderChange, this ) );
	m_pcAudioBackgroundSlider->setEnabled( false );
	this->addChild( m_pcAudioBackgroundSlider, 2 );

	//Adding in custom sprite for the start of the slider
	Sprite* pBackgroundBarStartSprite = Sprite::create( "Loose/Bar_Start.png" );
	// position the sprite at the start of the slider
	pBackgroundBarStartSprite->setPosition( 
		Vec2( m_pcAudioBackgroundSlider->getPosition().x - m_pcAudioBackgroundSlider->getContentSize().width / 2 
					- pBackgroundBarStartSprite->getContentSize().width / 2 + 1.0f , 
				 m_pcAudioBackgroundSlider->getPosition().y + pBackgroundBarStartSprite->getContentSize().height / 2 - 3.0f ) );
	// add the sprite as a child to this layer
	this->addChild( pBackgroundBarStartSprite , 1 );

	//Adding in custom sprite for the end of the slider
	Sprite* pBackgroundBarEndSprite = Sprite::create( "Loose/Bar_End.png" );
	// position the sprite at the end of the slider
	pBackgroundBarEndSprite->setPosition( 
		Vec2( m_pcAudioBackgroundSlider->getPosition().x + m_pcAudioBackgroundSlider->getContentSize().width / 2 
					+ pBackgroundBarEndSprite->getContentSize().width / 2 + 0.5f , 
				 m_pcAudioBackgroundSlider->getPosition().y - pBackgroundBarEndSprite->getContentSize().height / 2 + 3.0f ) );
	// add the sprite as a child to this layer
	this->addChild( pBackgroundBarEndSprite , 1 );

	//////////////////////////////////////////////////////////////////////////
	// Creating the Back button
	//////////////////////////////////////////////////////////////////////////
	
	MenuItemImage* pItemBack = MenuItemImage::create(
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Normal.png",
		"Loose/UI/Buttons/Menu_Back/Menu_Back_Selected.png",
		CC_CALLBACK_1( CAudioSetting::CB_OnBackButton, this ) );
	pItemBack->setPosition( Vec2( origin.x + ( visibleSize.width * 0.5f ),
		origin.y + ( visibleSize.height * 0.3f ) ) );
	m_apcButtonsList[ 0 ] = pItemBack;

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create( pItemBack, NULL );
	//Menu* pMenu = Menu::create( pItemStartGame , pItemQuitGame , NULL );
	pMenu->setPosition( Vec2::ZERO );
	this->addChild( pMenu, 1 );

	
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	//Get the Volume percentages from data persistence
	m_iEffectsPercent = m_pcAudioManager->GetEffectsVolune();
	m_iBackgroundPercent = m_pcAudioManager->GetBackgroundVolume();

	//Intialise the start of the sliders based on the percentages
	m_pcAudioEffectsSlider->setPercent( m_iEffectsPercent );
	m_pcAudioBackgroundSlider->setPercent( m_iBackgroundPercent );

	//Set the audio slider to be the first one lit.
	m_pcAudioEffectsSlider->setBright( true );


}

void CAudioSetting::VOnUpdate( f32 fTimeStep )
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

void CAudioSetting::VOnDestroy()
{
	delete m_pcAudioManager;
	m_pcAudioManager = nullptr;

	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;

	IGCGameLayer::VOnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// Keyboard and Controller Update Methods
//////////////////////////////////////////////////////////////////////////
void CAudioSetting::KeyboardControls()
{
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();

	//Handle all actions when the up arrow key has been pressed.
	if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) && m_bKeyReleased )
	{
		//Play the sound Menu up effect 
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
			if( m_iSelected == 0 )//Index 0 is for Audio Effects 
			{
				//De-select the Audio Effect slider
				m_pcAudioEffectsSlider->setEnabled( false );
			}
			else if( m_iSelected == 1 )// Index 1 is for  Background Music 
			{
				//De-select the Background Music slider
				m_pcAudioBackgroundSlider->setEnabled( false );
			}
			else if( m_iSelected == 2 )
			{
				//De-select the back button
				m_apcButtonsList[ 0 ]->unselected();
			}

			//Logic for updating what to select
			if( m_iSelected != -1 )
			{
				m_iSelected--;
				//Logic to loop around the options 
				if( m_iSelected == -1 )
				{
					m_iSelected = 2;
				}
			}
		}
		//Selection Logic
		if( m_iSelected == 2 ) // Index 2 is for back button
		{
			// Show that the Back button is currently selected.
			m_apcButtonsList[ 0 ]->selected();
		}
		if( m_iSelected == 1 ) // Index 1 is for the Background Music 
		{
			// Show that the Background Music slider is currently selected.
			m_pcAudioBackgroundSlider->setEnabled( true );
		}
		else if( m_iSelected == 0 ) // Index ) is for the Audio Effects 
		{
			// Show that the Audio Effects Slider is selected
			m_pcAudioEffectsSlider->setEnabled( true );
		}

	}

	//Handle all actions when the down arrow key has been pressed.
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) && m_bKeyReleased )
	{
		//Play the sound Menu Down effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
		//Reset flag
		m_bKeyReleased = false;

		//Logic for initially selecting the first item
		if( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			if( m_iSelected == 0 )//Index 0 is for Audio Effects 
			{
				//De-select the Audio Effect slider
				m_pcAudioEffectsSlider->setEnabled( false );
			}
			else if ( m_iSelected == 1 )// Index 1 is for  Background Music 
			{
				//De-select the Background Music slider
				m_pcAudioBackgroundSlider->setEnabled( false );
			}
			else if ( m_iSelected == 2 )
			{
				//De-select the back button
				m_apcButtonsList[ 0 ]->unselected();
			}
			//Selection Logic
			if( m_iSelected != 3 )
			{
				m_iSelected++;
				//Logic to loop through the selections 
				if( m_iSelected == 3 )
				{
					m_iSelected = 0;
				}
			}
		}
		//Selection Logic
		if ( m_iSelected == 2 ) // Index 2 is for back button
		{
			// Show that the Back button is currently selected.
			m_apcButtonsList[ 0 ]->selected();
		}
		if ( m_iSelected == 1 ) // Index 1 is for the Background Music 
		{
			// Show that the Background Music slider is currently selected.
			m_pcAudioBackgroundSlider->setEnabled( true );
		}
		else if ( m_iSelected == 0 ) // Index ) is for the Audio Effects 
		{
			// Show that the Audio Effects Slider is selected
			m_pcAudioEffectsSlider->setEnabled( true );
		}
	}

	//Handle all the actions when the Enter button has been pressed.
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Jump ) ) && m_bKeyReleased )
	{
		//Check that something is selected
		if( m_iSelected != -1 )
		{
			//reset the flag
			m_bKeyReleased = false;
			
			if( m_iSelected == 2 )//Index 2 is for the back button
			{
				// Activate the back button.
				m_apcButtonsList[ 0 ]->activate();
			}
		}
	}

	//Handles all the actions when the left arrow is pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) && m_bKeyReleased )
	{
		//reset the flag
		m_bKeyReleased = false;
		
		if( m_iSelected == 0 )// Index 0 is for the Audio Effects 
		{
			//Check that the percentage is not already 0
			if( m_iEffectsPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iEffectsPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioEffectsSlider->setPercent( m_iEffectsPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeEffecstVolume( m_iEffectsPercent );
		}
		
		if( m_iSelected == 1 )// Index 1 is for background music 
		{
			//Check that the percentage is not already 0
			if( m_iBackgroundPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iBackgroundPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioBackgroundSlider->setPercent( m_iBackgroundPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeBackgroundVolume( m_iBackgroundPercent );
		}
		// Play the Menu Down Sound Effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown, false );
	}

	//Handles all the actions when the right arrow is pressed 
	else if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Right ) ) && m_bKeyReleased )
	{
		//reset the flag
		m_bKeyReleased = false;
		
		if( m_iSelected == 0 )//Index 0 is for audio effects 
		{
			//Check the percentage is not already 100
			if( m_iEffectsPercent != 100 )
			{
				//Increase the percentage by 5
				m_iEffectsPercent += 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioEffectsSlider->setPercent( m_iEffectsPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeEffecstVolume( m_iEffectsPercent );
		}
		
		if( m_iSelected == 1 )// Index 1 is for background music 
		{
			//Check the percentage is not already 100
			if( m_iBackgroundPercent != 100 )
			{
				//Increase the percentage by 5
				m_iBackgroundPercent += 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioBackgroundSlider->setPercent( m_iBackgroundPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeBackgroundVolume( m_iBackgroundPercent );
		}
		//Play the Menu Up sound effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp, false );
	}

	// Handle the action when Escape has been pressed 
	if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Fire ) ) )
	{
		//Activate the back button
		m_apcButtonsList[ 0 ]->activate();
	}

	// These are all to set the KeyReleased flag 
	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Fire ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Up ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Down ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Jump ) ) )
	{
		m_bKeyReleased = true;
	}


	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Left ) ) )
	{
		m_bKeyReleased = true;
	}

	if( pKeyManager->ActionHasJustBeenReleased( u32( GameConfig::EPlayerActions::EPA_Right ) ) )
	{
		m_bKeyReleased = true;
	}
}

void CAudioSetting::ControllerControls( TGCController<GameConfig::EInputs> cController )
{
	//Handle actions when either the joystick is moved up or the up key on the d-pad has been pressed
	if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) >= 0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Up ) ) && m_bKeyReleased )
	{
		//Play the sound Menu up effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuUp , false );
		//reset the flag
		m_bKeyReleased = false;
		// Logic for initially selecting the first item
		if ( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			if ( m_iSelected == 0 )//Index 0 is for Audio Effects 
			{
				//De-select the Audio Effect slider
				m_pcAudioEffectsSlider->setEnabled( false );
			}
			else if ( m_iSelected == 1 )// Index 1 is for  Background Music 
			{
				//De-select the Background Music slider
				m_pcAudioBackgroundSlider->setEnabled( false );
			}
			else if ( m_iSelected == 2 )
			{
				//De-select the back button
				m_apcButtonsList[ 0 ]->unselected();
			}

			//Logic for updating what to select
			if ( m_iSelected != -1 )
			{
				m_iSelected--;
				//Logic to loop around the options 
				if ( m_iSelected == -1 )
				{
					m_iSelected = 2;
				}
			}
		}
		//Selection Logic
		if ( m_iSelected == 2 ) // Index 2 is for back button
		{
			// Show that the Back button is currently selected.
			m_apcButtonsList[ 0 ]->selected();
		}
		if ( m_iSelected == 1 ) // Index 1 is for the Background Music 
		{
			// Show that the Background Music slider is currently selected.
			m_pcAudioBackgroundSlider->setEnabled( true );
		}
		else if ( m_iSelected == 0 ) // Index ) is for the Audio Effects 
		{
			// Show that the Audio Effects Slider is selected
			m_pcAudioEffectsSlider->setEnabled( true );
		}
	}

	//Handle action when either the joystick is moved down or the down key on the d-pad is pressed. 
	else if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisY ) <= -0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Down ) ) && m_bKeyReleased )
	{
		//Play the sound Menu Down effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown , false );
		//Reset flag
		m_bKeyReleased = false;

		//Logic for initially selecting the first item
		if ( m_iSelected == -1 )
		{
			m_iSelected = 0;
		}
		else
		{
			if ( m_iSelected == 0 )//Index 0 is for Audio Effects 
			{
				//De-select the Audio Effect slider
				m_pcAudioEffectsSlider->setEnabled( false );
			}
			else if ( m_iSelected == 1 )// Index 1 is for  Background Music 
			{
				//De-select the Background Music slider
				m_pcAudioBackgroundSlider->setEnabled( false );
			}
			else if ( m_iSelected == 2 )
			{
				//De-select the back button
				m_apcButtonsList[ 0 ]->unselected();
			}
			//Selection Logic
			if ( m_iSelected != 3 )
			{
				m_iSelected++;
				//Logic to loop through the selections 
				if ( m_iSelected == 3 )
				{
					m_iSelected = 0;
				}
			}
		}
	}

	//Handle actions when the X button has been pressed 
	else if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Select ) && m_bKeyReleased )
	{
		//Check that something is selected
		if ( m_iSelected != -1 )
		{
			//reset the flag
			m_bKeyReleased = false;

			if ( m_iSelected == 2 )//Index 2 is for the back button
			{
				// Activate the back button.
				m_apcButtonsList[ 0 ]->activate();
			}
		}
	}

	//Handle action when either the joystick is moved right or the right key on the d-pad is pressed. 
	else if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisX ) >= 0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Right ) ) && m_bKeyReleasedX )
	{
		//reset the flag
		m_bKeyReleased = false;

		if ( m_iSelected == 0 )// Index 0 is for the Audio Effects 
		{
			//Check that the percentage is not already 0
			if ( m_iEffectsPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iEffectsPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioEffectsSlider->setPercent( m_iEffectsPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeEffecstVolume( m_iEffectsPercent );
		}

		if ( m_iSelected == 1 )// Index 1 is for background music 
		{
			//Check that the percentage is not already 0
			if ( m_iBackgroundPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iBackgroundPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioBackgroundSlider->setPercent( m_iBackgroundPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeBackgroundVolume( m_iBackgroundPercent );
		}
		// Play the Menu Down Sound Effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown , false );

	}

	//Handle action when either the joystick is moved left or the left key on the d-pad is pressed. 
	else if( ( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisX ) <= -0.8 || cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Left ) ) && m_bKeyReleasedX )
	{
		//reset the flag
		m_bKeyReleased = false;

		if ( m_iSelected == 0 )// Index 0 is for the Audio Effects 
		{
			//Check that the percentage is not already 0
			if ( m_iEffectsPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iEffectsPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioEffectsSlider->setPercent( m_iEffectsPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeEffecstVolume( m_iEffectsPercent );
		}

		if ( m_iSelected == 1 )// Index 1 is for background music 
		{
			//Check that the percentage is not already 0
			if ( m_iBackgroundPercent != 0 )
			{
				//Decrease the percentage by 5
				m_iBackgroundPercent -= 5;
			}
			//Set the New Percentage on the slider, to update its position 
			m_pcAudioBackgroundSlider->setPercent( m_iBackgroundPercent );
			//Let the Audio manager know about the new Percentage
			m_pcAudioManager->ChangeBackgroundVolume( m_iBackgroundPercent );
		}
		// Play the Menu Down Sound Effect 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuDown , false );
	}

	// Handle the action when the Circle button is pressed 
	if( cController.ButtonHasJustBeenPressed( GameConfig::EInputs::EI_Back ) && m_bKeyReleased )
	{
		//Activate the back button 
		m_apcButtonsList[ 0 ]->activate();
	}

	// These are all to set the KeyReleased flag 
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
	if( cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisX ) >= -0.1 &&   cController.GetCurrentAxisValueRaw( GameConfig::EInputs::EI_AxisX ) <= 0.1 )
	{
		m_bKeyReleasedX = true;
	}
	if( cController.ButtonHasJustBeenReleased( GameConfig::EInputs::EI_Left ) || cController.ButtonHasJustBeenReleased( GameConfig::EInputs::EI_Right ) )
	{
		m_bKeyReleasedX = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// UI Callbacks
//////////////////////////////////////////////////////////////////////////
void CAudioSetting::CB_OnBackButton( Ref* pSender )
{
	//Play the Back sound effect
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuBack, false );
	// Go back to the previous screen
	Director::getInstance()->popScene();
}

// This method is called when we change the slider using our mouse 
void CAudioSetting::CB_OnEffectSliderChange( Ref* pSender, ui::Slider::EventType ctype )
{
	if( ctype == ui::Slider::EventType::ON_PERCENTAGE_CHANGED )
	{
		ui::Slider *pSlider = dynamic_cast< ui::Slider * >( pSender );
		m_iEffectsPercent = pSlider->getPercent();
		m_pcAudioManager->ChangeEffecstVolume( m_iEffectsPercent );

	}
}

// This method is called when we change the slider using our mouse 
void CAudioSetting::CB_OnBackgroundSliderChange( Ref* pSender, ui::Slider::EventType ctype )
{
	if( ctype == ui::Slider::EventType::ON_PERCENTAGE_CHANGED )
	{
		ui::Slider *pSlider = dynamic_cast< ui::Slider * >( pSender );
		m_iBackgroundPercent = pSlider->getPercent();
		m_pcAudioManager->ChangeBackgroundVolume( m_iBackgroundPercent );

	}

}