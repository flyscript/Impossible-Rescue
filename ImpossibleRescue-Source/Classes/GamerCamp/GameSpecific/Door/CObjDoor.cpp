#ifndef _GCOBJDOOR_H_
#include "CObjDoor.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CObjDoor::CObjDoor()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjDoor ) )
{
}

CObjDoor::~CObjDoor()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjDoor );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjDoor::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// ready for switch sprite
	const char* chDefaultAnimation = "Door_Front";

	// convert the plist from factory parameter to a map
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );

	// create animation with plist file and the default animation string
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation ) ) );
}