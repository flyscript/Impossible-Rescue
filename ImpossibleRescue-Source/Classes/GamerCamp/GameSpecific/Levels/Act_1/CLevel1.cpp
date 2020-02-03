//Includes
#include "CLevel2.h"

// Include own .h
#include "CLevel1.h"


//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CLevel1::CLevel1()
	: CLevelBase(
		GetGCTypeIDOf( CLevel1 ),							// GC Type
		std::string( "OgmoEditor/Act_1_Level_1.oel" ),		// Ogmo Level
		"Loose/Backgrounds/Act1/Level_1/00.png",			// Background
		0,													// Level Goals
		1
	)
{
}

CLevel1::~CLevel1()
{
}

//////////////////////////////////////////////////////////////////////////
// CCNode Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CLevel1::onEnter()
{
	// Call base class first
	CLevelBase::onEnter();
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel1::VOnCreate()
{
	m_pcDataManager->SaveLives( GameConfig::k_iPlayerLives );
	m_pcDataManager->SaveHealth( GameConfig::k_iPlayerMaxHealth );
	// Call base class first
	CLevelBase::VOnCreate();

	// Level Label
	m_pLevelLabel->setString( "Level 1-1" );
}

void CLevel1::VOnUpdate( f32 fTimeStep )
{
	// Call base class first
	CLevelBase::VOnUpdate( fTimeStep );
}

void CLevel1::VOnDestroy()
{
	// Call base class first
	CLevelBase::VOnDestroy();
}

void CLevel1::VOnReset()
{
	// Call base class first
	CLevelBase::VOnReset();
}

//////////////////////////////////////////////////////////////////////////
// B2ContactListener Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel1::BeginContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::BeginContact( pB2Contact );
}

void CLevel1::EndContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::EndContact( pB2Contact );
}

void CLevel1::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	// Call base class first
	CLevelBase::PreSolve( pB2Contact, pOldManifold );
}

void CLevel1::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
	// Call base class first
	CLevelBase::PostSolve( pB2Contact, pImpulse );
}

//////////////////////////////////////////////////////////////////////////
// CLevelBase Virtual Method
//////////////////////////////////////////////////////////////////////////
void CLevel1::NextLevel()
{
	m_pcDataManager->SaveLevel( 2 );
	ReplaceScene( TGCGameLayerSceneCreator< CLevel2 >::CreateScene() );
}

void CLevel1::PlayBackgroundMusic()
{
	// Play background music
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Level1, true );
}
