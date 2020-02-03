//Includes
#include "CLevel3.h"

// Include own .h
#include "CLevel2.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CLevel2::CLevel2()
	: CLevelBase(
		GetGCTypeIDOf( CLevel2 ),							// GC Type
		std::string( "OgmoEditor/Act_1_Level_2.oel" ),		// Ogmo Level
		"Loose/Backgrounds/Act1/Level_2/00.png",			// Background
		1,													// Level Goals
		2
	)
{
}

CLevel2::~CLevel2()
{
}

//////////////////////////////////////////////////////////////////////////
// CCNode Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CLevel2::onEnter()
{
	// Call base class first
	CLevelBase::onEnter();

}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel2::VOnCreate()
{
	// Call base class first
	CLevelBase::VOnCreate();

	// Level Label
	m_pLevelLabel->setString( "Level 1-2" );
}

void CLevel2::VOnUpdate( f32 fTimeStep )
{
	// Call base class first
	CLevelBase::VOnUpdate( fTimeStep );
}

void CLevel2::VOnDestroy()
{
	// Call base class first
	CLevelBase::VOnDestroy();
}

void CLevel2::VOnReset()
{
	// Call base class first
	CLevelBase::VOnReset();
}

//////////////////////////////////////////////////////////////////////////
// B2ContactListener Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel2::BeginContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::BeginContact( pB2Contact );
}

void CLevel2::EndContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::EndContact( pB2Contact );
}

void CLevel2::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	// Call base class first
	CLevelBase::PreSolve( pB2Contact, pOldManifold );
}

void CLevel2::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
	// Call base class first
	CLevelBase::PostSolve( pB2Contact, pImpulse );
}

//////////////////////////////////////////////////////////////////////////
// CLevelBase Virtual Method
//////////////////////////////////////////////////////////////////////////
void CLevel2::PlayBackgroundMusic()
{
	// Play background music
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Level2 , true );
}

void CLevel2::NextLevel()
{
	m_pcDataManager->SaveLevel( 3 );
	ReplaceScene( TGCGameLayerSceneCreator< CLevel3 >::CreateScene() );
}