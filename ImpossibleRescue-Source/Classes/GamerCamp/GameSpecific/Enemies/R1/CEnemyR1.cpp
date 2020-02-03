#ifndef _GCENEMYR1_H_
#include "CEnemyR1.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/EventManager/CEventManager.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"


//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CEnemyR1::CEnemyR1()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CEnemyR1 ) )
	, m_fHealth( GameConfig::k_fR1Health )
	, m_fGravityScale( 2.0f )
	, m_fDirection( 1.0f )
	, m_fWalkDrag( GameConfig::k_fR1WalkDrag )
	, m_fWalkAcceleration( GameConfig::k_fR1WalkForce )
	, m_fJumpDrag( 50.0f )
	, m_bCanJump( true )
	, m_bJumping( false )
	, m_iJumpedFrames( 0 )
	, m_iMaxJumpingFrames( 25 )
	, m_fJumpAcceleration( 7250.0f )
	, m_fJumpAbility( 0.0f )
	, m_bIsFullyDeactivated( false )
	, m_fImmobilisedTime( 0.0f )
{
}

CEnemyR1::~CEnemyR1()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CEnemyR1 );

//////////////////////////////////////////////////////////////////////////
// Movement related methods
//////////////////////////////////////////////////////////////////////////
void CEnemyR1::SwitchDirection()
{
	SetVelocity( cocos2d::Vec2::ZERO );
	m_fDirection *= -1;
}

void CEnemyR1::Jump()
{
	m_bCanJump = true;
	m_bJumping = true;
	m_iJumpedFrames = 0;
}

void CEnemyR1::Jump( float fRequestedJumpAbility )
{
	if( m_fJumpAbility > fRequestedJumpAbility )
	{
		Jump();
	}
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
void CEnemyR1::SetJumpAbility( float fJumpAbility )
{
	m_fJumpAbility = fJumpAbility;
}

float CEnemyR1::GetJumpAbility()
{
	return m_fJumpAbility;
}

float CEnemyR1::Damage( float fDamage )
{
	return m_fHealth -= fDamage;
}

//////////////////////////////////////////////////////////////////////////
// Overrode CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
// IN_CPP_CREATION_PARAMS_DECLARE( CGCEnemyR1, "TexturePacker/Sprites/Enemies/Enemies.plist", "enemy_r1", b2_dynamicBody, true );

//virtual 
void CEnemyR1::VOnResourceAcquire()
{
	// IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCEnemyR1 );
	CGCObjSpritePhysics::VOnResourceAcquire();

	// apply the target sprite
	const char* chDefaultAnimation = "R1_Idle";
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation, 10.0f ) ) );

	// read OgmoEditor properties
	m_bIsMovementActivated = GetFactoryCreationParams()->bIsMovmentActivated;
	m_fDirection *= GetFactoryCreationParams()->bIsStartFlipped ? -1 : 1;
}

void CEnemyR1::VOnResurrected( void )
{
	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetFixedRotation( true );
	GetPhysicsBody()->SetGravityScale( m_fGravityScale );


	// subscribe events
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_DeactivateEnemies, [ & ] ( void* )
	{
		Immobilise();
	} );
}

void CEnemyR1::VOnUpdate( f32 fTimeStep )
{
	// If the enemy is not indefinitely immobilized then decrement the amount of immobilised time remaining
	if( m_fImmobilisedTime > -1.f )
	{
		m_fImmobilisedTime = std::max( 0.0f, m_fImmobilisedTime - fTimeStep );
	}

	// Activated only if the time left to be immobilised is 0
	m_bIsFullyDeactivated = m_fImmobilisedTime != 0.0f;

	if( !m_bIsFullyDeactivated && m_bIsMovementActivated )
	{
		cocos2d::Vec2 vTotalForce = cocos2d::Vec2::ZERO;
		cocos2d::Vec2 vDirectionalForce = cocos2d::Vec2::ZERO;
		cocos2d::Vec2 vDragForce = cocos2d::Vec2::ZERO;
		cocos2d::Vec2 vVelocity = GetVelocity();

		// Walk
		vDirectionalForce.x = m_fWalkAcceleration * m_fDirection;

		// Jump
		if( m_bJumping && m_iJumpedFrames < m_iMaxJumpingFrames )
		{
			vDirectionalForce.y = m_fJumpAcceleration;
			m_iJumpedFrames++;
		}
		else
		{
			m_bJumping = false;
		}

		// Drag
		vDragForce = cocos2d::Vec2( vVelocity.x * m_fWalkDrag, vVelocity.y * m_fJumpDrag );

		// Apply force
		vTotalForce = vDirectionalForce - vDragForce;
		ApplyForceToCenter( vTotalForce );

		// Face the correct direction
		SetFlippedX( GetVelocity().x > 0 );
	}
	else
	{
		SetVelocity( cocos2d::Vec2::ZERO );
	}
}

void CEnemyR1::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_fHealth = GameConfig::k_fR1Health;
	m_bIsFullyDeactivated = false;
	m_fImmobilisedTime = 0.0f;
	m_fDirection = GetFactoryCreationParams()->bIsStartFlipped ? -1 : 1;
}


//////////////////////////////////////////////////////////////////////////
// Immobilise
//////////////////////////////////////////////////////////////////////////
void CEnemyR1::Immobilise( float fTime /*= -1.0f*/ )
{
	SetVelocity( cocos2d::Vec2::ZERO );
	m_fImmobilisedTime = fTime;
}