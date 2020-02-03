#ifndef _GCOBJWALL_H_
#include "CObjWall.h"
#endif

#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h";


//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CObjWall::CObjWall()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjWall ) )
	, m_strFloorSpriteFrameName( "level_2" )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjWall );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjWall::VOnResourceAcquire()
{
	// call parent
	CGCObjSpritePhysics::VOnResourceAcquire();

	// Switch to different wall sprite asset by getting value from GetFactoryCreationParams()->strWallStyle
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, GetFactoryCreationParams()->strWallStyle ) ) );
}

void CObjWall::VOnResurrected()
{
	// call parent
	CGCObjSpritePhysics::VOnResurrected();
}
