#ifndef _GCOBJDOOR_H_
#include "CObjDoorLeft.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"


//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CObjDoorLeft::CObjDoorLeft()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjDoorLeft ) )
{
}

CObjDoorLeft::~CObjDoorLeft()
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjDoorLeft );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjDoorLeft::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// ready for switch sprite
	const char* chDefaultAnimation = "Door_Left";
	// convert the plist from factory parameter to a map
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	// create animation with plist file and the default animation string
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation ) ) );
}
