#ifndef _GCENEMYR2_H_
#include "CEnemyR2.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "GamerCamp/GameSpecific/Player/GCObjPlayer.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CEnemyR2::CEnemyR2()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CEnemyR2 ) )
	, m_fHealth( GameConfig::k_fR2Health )
	, m_pcProjectileManager( nullptr )
	, m_fFireRate( GameConfig::k_fR2FireRate )
	, m_fTimeUntilCanFire( 0 )
	, m_fFireRange( 400.0f )
	, m_iDirection( -1 )
	, m_bIsFullyDeactivated( false )
	, m_fImmobilisedTime( 0.0f )
{
}

CEnemyR2::~CEnemyR2()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CEnemyR2 );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
void CEnemyR2::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_marioJog = "R2_Idle";

	// animate!
	cocos2d::ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_marioJog ) ) );

	m_pcProjectileManager = static_cast< CGCObjGroupProjectile* >
		( CGCObjectManager::FindObjectGroupByID( GetGCTypeIDOf( CGCObjGroupProjectile ) ) );

	// Setup from the ogmo editor properties
	// - direction
	m_iDirection = GetFactoryCreationParams()->bIsStartFlipped ? 1 : -1;
	SetFlippedX( GetFactoryCreationParams()->bIsStartFlipped );
	// - attack range
	m_fFireRange = GetFactoryCreationParams()->fEnemyAttackRange;
}

void CEnemyR2::VOnUpdate( float fTimeStep )
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
		b2Vec2 v2Direction = b2Vec2( m_iDirection, 0 );
		b2Vec2 v2Target = v2Direction;
		v2Target *= m_fFireRange / IGCGAMELAYER_B2D_PIXELS_PER_METER;

		v2Target = v2Origin + v2Target;

		// Raycast shoot
		GetPhysicsBody()->GetWorld()->RayCast( this, v2Origin, v2Target );

		// Decrease the shooting cool down timer
		if( m_fTimeUntilCanFire > 0.0f )
		{
			m_fTimeUntilCanFire -= fTimeStep;
		}
	}
}

void CEnemyR2::VOnResurrected()
{
	m_fHealth = GameConfig::k_fR2Health;
	CGCObjSpritePhysics::VOnResurrected();
	// Hotfix for fixing rotation
	GetPhysicsBody()->SetFixedRotation( true );

}

void CEnemyR2::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_fTimeUntilCanFire = 0;
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
void CEnemyR2::SetFireRate( float fFirerate )
{
	m_fFireRate = fFirerate;
}

float CEnemyR2::Damage( float fDamage )
{
	return m_fHealth -= fDamage;
}

//////////////////////////////////////////////////////////////////////////
// Immobilise
//////////////////////////////////////////////////////////////////////////
void CEnemyR2::Immobilise( float fTime /*= -1.0f*/ )
{
	SetVelocity( cocos2d::Vec2::ZERO );
	m_fImmobilisedTime = fTime;
}

//////////////////////////////////////////////////////////////////////////
// Box2D Raycast virtual method for callback
//////////////////////////////////////////////////////////////////////////
float32 CEnemyR2::ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction )
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

	// If it is player and no cool down for firing
	if( pPlayer && m_fTimeUntilCanFire <= 0.0f )
	{
		// Setup the firing offset position to match the sprite
		cocos2d::Vec2 vStartPos = GetSpritePosition() + cocos2d::Vec2( -70.0f *  ( m_iDirection == -1 ? 1 : -1 ), -8.0f );
		
		// Fire projectile
		m_pcProjectileManager->Fire(
			GameConfig::EWeaponType::EWT_R3,
			vStartPos,
			cocos2d::Vec2( m_iDirection, 0.0f ),
			100.0f
		);

		// Increase the fire cool down timer
		m_fTimeUntilCanFire = m_fFireRate;
	}

	return float32();
}