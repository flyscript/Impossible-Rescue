#include "CHackable.h"

#include "cocos2d.h"

//Constructor
CHackable::CHackable()
	: m_pfnCallbackOnFailed( nullptr )
	, m_pfnCallbackOnSucceed( nullptr )
	, m_bTriggered( false )
	, m_bHacked( false )
	, m_bIsHacking( false )
	, m_bCanMoveWhileHacking( false )
	, m_ePlayerInput( GameConfig::EPlayerActions::EPA_NULL )
	, m_bIsPlayingHackingSFX( false )
{
	m_pcAudioManager = new CAudioManager();
}

//Destructor
CHackable::~CHackable()
{

}

//Points this CHackable to which method to run if a hack fails.
void CHackable::SetOnFailMethod( std::function<void()> pfnCallback )
{
	m_pfnCallbackOnFailed = pfnCallback;
}

//Points this CHackable to which method to run if a hack succeeds.
void CHackable::SetOnSucceedMethod( std::function<void()> pfnCallback )
{
	m_pfnCallbackOnSucceed = pfnCallback;
}

void CHackable::SetOnStartedMethod( std::function<void()> pfnCallback )
{
	m_pfnCallbackOnStarted = pfnCallback;
}

// Set current player input
void CHackable::SetPlayerInput( GameConfig::EPlayerActions eAction )
{
	m_ePlayerInput = eAction;
}

// Return current player input
GameConfig::EPlayerActions CHackable::GetPlayerInput()
{
	return m_ePlayerInput;
}


void CHackable::VUpdate( float fDeltaTime )
{

}

void CHackable::VTrigger()
{

}

void CHackable::VReset()
{
	m_bHacked = false;
}

void CHackable::VEnter()
{

}

//Default behavior for Exit, can be expanded by inherited classes
void CHackable::VExit()
{
	m_bTriggered = false;
}



void CHackable::VShowUI()
{

}

void CHackable::VHideUI()
{

}

//Called by inherited classes when a hack succeeds. Saves exposing m_pfnCallbackOnSucceed for the sake of encapsulation.
void CHackable::HackSucceeded()
{
	m_bHacked = true;

	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleHackPass, false );

	if( m_pfnCallbackOnSucceed )
	{
		m_pfnCallbackOnSucceed();
	}
	else
	{
#if _DEBUG
		CCLOG( "No callback method for succeed :(" );
#endif
	}
}

//Called by inherited classes when a hack fails. Saves exposing m_pfnCallbackOnFailed for the sake of encapsulation.
void CHackable::HackFailed()
{

	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleHackFail, false );

	if( m_pfnCallbackOnFailed )
	{
		m_pfnCallbackOnFailed();
	}
	else
	{
#if _DEBUG
		CCLOG( "No callback method for failure :(" );
#endif
	}
}