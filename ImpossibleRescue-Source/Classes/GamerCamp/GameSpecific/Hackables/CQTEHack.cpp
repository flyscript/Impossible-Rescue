//Includes

#include "cocos2d.h"
#include "GamerCamp/GameSpecific/UI/UIAnimatedBar.h"
#include "Helper/HeaderLinker.h"

// Include own header
#include "CQTEHack.h"

// Constructor
CQTEHack::CQTEHack()
	: m_fTimePerButton( 1.0f )
	, m_iNumberOfButtons( 4 )
	, m_aeButtons( nullptr )
	, m_fPromptTime( 0.0f )
	, m_iEventsComplete( 0 )
	, m_eKeyToPress( GameConfig::EPlayerActions::EPA_NULL )
	, m_pActiveScene( nullptr )
	, m_pPromptLabel( nullptr )
	, m_pHackLabel( nullptr )
{
	m_aeButtons = new std::vector<GameConfig::EPlayerActions>;

	AddAction( GameConfig::EPlayerActions::EPA_Up );
	AddAction( GameConfig::EPlayerActions::EPA_Down );
	AddAction( GameConfig::EPlayerActions::EPA_Left );
	AddAction( GameConfig::EPlayerActions::EPA_Right );
}

// Destructor
CQTEHack::~CQTEHack()
{

}

// Adds a player action (up/down/left/right/jump/shoot/interact) to the "buttons" for this QTE
void CQTEHack::AddAction( GameConfig::EPlayerActions eAction )
{
	m_aeButtons->push_back( eAction );
}

void CQTEHack::PickNewKey()
{
	GCASSERT( m_aeButtons->size() > 0, "No buttons in QTE hackable" );

	m_eKeyToPress = m_aeButtons->at( rand() % m_aeButtons->size() );

	switch( m_eKeyToPress )
	{
	case GameConfig::EPlayerActions::EPA_Up:
		m_pPromptLabel->setString( "Up" );
		break;
	case GameConfig::EPlayerActions::EPA_Down:
		m_pPromptLabel->setString( "Down" );
		break;
	case GameConfig::EPlayerActions::EPA_Left:
		m_pPromptLabel->setString( "Left" );
		break;
	case GameConfig::EPlayerActions::EPA_Right:
		m_pPromptLabel->setString( "Right" );
		break;
	default:
		m_pPromptLabel->setString( "Uh Oh" );
		break;
	}

	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( -m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f ) );

}

void CQTEHack::InitUI()
{
}

// Hacking behavior for QTE
void CQTEHack::VUpdate( float fDeltaTime )
{
	if( GetTriggered() )
	{
		// Call start callback once
		if( !GetIsHacking() )
		{
			SetIsHacking( true );
			TriggerOnStartedCallback();
		}

		// Make sure there is only one hacking sfx is playing
		if( !m_bIsPlayingHackingSFX )
		{
			m_bIsPlayingHackingSFX = true;
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleHack, true );
		}

		//Add time to counter
		m_fPromptTime += fDeltaTime;

		// If the time the prompt has been up for exceeds the time allowed then fail
		if( m_fPromptTime > m_fTimePerButton )
		{
			VExit();
			HackFailed();

			m_pPromptLabel->setString( "Timed Out!" );

			cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( -m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f ) );

			VShowUI();
		}
		else
		{
			cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
			m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f - 60.f + ( ( m_fPromptTime / m_fTimePerButton ) * 50.0f ) ) );


			// This logic could go in PlayerInput, but I thought it best
			// to keep all mechanical logic in the Update function
			if( GetPlayerInput() == m_eKeyToPress )
			{
				//If the player has pressed the right key

				//set key pressed to null
				SetPlayerInput( GameConfig::EPlayerActions::EPA_NULL );

				//select new key
				PickNewKey();

				// Increment correct key count
				m_iEventsComplete++;

				//reset timer
				m_fPromptTime = 0.0f;
			}
			else if( GetPlayerInput() != GameConfig::EPlayerActions::EPA_NULL && GetPlayerInput() != GameConfig::EPlayerActions::EPA_Hack )
			{
				//If the player has pressed a key, but it wasn't the correct one, then fail

				VExit();
				HackFailed();

				m_pPromptLabel->setString( "Wrong Key!" );

				cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
				m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( -m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f ) );

				VShowUI();

				return;
			}

			//If the hacking is complete, then run succeeded code
			if( m_iEventsComplete == m_iNumberOfButtons )
			{
				m_pPromptLabel->setString( "Win!" );

				cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
				m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( -m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f ) );


				VExit();
				HackSucceeded();
				VShowUI();

			}
		}

	}
	else if( IsHacked() )
	{
		m_pPromptLabel->setString( "Hacked" );

		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		m_pPromptLabel->setPosition( cocos2d::Vec2( visibleSize.width / 2.f, visibleSize.height / 2.f ) - cocos2d::Vec2( m_pPromptLabel->getContentSize().width / 2.f, m_pPromptLabel->getContentSize().height / 2.f ) );

		VShowUI();

		// Stop hacking sfx when hack success
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_DanielleHack );
		m_bIsPlayingHackingSFX = false;
	}
	else
	{
		// Stop hacking sfx when hack fails
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_DanielleHack );
		m_bIsPlayingHackingSFX = false;
	}
}

// This lets the QTE obj know to start running
void CQTEHack::VTrigger()
{
	if( !GetTriggered() && !IsHacked() )
	{
		SetTriggered( true );

		VShowUI();

		//set key pressed to null
		SetPlayerInput( GameConfig::EPlayerActions::EPA_NULL );

		// Set correct key count to 0
		m_iEventsComplete = 0;

		//select new key
		PickNewKey();

		//reset timer
		m_fPromptTime = 0.0f;

		// hide hack label
		m_pHackLabel->setVisible( false );
	}
}

void CQTEHack::VEnter()
{
	if( !m_pActiveScene )
	{
		m_pActiveScene = IGCGameLayer::ActiveInstance();

		// Initialize hack label
		m_pPromptLabel = cocos2d::Label::createWithTTF( "", "fonts/deadman.ttf", 69 );
		m_pActiveScene->addChild( m_pPromptLabel, 1 );
		m_pPromptLabel->setTextColor( cocos2d::Color4B( 243, 156, 18, 255 ) );
		m_pPromptLabel->enableOutline( cocos2d::Color4B::BLACK, 1 );
		m_pPromptLabel->setVisible( false );

		// Initialize hack label
		m_pHackLabel = cocos2d::Label::createWithTTF( "Hack", "fonts/deadman.ttf", 24 );
		m_pActiveScene->addChild( m_pHackLabel, 1 );
		m_pHackLabel->setTextColor( cocos2d::Color4B( 243, 156, 18, 255 ) );
		m_pHackLabel->enableOutline( cocos2d::Color4B::BLACK, 1 );
		m_pHackLabel->setPosition( GetTerminal()->GetSpritePosition() + cocos2d::Vec2( 0.0f, 50.0f ) );

	}
	m_pHackLabel->setVisible( true );
}

// This lets the QTE obj know to stop running
void CQTEHack::VExit()
{
	//Hide UI at end
	VHideUI();

	// Set is hacking to false
	SetIsHacking( false );

	// This stops this code running when the player leaves the hackable after a successful hack
	if( !IsHacked() )
	{
		//Call parent, sets triggered to false
		CHackable::VExit();
	}
}

// Shows the UI
void CQTEHack::VShowUI()
{
	m_pHackLabel->setVisible( true );
	m_pPromptLabel->setVisible( true );
}

// Hides the UI
void CQTEHack::VHideUI()
{
	m_pPromptLabel->setVisible( false );
	m_pHackLabel->setVisible( false );
}

void CQTEHack::VReset()
{
	//Must call parent behavior first
	CHackable::VReset();
}
