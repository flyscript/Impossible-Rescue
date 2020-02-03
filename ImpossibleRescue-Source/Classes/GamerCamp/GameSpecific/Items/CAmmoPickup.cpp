#ifndef _GCOBJAMMOPICKUPS_H_
#include "AppDelegate.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "CAmmoPickup.h"
#endif

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjAmmoPickup );
CObjAmmoPickup::CObjAmmoPickup( void )
	: CGCObjItem( GetGCTypeIDOf( CObjAmmoPickup ) )
	, m_eAmmoType( GameConfig::EWeaponType::EWT_Overcharge )
	, m_fTime( GameConfig::k_fOverchargePickupLifeTime )
{

}

//virtual 
void CObjAmmoPickup::VOnResourceAcquire( void )
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	//Get the animations
	char cAnimation[50];
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );

	switch ( m_eAmmoType )
	{
	default:
		CCLOG( "INVALID AMMO TYPE" );

// 	case GameConfig::EWeaponType::EWT_Normal:
// 		//Make it look appropriate
// 		sprintf( cAnimation, "BulletPickup" );
// 		break;

	case GameConfig::EWeaponType::EWT_Overcharge:
		//Make it look appropriate
		sprintf( cAnimation, "PlantPickup" );
		break;
	}

	ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, cAnimation ) ) );
}