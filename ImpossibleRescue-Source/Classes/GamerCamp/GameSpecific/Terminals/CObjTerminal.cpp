#ifndef _GCOBJTERMINAL_H_
#include "CObjTerminal.h"
#endif

#include "GamerCamp/GameSpecific/Hackables/CQTEHack.h"
#include "GamerCamp/GameSpecific/Hackables/CHackable.h"
#include "GamerCamp/GameSpecific/Hackables/CButtonHack.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/EventManager/CEventManager.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CObjTerminal::CObjTerminal()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjTerminal ) )
	, m_bIsKeyTerminal( false )
	, m_bIsTerminator( false )
	, m_bIsMover( false )
	, m_strHackType( "ButtonHack" )
	, m_fHackTime( 5.0f )
	, m_bIsResetable( false )
	, m_fTimePerButton( 1.0f )
	, m_iNumberOfEvents( 4 )
{
}

CObjTerminal::~CObjTerminal()
{
	delete m_CHackableInterface;
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjTerminal );

//////////////////////////////////////////////////////////////////////////
// Load assets Module 1 method
//////////////////////////////////////////////////////////////////////////
//virtual 
void CObjTerminal::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// ready for switch sprite
	// The terminal sprite frame name is <HACKING_TARGET>_Terminal_<TERMINAL_STATUS>
	// Until sprint 4, we have 3 different hacking targets and 3 terminal statuses
	// Hacking Target:
	// 1. Door
	// 2. Enemy
	// 3. Moving Platform / Platform
	// Terminal Status:
	// 1. Idle
	// 2. Hacking
	// 3. Hacked
	// In the OgmoEditor, we already have <HACKING_TARGET>_Terminal in the field called TerminalType

	std::string strDefaultAnimation = GetFactoryCreationParams()->strTerminalType + "_Idle";
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strDefaultAnimation, 1.0f ) ) );

	// create interface
	if( GetFactoryCreationParams()->strHackType.compare("ButtonHack") == 0 )
	{
		m_CHackableInterface = new CButtonHack( GetFactoryCreationParams()->fHackTime, GetFactoryCreationParams()->bIsHackResetable );
	}
	else if( GetFactoryCreationParams()->strHackType.compare( "QTE" ) == 0 )
	{
		CQTEHack* quickTimeEventHack = new CQTEHack();
		quickTimeEventHack->SetTimePerButton( GetFactoryCreationParams()->fHackTimePerButton );
		quickTimeEventHack->SetNumberOfEvents( GetFactoryCreationParams()->iHackNumberOfEvents );
		m_CHackableInterface = quickTimeEventHack;
	}
	else
	{
		m_CHackableInterface = new CButtonHack( 5.0f, true );
	}

	// Setup key terminal
	m_bIsKeyTerminal = GetFactoryCreationParams()->bIsKeyTerminal;
	m_bIsTerminator = GetFactoryCreationParams()->bIsTerminator;
	m_bIsMover = GetFactoryCreationParams()->bIsMover;

	// setup callbacks
	m_CHackableInterface->SetOnSucceedMethod( std::bind( &CObjTerminal::OnHackSucceed, this ) );
	m_CHackableInterface->SetOnFailMethod( std::bind( &CObjTerminal::OnHackFailed, this ) );
	m_CHackableInterface->SetOnStartedMethod( std::bind( &CObjTerminal::OnHackStarted, this ) );

	m_CHackableInterface->SetTerminal( this );
}

void CObjTerminal::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// Reset hacking progress
	m_CHackableInterface->VReset();

	// Restore terminal sprite animation to idle state
	StopAction( m_pCurrentAction );
	std::string strDefaultAnimation = GetFactoryCreationParams()->strTerminalType + "_Idle";
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strDefaultAnimation, 1.0f ) ) );
}

//////////////////////////////////////////////////////////////////////////
// Hackable interface accessor
//////////////////////////////////////////////////////////////////////////
void CObjTerminal::SetHackableInterface( CHackable* rCHackableInterface )
{
	m_CHackableInterface = rCHackableInterface;
}

CHackable& CObjTerminal::GetHackableInterface()
{
	GCASSERT( m_CHackableInterface, "Interface not implemented in this terminal!" );
	return *m_CHackableInterface;
}

//////////////////////////////////////////////////////////////////////////
// Hack callbacks
//////////////////////////////////////////////////////////////////////////
void CObjTerminal::OnHackStarted()
{
	// Change to hacking sprite
	StopAction( m_pCurrentAction );
	std::string strDefaultAnimation = GetFactoryCreationParams()->strTerminalType + "_Hacking";
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strDefaultAnimation, 1.0f ) ) );
}

void CObjTerminal::OnHackSucceed()
{
	// Event Manager reference
	CEventManager&	cEventManager = CEventManager::Manager();

	if( m_bIsKeyTerminal )
	{
		// If this is the key terminal, trigger gained objective event
		cEventManager.Trigger( GameConfig::EEventType::EET_GainedObjective, nullptr );
	}

	if( m_bIsTerminator )
	{
		// If this terminal is terminator, when it is hacked notify enemies to shutdown.
		cEventManager.Trigger( GameConfig::EEventType::EET_DeactivateEnemies, nullptr );
	}

	if( m_bIsMover )
	{
		// If this terminal is mover, when it is hacked activate moving platform.
		cEventManager.Trigger( GameConfig::EEventType::EET_ActivateMovingPlatform, nullptr );
	}

	// Notify this terminal has been hacked
	cEventManager.Trigger( GameConfig::EEventType::EET_HackSucceed, nullptr );

	// Change to hacked sprite
	StopAction( m_pCurrentAction );
	std::string strDefaultAnimation = GetFactoryCreationParams()->strTerminalType + "_Hacked";
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strDefaultAnimation, 1.0f ) ) );
}

void CObjTerminal::OnHackFailed()
{
	// Event Manager reference
	CEventManager&	cEventManager = CEventManager::Manager();

	// notify all player is failed on hacking this terminal
	cEventManager.Trigger( GameConfig::EEventType::EET_HackFail, nullptr );

	// Change to idle sprite
	StopAction( m_pCurrentAction );
	std::string strDefaultAnimation = GetFactoryCreationParams()->strTerminalType + "_Idle";
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strDefaultAnimation, 1.0f ) ) );
}