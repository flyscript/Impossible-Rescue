#ifndef _CTURNNODE_H_
#include "CTurnNode.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CTurnNode::CTurnNode()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CTurnNode ) )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CTurnNode );

//////////////////////////////////////////////////////////////////////////
// Overrided CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
IN_CPP_CREATION_PARAMS_DECLARE( CTurnNode, "TexturePacker/Sprites/Nodes/Nodes.plist", "node", b2_kinematicBody, true );
void CTurnNode::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CTurnNode );
	CGCObjSpritePhysics::VOnResourceAcquire();

	// apply the target sprite
#if _DEBUG
	const char* chDefaultAnimation = "TurnNode";
#elif _RELEASE
	const char* chDefaultAnimation = "ReleaseNode";
#else
	const char* chDefaultAnimation = "ReleaseNode";
#endif
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, chDefaultAnimation ) ) );
}