#ifndef _CEXITNODE_H_
#include "CExitNode.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"


//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CExitNode::CExitNode()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CExitNode ) )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CExitNode );

//////////////////////////////////////////////////////////////////////////
// Overrided CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
void CExitNode::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// apply the target sprite
#if _DEBUG
	const char* chDefaultAnimation = "ExitNode";
#elif _RELEASE
	const char* chDefaultAnimation = "ReleaseNode";
#else
	const char* chDefaultAnimation = "ReleaseNode";
#endif
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation ) ) );
}