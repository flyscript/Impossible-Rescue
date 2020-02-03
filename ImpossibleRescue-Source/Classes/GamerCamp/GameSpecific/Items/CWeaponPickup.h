#ifndef _GCOBJWEAPONPICKUPS_H_
#define _GCOBJWEAPONPICKUPS_H_

#include "../../GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"


class CObjWeaponPickup
	: public CGCObjItem
{
private:
	// The weapon type to change the player to
	GameConfig::EWeaponType m_eWeaponType;

	// How long the player will have this weapon type for
	float m_fTime;

	// Overrides the fire rate. Values of 0 should be used to not override it.
	float m_fFireRate;

public:

	CObjWeaponPickup( void );
	GCFACTORY_DECLARE_CREATABLECLASS( CObjWeaponPickup );

	virtual void VOnResourceAcquire( void ) override;

	inline float GetTime();
	inline void SetTime( float fTime );

	inline float GetFireRate();
	inline void SetFireRate( float fFireRate );

	inline GameConfig::EWeaponType GetWeaponType();
	inline void SetWeaponType( GameConfig::EWeaponType eAmmoType );
};

inline float CObjWeaponPickup::GetTime()
{
	return m_fTime;
}

inline void CObjWeaponPickup::SetTime( float fTime )
{
	m_fTime = fTime;
}
inline float CObjWeaponPickup::GetFireRate()
{
	return m_fFireRate;
}

inline void CObjWeaponPickup::SetFireRate( float fFireRate )
{
	m_fFireRate = fFireRate;
}

inline GameConfig::EWeaponType CObjWeaponPickup::GetWeaponType()
{
	return m_eWeaponType;
}

inline void CObjWeaponPickup::SetWeaponType( GameConfig::EWeaponType eWeaponType )
{
	m_eWeaponType = eWeaponType;
}

#endif
