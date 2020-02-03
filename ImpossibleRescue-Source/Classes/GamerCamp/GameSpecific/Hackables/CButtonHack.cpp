#include "cocos2d.h"

#include "CHackable.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameSpecific/UI/UIAnimatedBar.h"

#include "Helper/GameConfig.h"

#include "CButtonHack.h"

//////////////////////////////////////////////////////////////////////////
// Constructors and Destructor
//////////////////////////////////////////////////////////////////////////
CButtonHack::CButtonHack()
	: m_bResetable( true )
	, m_fMaxHackTime( 5.0f )
	, m_fHackProgress( 0.0f )
	, m_pActiveScene( nullptr )
	, m_pHackingProgressBar( nullptr )
{
	// Init UI
	InitUI();
	SetCanMoveWhilstHacking( true );
}

CButtonHack::CButtonHack( float fMaxHackTime, bool bResetable )
	: m_bResetable( bResetable )
	, m_fMaxHackTime( fMaxHackTime )
	, m_fHackProgress( 0.0f )
	, m_pActiveScene( nullptr )
	, m_pHackingProgressBar( nullptr )
{
	// Init UI
	InitUI();

	SetCanMoveWhilstHacking( true );
}

CButtonHack::~CButtonHack()
{

}

//////////////////////////////////////////////////////////////////////////
// Overriding the parent methods - State methods
//////////////////////////////////////////////////////////////////////////
void CButtonHack::VUpdate( float fDeltaTime )
{
	// If the hackable object is triggered and we are hacking it
	if( GetTriggered() && GetPlayerInput() == GameConfig::EPlayerActions::EPA_Hack )
	{
		// Call start callback once
		if( !GetIsHacking() )
		{
			SetIsHacking( true );
			TriggerOnStartedCallback();
		}

		// Make sure only once sfx is playing
		if( !m_bIsPlayingHackingSFX )
		{
			// Play audio effect
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleHack, true );
			m_bIsPlayingHackingSFX = true;
		}

		// Increase the progress by delta time
		m_fHackProgress += fDeltaTime;

		// If the progress is equal or greater than the max hack time
		if( m_fHackProgress >= m_fMaxHackTime )
		{
			CCLOG( "Finished Hacking" );

			// Hide UI
			VExit();

			// Call callback function to notify outside
			HackSucceeded();
		}
	}
	else if( GetTriggered() && GetPlayerInput() == GameConfig::EPlayerActions::EPA_NULL )
	{
		if( m_bIsPlayingHackingSFX )
		{
			// Stop audio effect
			m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_DanielleHack );
			m_bIsPlayingHackingSFX = false;
		}
	}

	// Refresh the UI content for every frame
	UpdateUI();
}

void CButtonHack::VTrigger()
{
	// If the current hackable object is not triggered and is not hacked
	if( !GetTriggered() && !IsHacked() )
	{
		// Set Triggered flag
		SetTriggered( true );

		// Show loading bar
		VShowUI();
	}
}

void CButtonHack::VEnter()
{
	VShowUI();
}

void CButtonHack::VExit()
{
	SetIsHacking( false );

	// This stops this code running when the player leaves the hackable after a successful hack
	if( !IsHacked() && GetTriggered() )
	{

		// Reset hacking progress if needed
		if( m_bResetable && m_fHackProgress < m_fMaxHackTime )
		{
			m_fHackProgress = 0.0f;
			m_pHackingProgressBar->SetValueWithoutAnimation( m_fHackProgress );
			HackFailed();
		}
	}


	if( m_bIsPlayingHackingSFX )
	{
		// Stop audio effect
		m_pcAudioManager->StopEffectsMusic( GameConfig::EMusic::EM_DanielleHack );
		m_bIsPlayingHackingSFX = false;
	}

	//Hide UI at end
	VHideUI();

	//Call parent, sets triggered to false
	CHackable::VExit();
}

void CButtonHack::VReset()
{
	//Must call parent behavior first
	CHackable::VReset();
	m_fHackProgress = 0;
	m_pHackingProgressBar->SetValueWithoutAnimation( m_fHackProgress );
	m_pHackLabel->setString( "Hack" );
}

void CButtonHack::InitUI()
{
	if( !m_pActiveScene )
	{
		m_pActiveScene = IGCGameLayer::ActiveInstance();

		// Get useful information from the scene
		cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

		// Initialize hack label
		m_pHackLabel = cocos2d::Label::createWithTTF( "Hack", "fonts/deadman.ttf", 24 );
		m_pActiveScene->addChild( m_pHackLabel, 1 );
		m_pHackLabel->setTextColor( cocos2d::Color4B( 243, 156, 18, 255 ) );
		m_pHackLabel->enableOutline( cocos2d::Color4B::BLACK, 1 );
		m_pHackLabel->setVisible( false );

		// initialize hack progress bar
		m_pHackingProgressBar = UIAnimatedBar::create( "Loose/UI/Bars/loadingBar.png", 0.0f );
		m_pActiveScene->addChild( m_pHackingProgressBar, 1 );
		m_pHackingProgressBar->SetMaxValue( m_fMaxHackTime );
		m_pHackingProgressBar->SetVisible( false );
	}
}

//////////////////////////////////////////////////////////////////////////
// Overriding the parent methods - UI methods
//////////////////////////////////////////////////////////////////////////
void CButtonHack::UpdateUI()
{
	// Update the loading bar progress by calculating the progress in percentage
	m_pHackingProgressBar->SetValue( m_fHackProgress );
	if( IsHacked() )
	{
		m_pHackLabel->setString( "Hacked" );
	}
}

void CButtonHack::VShowUI()
{
	// Show progress bar
	if( m_pHackingProgressBar )
	{
		if( !IsHacked() )
		{
			m_pHackingProgressBar->SetVisible( true );
		}
		m_pHackLabel->setVisible( true );

		// Set loading bar position
		m_pHackingProgressBar->SetPosition( GetTerminal()->GetSpritePosition() + cocos2d::Vec2( 0.0f, 70.0f ) );

		// Set "hack" to the correct position
		m_pHackLabel->setPosition( GetTerminal()->GetSpritePosition() + cocos2d::Vec2( 0.0f, 50.0f ) );

	}

}

void CButtonHack::VHideUI()
{
	// Hide progress bar
	if( m_pHackingProgressBar )
	{
		m_pHackLabel->setVisible( false );
		m_pHackingProgressBar->SetVisible( false );
	}
}
