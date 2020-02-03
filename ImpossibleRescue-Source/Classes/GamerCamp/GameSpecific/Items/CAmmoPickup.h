#ifndef _GCOBJAMMOPICKUPS_H_
#define _GCOBJAMMOPICKUPS_H_

#include "../../GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"


class CObjAmmoPickup
	: public CGCObjItem
{
private:
	//The ammo type to change the player to
	GameConfig::EWeaponType m_eAmmoType;

	//How long the player will have this ammo type for
	float m_fTime;

public:

	CObjAmmoPickup( void );
	GCFACTORY_DECLARE_CREATABLECLASS( CObjAmmoPickup );

	virtual void VOnResourceAcquire( void ) override;

	inline float GetTime();
	inline void SetTime( float fTime );

	inline GameConfig::EWeaponType GetAmmoType();
	inline void SetAmmoType( GameConfig::EWeaponType eAmmoType );
};

inline float CObjAmmoPickup::GetTime()
{
	return m_fTime;
}

inline void CObjAmmoPickup::SetTime( float fTime )
{
	m_fTime = fTime;
}

inline GameConfig::EWeaponType CObjAmmoPickup::GetAmmoType()
{
	return m_eAmmoType;
}

inline void CObjAmmoPickup::SetAmmoType( GameConfig::EWeaponType eAmmoType )
{
	m_eAmmoType = eAmmoType;
}

#endif
