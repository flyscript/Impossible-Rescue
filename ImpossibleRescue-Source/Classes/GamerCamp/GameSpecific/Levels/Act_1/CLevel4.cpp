//Includes
#include "GamerCamp/GameSpecific/Levels/Act_2/CCutsceneAct2.h"
#include "GamerCamp/GameSpecific/Levels/Act_3/COutroScene3.h"
#include <functional>

// Include own .h
#include "CLevel4.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CLevel4::CLevel4()
	: CLevelBase(
		GetGCTypeIDOf( CLevel4 ),									// GC Type
		std::string( "OgmoEditor/Act_1_Level_4.oel" ),				// Ogmo Level
		"Loose/Backgrounds/Act1/Level_4/00.png",					// Background
		1,															// Level Goals
		4,
		60.0f														// Level Time
	)
	, m_bIsBarrierActivated( true )
{
}

CLevel4::~CLevel4()
{
}

//////////////////////////////////////////////////////////////////////////
// CCNode Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CLevel4::onEnter()
{
	// Call base class first
	CLevelBase::onEnter();
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel4::VOnCreate()
{
	// Call base class first
	CLevelBase::VOnCreate();

	// Add Barrier
	// - Barrier starting position
	cocos2d::Vec2 v2BarrierStartPos( 1580.0f, -150.0f );

	// - Barrier factory creation parameters
	CGCFactoryCreationParams& sParams = m_sBarrierCreateParams;
	sParams.strClassName = "CObjBarrier";
	sParams.strPlistFile = "TexturePacker/Sprites/Barrier/Barrier.plist";
	sParams.strPhysicsShape = "barrier";
	sParams.eB2dBody_BodyType = b2_kinematicBody;
	sParams.bB2dBody_FixedRotation = true;

	// Create player object
	m_pcGCOBarrier = static_cast< CObjBarrier* >( CGCFactory_ObjSpritePhysics::GetFactory().CreateInstance( sParams, v2BarrierStartPos ) );

	// Level Label
	m_pLevelLabel->setString( "Level 1-4" );
}

void CLevel4::VOnUpdate( f32 fTimeStep )
{
	// Call base class first
	CLevelBase::VOnUpdate( fTimeStep );

	if( m_bIsBarrierActivated  && m_bTutorialIsSkipped )
	{
		UpdateBarrier();
	}
}

void CLevel4::VOnDestroy()
{
	// Call base class first
	CLevelBase::VOnDestroy();
}

void CLevel4::VOnReset()
{
	m_bIsBarrierActivated = true;

	// Call base class first
	CLevelBase::VOnReset();
}

//////////////////////////////////////////////////////////////////////////
// B2ContactListener Virtual Methods Override from Parent
//////////////////////////////////////////////////////////////////////////
void CLevel4::BeginContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::BeginContact( pB2Contact );
}

void CLevel4::EndContact( b2Contact* pB2Contact )
{
	// Call base class first
	CLevelBase::EndContact( pB2Contact );
}

void CLevel4::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	// Call base class first
	CLevelBase::PreSolve( pB2Contact, pOldManifold );
}

void CLevel4::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
	// Call base class first
	CLevelBase::PostSolve( pB2Contact, pImpulse );
}

//////////////////////////////////////////////////////////////////////////
// CLevelBase Virtual Method
//////////////////////////////////////////////////////////////////////////
void CLevel4::PlayBackgroundMusic()
{
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Level4, false );
}

void CLevel4::NextLevel()
{
	// We don't have level 5 right now, but if we save level 1 as before.
	// The level 1 cutscene will not show up when player presses "Continue" at the main menu scene.
	m_pcDataManager->SaveLevel( 5 );

	ReplaceScene( TGCGameLayerSceneCreator< COutroScene3 >::CreateScene() );
}

// Method to update the position of the barrier
void CLevel4::UpdateBarrier()
{
	// Variable to store the pixels to meters ratio
	f32 fPTM_Ratio = IGCGAMELAYER_B2D_PIXELS_PER_METER;

	// Variable to store the distance barrier has to move in pixels 
	f32 fMoveDistanceinPixels = 180.0f;
	// Variable to store the distance barrier has to move in meters 
	f32 fMoveDistanceinMeters;
	// Variable to store the velocity of the barrier.
	f32 fVelocity;
	// Variable to store the time barrier has to move
	f32 fTime;

	//Calculating the velocity of the barrier from 0 seconds to half the total time.
	if( m_fLevelTimeElapsed <= m_fLevelTime / 3 )
	{
		//m_pcGCOBarrier->SetSpritePosition( Vec2( m_pcGCOBarrier->GetSpritePosition().x , 160*(m_fLevelTimeElapsed * 2 / m_fLevelTime)- 150.0f ));
		fMoveDistanceinMeters = fMoveDistanceinPixels / fPTM_Ratio;
		fTime = m_fLevelTime / 2;
		fVelocity = fMoveDistanceinMeters / fTime;


	}
	//Calculating the velocity of the barrier in the last second of the level.
	else if( ( int )m_fLevelTimeElapsed == ( ( int )m_fLevelTime ) - 1 )
	{
		f32 fMoveDistanceinPixels = 160.0f;
		fMoveDistanceinMeters = fMoveDistanceinPixels / fPTM_Ratio;
		fTime = 1;
		fVelocity = fMoveDistanceinMeters / fTime;
		//m_pcGCOBarrier->SetSpritePosition( Vec2( 1580.0f , 185.0f ) );

	}
	// Setting the velocity to 0 in other cases.
	else
	{
		//m_pcGCOBarrier->SetSpritePosition( Vec2( 1580.0f , 10.0f ) );
		fVelocity = 0.0f;
	}

	m_pcGCOBarrier->SetVelocity( cocos2d::Vec2( 0.0f, fVelocity ) );
}