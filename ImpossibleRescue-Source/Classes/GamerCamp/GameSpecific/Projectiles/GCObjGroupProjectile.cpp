////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

#include "Helper/GameConfig.h"

#include "GamerCamp/GameSpecific/Projectiles/GCObjProjectile.h"

#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"



//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
// N.B. we want reset to put the projectiles on the dead list
//////////////////////////////////////////////////////////////////////////
CGCObjGroupProjectile::CGCObjGroupProjectile()
	: m_iProjectileCount( 500 )
{
	SetResetBehaviour( CGCObjectGroup::EResetDead );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupProjectile::~CGCObjGroupProjectile()
{
}



//////////////////////////////////////////////////////////////////////////
// only handle invaders
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupProjectile::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjProjectile ) == idQueryType );
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupProjectile::VGetTypeId()
{
	return GetGCTypeIDOf( CGCObjGroupProjectile );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectile::CreateProjectiles()
{
	// n.b. these register themselves with this class on creation via CGCObject & CGCObjectManager
	for( u32 uLoop = 0; uLoop < m_iProjectileCount; ++uLoop )
	{
		new CGCObjProjectile();
	}
}



//////////////////////////////////////////////////////////////////////////
//
struct SArrayOfProjectiles
{
	u32 uCount;
	CGCObjProjectile* apProjectiles[ CGCObjectGroup::EMaxGCObjects ];
};
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectile::DestroyProjectiles()
{
	// this iterates the array of registered CGCObjects 
	// calling the supplied functor then deleting them
	DestroyObjectsReverseOrder( [ & ] ( CGCObject* pObject )
	{
		GCASSERT( GetGCTypeIDOf( CGCObjProjectile ) == pObject->GetGCTypeID(), "wrong type!" );
		delete pObject;
	} );
}

void CGCObjGroupProjectile::Fire( GameConfig::EWeaponType eWeaponType, Vec2 v2Position, Vec2 v2Direction, f32 fMaxLifeTime )
{
	SpawnProjectile( eWeaponType, v2Position, v2Direction, fMaxLifeTime );

	//Old code for different weapon & ammo types:
// 	switch (eWeaponType)
// 	{
// 	default:
// 		CCLOG("INVALID WEAPON TYPE");
// 
// 	case GameConfig::EWeaponType::EWT_Auto:
// 		// Auto fire can be managed by the invoking class
// 		// If the code reaches here it will simply pass on to single shot, as it's mechanically the same
// 
// 	case GameConfig::EWeaponType::EWT_Single:
// 		// Simply call SpawnProjectile once
// 		SpawnProjectile(eAmmoType, v2Position, v2Direction, fMaxLifeTime);
// 		break;
// 		
// 	case GameConfig::EWeaponType::EWT_Burst:
// 		// Fire a burst - TODO: fuse axis alignment to make burst work properly when not horizontally aligned
// 		SpawnProjectile(eAmmoType, v2Position - Vec2(0.0f, 10.0f), v2Direction, fMaxLifeTime);
// 		SpawnProjectile(eAmmoType, v2Position + v2Direction * 10.f, v2Direction, fMaxLifeTime);
// 		SpawnProjectile(eAmmoType, v2Position + Vec2(0.0f, 10.0f), v2Direction, fMaxLifeTime);
// 		break;
// 	}
}

//////////////////////////////////////////////////////////////////////////
// Called by the class to fire a specific type of projectile
//////////////////////////////////////////////////////////////////////////
void CGCObjGroupProjectile::SpawnProjectile( GameConfig::EWeaponType eWeaponType, Vec2 v2Position, Vec2 v2Direction, f32 fMaxLifeTime )
{
	// Check we have a projectile to spawn...
	if( GetCountDead() )
	{

		// This case is safe because we know what type this group manages
		CGCObjProjectile* pProjectile = static_cast< CGCObjProjectile* >( GetDeadObject() );

		// Get the animation list
		char cAnimation[ 50 ];
		ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( pProjectile->GetFactoryCreationParams()->strPlistFile );

		switch( eWeaponType )
		{
		default:
			CCLOG( "INVALID AMMO TYPE" );

		case GameConfig::EWeaponType::EWT_Stun:

			// fMaxLifeTime may be used in future to offset the default if needed
			pProjectile->SetLifeTime( GameConfig::k_fStunShotLifetime );

			// Set up the damage
			pProjectile->SetDamage( GameConfig::k_fStunShotDamage );

			// Set up the muzzle velocity
			pProjectile->SetMuzzleVelocity( GameConfig::k_fStunMuzzleVelocity );

			// Pick appropriate animation
			sprintf( cAnimation, "Tase" );

			break;

			// 		case GameConfig::EWeaponType::EWT_Normal:
			// 
			// 			// fMaxLifeTime may be used in future to offset the default if needed
			// 			pProjectile->SetLifeTime( GameConfig::k_fNormalShotLifetime );
			// 
			// 			// Set up the damage
			// 			pProjectile->SetDamage( GameConfig::k_fNormalShotDamage );
			// 
			// 			// Set up the muzzle velocity
			// 			pProjectile->SetMuzzleVelocity( GameConfig::k_fNormalFireSpeed );
			// 
			// 			// Make it look appropriate
			// 			sprintf(cAnimation, "Bullet");
			// 
			// 			break;

		case GameConfig::EWeaponType::EWT_Overcharge:

			// fMaxLifeTime may be used in future to offset the default if needed
			pProjectile->SetLifeTime( GameConfig::k_fChargeShotLifetime );

			// Set up the damage
			pProjectile->SetDamage( GameConfig::k_fOverchargeDamage );

			// Set up the muzzle velocity
			pProjectile->SetMuzzleVelocity( GameConfig::k_fOverchargeMuzzleVelocity );

			// Make it look appropriate
			sprintf( cAnimation, "Plant" );

			break;

		case GameConfig::EWeaponType::EWT_R2:

			// fMaxLifeTime may be used in future to offset the default if needed
			pProjectile->SetLifeTime( GameConfig::k_fR2ShotLifetime );

			// Set up the damage
			pProjectile->SetDamage( GameConfig::k_iR2ShotDamage );

			// Set up the muzzle velocity
			pProjectile->SetMuzzleVelocity( GameConfig::k_fR2ShotMuzzleVelocity );

			// Make it look appropriate
			sprintf( cAnimation, "Poison" );

			break;

		case GameConfig::EWeaponType::EWT_R3:

			// fMaxLifeTime may be used in future to offset the default if needed
			pProjectile->SetLifeTime( GameConfig::k_fR3ShotLifetime );

			// Set up the damage
			pProjectile->SetDamage( GameConfig::k_iR3ShotDamage );

			// Set up the muzzle velocity
			pProjectile->SetMuzzleVelocity( GameConfig::k_fR3ShotMuzzleVelocity );

			// Make it look appropriate
			sprintf( cAnimation, "Old" );
			break;

		}

		// Set rotation of projectile according to direction
		float f360Angle = -v2Direction.getAngle();

		// Set Ammo type to the projectile.
		pProjectile->SetWeaponType( eWeaponType );

		// Comment out as appropriate for selected mechanic
		pProjectile->SetSpriteRotation( f360Angle );

		// Set up the projectile start position
		pProjectile->SetSpritePosition( v2Position );

		// Set it going
		pProjectile->SetDirection( v2Direction );

		// Change the look
		pProjectile->OverrideAction( pProjectile->ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, cAnimation ) ) ) );

		// Resurrect it to fire	it
		VOnObjectResurrect( pProjectile );
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// called from CGCObjectManager::Initialise
//virtual 
void CGCObjGroupProjectile::VOnGroupResourceAcquire()
{
	CreateProjectiles();
	CGCObjectGroup::VOnGroupResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupProjectile::VOnGroupResourceRelease()
{
	// need to do this first as it resets the state of internal lists
	CGCObjectGroup::VOnGroupResourceRelease();
	DestroyProjectiles();
}


