#ifndef _GCOBJPLATFORMS_H_
#include "CEnemyR3.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CEnemyR3::CEnemyR3( void )
	: CGCObjSpritePhysics( GetGCTypeIDOf( CEnemyR3 ) )
	, m_fHealth( GameConfig::k_fR3Health )
	, m_pcProjectileManager( nullptr )
	, m_fFireRate( GameConfig::k_fR3FireRate )
	, m_fTimeUntilCanFire( 0 )
	, m_fGravityScale( 2.0f )
	, m_fDirection( 1.0f )
	, m_fWalkDrag( GameConfig::k_fR3WalkDrag )
	, m_fWalkAcceleration( GameConfig::k_fR3WalkForce )
	, m_fJumpDrag( 50.0f )
	, m_bCanJump( true )
	, m_bJumping( false )
	, m_iJumpedFrames( 0 )
	, m_iMaxJumpingFrames( 25 )
	, m_fJumpAcceleration( 10000.0f )
	, m_fJumpAbility( 0.0f )
	, m_fFireRange( 300.0f )
	, m_bIsFullyDeactivated( false )
	, m_fImmobilisedTime( 0.0f )
{
}

CEnemyR3::~CEnemyR3()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CEnemyR3 );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
void CEnemyR3::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_marioJog = "R3_Idle";

	// animate!
	cocos2d::ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_marioJog ) ) );

	m_pcProjectileManager = static_cast< CGCObjGroupProjectile* >
		( CGCObjectManager::FindObjectGroupByID( GetGCTypeIDOf( CGCObjGroupProjectile ) ) );

	// Read ogmo editor properties
	// - direction
	m_fDirection *= GetFactoryCreationParams()->bIsStartFlipped ? -1 : 1;
	SetFlippedX( GetFactoryCreationParams()->bIsStartFlipped );
	// - attack range
	m_fFireRange = GetFactoryCreationParams()->fEnemyAttackRange;

}

void CEnemyR3::VOnResurrected()
{
	CGCObjSpritePhysics::VOnResurrected();
	// Hotfix for fixing rotation
	GetPhysicsBody()->SetFixedRotation( true );
	GetPhysicsBody()->SetGravityScale( m_fGravityScale );
}

void CEnemyR3::VOnUpdate( float fTimeStep )
{

	// If the enemy is not indefinitely immobilized then decrement the amount of immobilised time remaining
	if( m_fImmobilisedTime > -1.f )
		m_fImmobilisedTime = std::max( 0.0f, m_fImmobilisedTime - fTimeStep );

	// Activated only if the time left to be immobilised is 0
	m_bIsFullyDeactivated = m_fImmobilisedTime != 0.0f;

	if( !m_bIsFullyDeactivated )
	{
		// Raycast setup
		b2Vec2 v2Origin = GetPhysicsBody()->GetWorldCenter();
		b2Vec2 v2Direction = b2Vec2( m_fDirection, 0 );
		b2Vec2 v2Target = v2Direction;
		v2Target *= m_fFireRange / IGCGAMELAYER_B2D_PIXELS_PER_METER;

		v2Target = v2Origin + v2Target;

		// Raycast shoot
		GetPhysicsBody()->GetWorld()->RayCast( this, v2Origin, v2Target );

		FireUpdate( fTimeStep );
		MovementUpdate();
	}

}

void CEnemyR3::VOnReset()
{
	m_fHealth = GameConfig::k_fR3Health;
	CGCObjSpritePhysics::VOnReset();
	m_fTimeUntilCanFire = 0;
}

//////////////////////////////////////////////////////////////////////////
// b2RayCastCallback virtual method
//////////////////////////////////////////////////////////////////////////
float32 CEnemyR3::ReportFixture( b2Fixture * fixture, const b2Vec2 & point, const b2Vec2 & normal, float32 fraction )
{
	// Get the hit object user data from b2 fixture
	CGCObjSpritePhysics * pGcSprPhys = ( CGCObjSpritePhysics* )fixture->GetBody()->GetUserData();

	// If it is not a CGCObjSpritePhysics, we don't handle it
	if( pGcSprPhys == NULL )
	{
		return NULL;
	}

	// Try to safe cast to CGCObjPlayer
	CGCObjPlayer* pPlayer = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhys );

	// If it is player...
	if( pPlayer )
	{
		// Enemy is not in cool down for firing
		if( m_fTimeUntilCanFire <= 0.0f )
		{
			// Fire projectile
			m_pcProjectileManager->Fire(
				GameConfig::EWeaponType::EWT_R3,
				GetSpritePosition() + cocos2d::Vec2( -70.0f + ( IsFlippedX() * 140 ), -32.0f ),
				cocos2d::Vec2( -1.0f + ( IsFlippedX() * 2 ), 0.0f ),
				100.0f
			);

			// Increase the cool down timer
			m_fTimeUntilCanFire = m_fFireRate;
		}
	}

	return float32();
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
void CEnemyR3::SetFireRate( float fFirerate )
{
	m_fFireRate = fFirerate;
}

void CEnemyR3::SetJumpAbility( float fJumpAbility )
{
	m_fJumpAbility = fJumpAbility;
}

float CEnemyR3::GetJumpAbility()
{
	return m_fJumpAbility;
}

float CEnemyR3::Damage( float fDamage )
{
	return m_fHealth -= fDamage;
}

//////////////////////////////////////////////////////////////////////////
// Methods for Node Handling
//////////////////////////////////////////////////////////////////////////
void CEnemyR3::SwitchDirection()
{
	SetVelocity( cocos2d::Vec2::ZERO );
	m_fDirection *= -1;
}

void CEnemyR3::Jump()
{
	m_bCanJump = true;
	m_bJumping = true;
	m_iJumpedFrames = 0;
}

void CEnemyR3::Jump( float fRequestedJumpAbility )
{
	if( m_fJumpAbility > fRequestedJumpAbility )
	{
		Jump();
	}
}

//////////////////////////////////////////////////////////////////////////
// Update Methods
//////////////////////////////////////////////////////////////////////////
void CEnemyR3::MovementUpdate()
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

void CEnemyR3::FireUpdate( float fTimeStep )
{
	// Decrease the shooting cool down timer
	if( m_fTimeUntilCanFire > 0.0f )
	{
		m_fTimeUntilCanFire -= fTimeStep;
	}
}

//////////////////////////////////////////////////////////////////////////
// Immobilise
//////////////////////////////////////////////////////////////////////////
void CEnemyR3::Immobilise( float fTime /*= -1.0f*/ )
{
	SetVelocity( cocos2d::Vec2::ZERO );
	m_fImmobilisedTime = fTime;
}