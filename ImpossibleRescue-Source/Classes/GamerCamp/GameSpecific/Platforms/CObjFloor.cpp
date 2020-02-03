#ifndef _GCOBJFLOOR_H_
#include "CObjFloor.h"
#endif

#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h";

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CObjFloor::CObjFloor()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjFloor ) )
	, m_strFloorSpriteFrameName( "level_2" )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjFloor );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjFloor::VOnResourceAcquire()
{
	// call parent
	CGCObjSpritePhysics::VOnResourceAcquire();

	// Switch to different floor sprite asset by getting value from GetFactoryCreationParams()->strFloorStyle
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, GetFactoryCreationParams()->strFloorStyle ) ) );
}

void CObjFloor::VOnResurrected()
{
	// call parent
	CGCObjSpritePhysics::VOnResurrected();
}