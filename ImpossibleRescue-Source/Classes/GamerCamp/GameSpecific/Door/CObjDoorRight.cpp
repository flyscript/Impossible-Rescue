#ifndef _GCOBJDOORRIGHT_H_
#include "CObjDoorRight.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/EventManager/CEventManager.h"
#include "Helper/GameConfig.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CObjDoorRight::CObjDoorRight()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjDoorRight ) )
	, m_pCurrentAction( nullptr )
{
}

CObjDoorRight::~CObjDoorRight()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjDoorRight );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjDoorRight::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// ready for switch sprite
	const char* chDefaultAnimation = "Door_Right";
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation, 5.0f ) ) );
}

void CObjDoorRight::VOnResurrected()
{
	CGCObjSpritePhysics::VOnResurrected();

	// Subscribe level complete event to change the sprite name
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_LevelCompleted, [ & ] ( void* )
	{
		// Stop the current action to prevent animation mismatch issue
		StopAction( m_pCurrentAction );

		// Read new action from plist and apply it to the sprite
		const char* chOpenDoorAnimation = "Door_Right_Open";
		cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
		m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chOpenDoorAnimation, 5.0f ) ) );
	} );
}

void CObjDoorRight::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// Change back to the normal sprite frame name
	StopAction( m_pCurrentAction );

	const char* chDefaultAnimation = "Door_Right";
	// convert the plist from factory parameter to a map
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	// create animation with plist file and the default animation string
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation, 5.0f ) ) );

}
