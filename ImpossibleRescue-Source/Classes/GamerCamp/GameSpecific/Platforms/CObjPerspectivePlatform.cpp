#ifndef _GCOBJPERSPECTIVEPLATFORM_H_
#include "CObjPerspectivePlatform.h"
#endif

#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h";

//////////////////////////////////////////////////////////////////////////
// Construcotr
//////////////////////////////////////////////////////////////////////////
CObjPerspectivePlatform::CObjPerspectivePlatform()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjPerspectivePlatform ) )
	, m_strFloorSpriteFrameName( "level_2" )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjPerspectivePlatform );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjPerspectivePlatform::VOnResourceAcquire()
{
	// call parent
	CGCObjSpritePhysics::VOnResourceAcquire();

	// Switch to different wall sprite asset by getting value from GetFactoryCreationParams()->strWallStyle
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );

	// Sprite frame name combine the platform style and location, for example the current platform is level 3 and it is located at the higher level.
	// The sprite frame name should be: Level_3_Platform_High
	// To make this frame name, we can combine string by calling this.
	// - GetFactoryCreationParams()->strPerspectivePlatformStyle + "_" + GetFactoryCreationParams()->strPerspectivePlatformLocation
	std::string strFramename = GetFactoryCreationParams()->strPerspectivePlatformStyle + "_" + GetFactoryCreationParams()->strPerspectivePlatformLocation;
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, strFramename ) ) );
}

void CObjPerspectivePlatform::VOnResurrected()
{
	// call parent
	CGCObjSpritePhysics::VOnResurrected();
}