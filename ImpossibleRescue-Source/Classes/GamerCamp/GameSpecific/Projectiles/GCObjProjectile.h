////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJPLAYERPROJECTILE_H_
#define _GCOBJPLAYERPROJECTILE_H_

#include "Helper/GameConfig.h"

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif


//////////////////////////////////////////////////////////////////////////
// This is a sample class derived from CGCObject.
// 
// It demonstrates how you might do projectiles, it relies on 
// CGCObjGroupProjectile to manage the lifetimes of these.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjProjectile
	: public CGCObjSpritePhysics
{
private:
	float			m_fMuzzleVelocity;		// velocity of the projectile
	float			m_fLifetime;			// maximum lifetime
	float			m_fRemainingLifetime;	// remaining lifetime
	float			m_fDamage;				// damage
	cocos2d::Vec2	m_v2Direction;			// firing direction

	GameConfig::EWeaponType	m_eWeaponType;	//The Type of ammo

	cocos2d::Action*	m_pCurrentAction;

public:
	CGCObjProjectile();

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjProjectile()
	{
	}

	// accessors for data members
	inline void SetMuzzleVelocity( float fVelocity );
	inline float GetMuzzleVelocity();

	inline void			SetDamage( float fDamage );
	inline float		GetDamage();
	inline void			SetLifeTime( float fLifeTime );
	inline float		GetLifeTime();
	inline float		GetLifeRemaining();
	inline void			SetDirection( cocos2d::Vec2 v2Direction );
	inline void			SetWeaponType( GameConfig::EWeaponType iAmmoType );
	inline GameConfig::EWeaponType	GetWeaponType();

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

	virtual void VOnResurrected() override;

	virtual void VOnResourceAcquire() override;

	virtual void VOnReset() override;

	virtual void VOnUpdate( f32 fTimeStep ) override;

	void OverrideAction( cocos2d::Action*_pAction );

	// overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////
};



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void CGCObjProjectile::SetMuzzleVelocity( float fVelocity )
{
	m_fMuzzleVelocity = fVelocity;
}

inline float CGCObjProjectile::GetMuzzleVelocity()
{
	return m_fMuzzleVelocity;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void CGCObjProjectile::SetDirection( cocos2d::Vec2 v2Direction )
{
	m_v2Direction = v2Direction;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void CGCObjProjectile::SetDamage( float fDamage )
{
	m_fDamage = fDamage;
}
inline float CGCObjProjectile::GetDamage()
{
	return m_fDamage;
}
inline void CGCObjProjectile::SetLifeTime( float fLifeTime )
{
	m_fLifetime = fLifeTime;
	m_fRemainingLifetime = fLifeTime;
}
inline float CGCObjProjectile::GetLifeTime()
{
	return m_fLifetime;
}
inline float CGCObjProjectile::GetLifeRemaining()
{
	return m_fRemainingLifetime;
}

inline void CGCObjProjectile::SetWeaponType( GameConfig::EWeaponType eWeaponType )
{
	m_eWeaponType = eWeaponType;
}
inline GameConfig::EWeaponType CGCObjProjectile::GetWeaponType()
{
	return m_eWeaponType;
}

#endif // #ifndef _GCOBJPLAYERPROJECTILE_H_