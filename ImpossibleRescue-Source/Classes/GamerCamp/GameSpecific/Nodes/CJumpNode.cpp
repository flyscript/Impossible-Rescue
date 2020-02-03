#ifndef _CJUMPNODE_H_
#include "CJumpNode.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"


//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CJumpNode::CJumpNode()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CJumpNode ) )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CJumpNode );


//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Methods
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CJumpNode, "TexturePacker/Sprites/Nodes/Nodes.plist", "node", b2_kinematicBody, true );
void CJumpNode::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CJumpNode );
	CGCObjSpritePhysics::VOnResourceAcquire();

	// apply the target sprite
#if _DEBUG
	const char* chDefaultAnimation = "JumpNode";
#elif _RELEASE
	const char* chDefaultAnimation = "ReleaseNode";
#else
	const char* chDefaultAnimation = "ReleaseNode";
#endif
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation ) ) );
}