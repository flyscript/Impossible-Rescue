////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjProjectile.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"

#include "GCObjProjectile.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
// N.B. this is super important!
//
//////////////////////////////////////////////////////////////////////////
CGCObjProjectile::CGCObjProjectile()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjProjectile ) )
	, m_fMuzzleVelocity( 20.f )
	, m_fLifetime( 3 )
	, m_fRemainingLifetime( 3 )
	, m_v2Direction( cocos2d::Vec2::ZERO )
	, m_pCurrentAction( nullptr )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGCObject interface

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CGCObjProjectile, "TexturePacker/Sprites/Egg/Egg.plist", "egg", b2_dynamicBody, false );
//virtual 
void CGCObjProjectile::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CGCObjProjectile );
	CGCObjSpritePhysics::VOnResourceAcquire();

}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjProjectile::VOnReset()
{

	CGCObjSpritePhysics::VOnReset();
	SetZIndex( 2 );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjProjectile::VOnUpdate( f32 fTimeStep )
{

	// changes to desired velocity
	Vec2 v2DesiredVelocityDelta = ( ( m_fMuzzleVelocity *  m_v2Direction ) - GetVelocity() );

	// 1) f = m * a -> a = f/m
	// 2) v = a * t -> a = v/t
	// so f/m = v/t -> f = (v/t) * m
	Vec2 v2RequiredForce = GetPhysicsBody()->GetMass() * ( ( 1.0f / fTimeStep ) * v2DesiredVelocityDelta );
	ApplyForceToCenter( v2RequiredForce );

	// handle lifetime
	m_fRemainingLifetime -= fTimeStep;
	if( m_fRemainingLifetime < 0.0f )
	{
		CGCObjectManager::ObjectKill( this );
	}
}

void CGCObjProjectile::OverrideAction( cocos2d::Action*_pAction )
{
	if( m_pCurrentAction )
	{
		StopAction( m_pCurrentAction );
	}
	m_pCurrentAction = _pAction;
}

void CGCObjProjectile::VOnResurrected( void )
{

	CGCObjSpritePhysics::VOnResurrected();
	GetPhysicsBody()->SetGravityScale( 0.0f );
}