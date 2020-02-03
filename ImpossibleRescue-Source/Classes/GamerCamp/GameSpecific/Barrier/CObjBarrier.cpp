#include "CObjBarrier.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CObjBarrier::CObjBarrier()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjBarrier ) )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjBarrier );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
void CObjBarrier::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// Run action
	const char* chDefaultAnimation = "BarrierIdle";
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation, 5.0f ) ) );
}