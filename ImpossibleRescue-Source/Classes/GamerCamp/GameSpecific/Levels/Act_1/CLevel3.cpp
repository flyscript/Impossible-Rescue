//Includes
#include "CLevel4.h"

// Include own .h
#include "CLevel3.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CLevel3::CLevel3()
	: CLevelBase(
		GetGCTypeIDOf( CLevel3 ),							// GC Type
		std::string( "OgmoEditor/Act_1_Level_3.oel" ),		// Ogmo Level
		"Loose/Backgrounds/Act1/Level_3/00.png",			// Background
		1,													// Level Goals
		3
	)
{
}

CLevel3::~CLevel3()
{
}

//////////////////////////////////////////////////////////////////////////
// CCNode Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CLevel3::onEnter()
{
	// Call base class first
	CLevelBase::onEnter();
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel3::VOnCreate()
{
	// Call base class first
	CLevelBase::VOnCreate();

	// Level Label
	m_pLevelLabel->setString( "Level 1-3" );
}

void CLevel3::VOnUpdate( f32 fTimeStep )
{
	// Call base class first
	CLevelBase::VOnUpdate( fTimeStep );
}

void CLevel3::VOnDestroy()
{
	// Call base class first
	CLevelBase::VOnDestroy();
}

void CLevel3::VOnReset()
{
	// Call base class first
	CLevelBase::VOnReset();
}

//////////////////////////////////////////////////////////////////////////
// B2ContactListener Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel3::BeginContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::BeginContact( pB2Contact );
}

void CLevel3::EndContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::EndContact( pB2Contact );
}

void CLevel3::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	// Call base class first
	CLevelBase::PreSolve( pB2Contact, pOldManifold );
}

void CLevel3::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
	// Call base class first
	CLevelBase::PostSolve( pB2Contact, pImpulse );
}

//////////////////////////////////////////////////////////////////////////
// CLevelBase Virtual Method
//////////////////////////////////////////////////////////////////////////
void CLevel3::PlayBackgroundMusic()
{
	// Play background music
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Level3 , true );
}

void CLevel3::NextLevel()
{
	m_pcDataManager->SaveLevel( 4 );
	ReplaceScene( TGCGameLayerSceneCreator< CLevel4 >::CreateScene() );
}