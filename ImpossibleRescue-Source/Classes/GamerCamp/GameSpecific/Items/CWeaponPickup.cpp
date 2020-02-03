#ifndef _GCOBJWEAPONPICKUPS_H_
#include "AppDelegate.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "CWeaponPickup.h"
#endif

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjWeaponPickup );
CObjWeaponPickup::CObjWeaponPickup( void )
	: CGCObjItem( GetGCTypeIDOf( CObjWeaponPickup ) )
	, m_eWeaponType( GameConfig::EWeaponType::EWT_Overcharge )
	, m_fTime( GameConfig::k_fOverchargePickupLifeTime )
	, m_fFireRate( GameConfig::k_fOverchargeFireRate )
{

}

//virtual 
void CObjWeaponPickup::VOnResourceAcquire( void )
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	//Get the animations
	char cAnimation[50];
	ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );

	switch ( m_eWeaponType )
	{
	default:
		CCLOG( "INVALID AMMO TYPE" );

		case GameConfig::EWeaponType::EWT_Overcharge:
		//Make it look appropriate
		sprintf( cAnimation, "AutoPickup" );
		break;
// 
// 	case GameConfig::EWeaponType::EWT_Overcharge:
// 		//Make it look appropriate
// 		sprintf( cAnimation, "BurstPickup" );
// 		break;
	}

	ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, cAnimation ) ) );
}